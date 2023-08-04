export module wgen;
import casein;
import hai;
import prefabs;
import rng;
import qsu;
import silog;
import sitime;
import tile;
import tilemap;
import traits;

static_assert(static_cast<unsigned>(tile::terrain::last) < 64);

struct ec : tile::terrain::compos {};

static const auto max_entropy = static_cast<unsigned>(tile::terrain::last);

class bitmask {
  traits::ints::uint64_t m_ones{};
  unsigned m_bit_count{};

public:
  constexpr void set(unsigned c) noexcept {
    auto mask = 1U << c;
    if (m_ones & mask)
      return;
    m_bit_count++;
    m_ones |= mask;
  }
  constexpr void reset(unsigned c) noexcept {
    auto mask = 1U << c;
    if (m_ones & mask) {
      m_ones ^= 1 << c;
      m_bit_count--;
    }
  }

  constexpr void merge(bitmask b) noexcept {
    auto i = (m_ones &= b.m_ones);
    m_bit_count = 0;
    while (i > 0) {
      if (i & 1)
        m_bit_count++;
      i >>= 1;
    }
  }

  [[nodiscard]] constexpr bool operator[](unsigned bit) const noexcept {
    return (m_ones & (1 << bit)) != 0;
  }

  [[nodiscard]] constexpr auto bits() const noexcept { return m_ones; }
  [[nodiscard]] constexpr auto bit_count() const noexcept {
    return m_bit_count;
  }
};

class eigen {
  bitmask m_bits{};
  unsigned m_value{};

public:
  constexpr void set_one(unsigned c) noexcept { m_bits.set(c); }
  constexpr void merge(bitmask b) noexcept { m_bits.merge(b); }

  [[nodiscard]] constexpr bool operator[](unsigned bit) const noexcept {
    return m_bits[bit];
  }

  [[nodiscard]] constexpr auto bits() const noexcept { return m_bits; }
  [[nodiscard]] constexpr auto entropy() const noexcept {
    return m_bits.bit_count();
  }
  [[nodiscard]] constexpr auto value() const noexcept { return m_value; }

  auto observe() {
    auto r = rng::rand(entropy());
    for (auto bit = 0U; bit < max_entropy; bit++) {
      if (!m_bits[bit])
        continue;

      if (r-- > 0)
        continue;

      m_bits = {};
      m_bits.set(bit);
      m_value = bit;
      return bit;
    }
    // Should never happen
    m_bits = {};
    m_value = 0;
    return 0U;
  }
};

struct consts {
  tilemap::map pat;
  eigen e;
};
static const consts cs = [] {
  tile::terrain::compos tmp{};
  prefabs::wgen(&tmp, 0, 0);

  tilemap::map pat{};
  eigen e{};
  for (auto &[id, t] : tmp.tiles) {
    auto [x, y, w, h] = area::get(&tmp, id);
    pat.set(x, y, t);
    e.set_one(t);
  }
  return consts{pat, e};
}();
class ieigen : public eigen {
  bitmask m_stage{};

public:
  ieigen() : eigen{cs.e} {}

  auto stage() { return m_stage; }

  void reset_stage() { m_stage = {}; }
  void set_stage(unsigned i) { m_stage.set(i); }
  void apply_stage() { merge(m_stage); }
};

static constexpr const auto width = 32;
static constexpr const auto height = 32;
class map {
  static constexpr const auto margin = 2U;

  ieigen m_states[height][width]{};

public:
  void observe_minimal_entropy() {
    unsigned min_x{};
    unsigned min_y{};
    unsigned min_e = max_entropy;
    for (auto y = margin; y < height - margin * 2; y++) {
      for (auto x = margin; x < width - margin * 2; x++) {
        if (m_states[y][x].value())
          continue;

        auto e = m_states[y][x].entropy();
        if (e == 0)
          continue;
        if (min_e < e)
          continue;

        // TODO: randomize all with same min_e
        if (min_e == e && rng::randf() > 0.5)
          continue;

        min_e = e;
        min_x = x;
        min_y = y;
      }
    }
    if (min_e == max_entropy) {
      return;
    }
    auto n = m_states[min_y][min_x].observe();

    for (auto dx = -1; dx <= 1; dx++) {
      for (auto dy = -1; dy <= 1; dy++) {
        m_states[min_y + dy][min_x + dx].reset_stage();
      }
    }

    cs.pat.for_each([&](int x, int y, unsigned t) {
      if (n != t)
        return;

      for (auto dx = -1; dx <= 1; dx++) {
        for (auto dy = -1; dy <= 1; dy++) {
          auto px = ((x + dx) + tilemap::width) % tilemap::width;
          auto py = ((y + dy) + tilemap::height) % tilemap::height;
          if (!m_states[min_y + dy][min_x + dx][cs.pat.get(px, py)])
            return;
        }
      }
      for (auto dx = -1; dx <= 1; dx++) {
        for (auto dy = -1; dy <= 1; dy++) {
          auto px = ((x + dx) + tilemap::width) % tilemap::width;
          auto py = ((y + dy) + tilemap::height) % tilemap::height;
          m_states[min_y + dy][min_x + dx].set_stage(cs.pat.get(px, py));
        }
      }
    });
    for (auto dx = -1; dx <= 1; dx++) {
      for (auto dy = -1; dy <= 1; dy++) {
        m_states[min_y + dy][min_x + dx].apply_stage();
      }
    }
  }

  void print(tile::terrain::compos *ec) const {
    for (auto y = 0; y < height; y++) {
      for (auto x = 0; x < width; x++) {
        auto t = static_cast<tile::terrain::c>(m_states[y][x].value());

        if (t != tile::terrain::blank)
          tile::terrain::add_tile(ec, t, x, y);

        sprite::c s{
            .layer = sprite::layers::debug,
            .colour = {0, 0, 0, 0.5},
            .dim = 1,
        };
        rect r{static_cast<float>(x), static_cast<float>(y), 1, 1};

        auto e = m_states[y][x].entropy();
        if (e < 4) {
          s.colour.r = e / 4.f;
        } else if (e < 16) {
          s.colour.g = e / 16.f;
        } else {
          s.colour.b = e / 64.f;
        }

        sprite::add(ec, s, r);
      }
    }
  }
};

class app {
  qsu::main m_q{};
  ec m_ec{};
  hai::uptr<map> m_map;

  void setup() {
    m_q.set_grid(32, 32);
    m_q.center_at(16, 16);

    rng::seed(69);

    m_map = hai::uptr<map>::make();
    step();
  }

  void step() {
    m_ec = {};
    m_map->observe_minimal_entropy();
    m_map->print(&m_ec);
    m_q.fill(&m_ec);
  }

public:
  void process_event(const casein::event &e) {
    m_q.process_event(e);

    switch (e.type()) {
    case casein::CREATE_WINDOW:
      setup();
      break;
    case casein::KEY_DOWN:
      step();
      break;
    default:
      break;
    }
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static app gg{};
  gg.process_event(e);
}
