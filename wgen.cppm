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

class map {
  static constexpr const auto margin = 2U;
  static constexpr const auto width = tilemap::width;
  static constexpr const auto height = tilemap::height;

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
      if (x == 0 || x == width - 1)
        return;
      if (y == 0 || y == height - 1)
        return;

      for (auto dx = -1; dx <= 1; dx++) {
        for (auto dy = -1; dy <= 1; dy++) {
          if (!m_states[min_y + dy][min_x + dx][cs.pat.get(x + dx, y + dy)])
            return;
        }
      }
      for (auto dx = -1; dx <= 1; dx++) {
        for (auto dy = -1; dy <= 1; dy++) {
          m_states[min_y + dy][min_x + dx].set_stage(
              cs.pat.get(x + dx, y + dy));
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
    for (auto y = margin; y < height - margin * 2; y++) {
      for (auto x = margin; x < width - margin * 2; x++) {
        auto t = static_cast<tile::terrain::c>(m_states[y][x].value());
        if (t > tile::terrain::blank)
          tile::terrain::add_tile(ec, t, x, y);
      }
    }
  }
};

class app {
  qsu::main m_q{};
  ec m_ec{};

  void setup() {
    m_q.set_grid(16, 16);
    m_q.center_at(8, 8);

    sitime::stopwatch sw{};
    rng::seed(69);
    map m{};
    m.observe_minimal_entropy();
    m.print(&m_ec);
    silog::log(silog::info, "took %dms", sw.millis());

    m_q.fill(&m_ec);
  }

public:
  void process_event(const casein::event &e) {
    m_q.process_event(e);

    switch (e.type()) {
    case casein::CREATE_WINDOW:
      setup();
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
