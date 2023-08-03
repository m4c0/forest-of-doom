export module wgen;
import casein;
import hai;
import prefabs;
import rng;
import qsu;
import silog;
import tile;
import tilemap;
import traits;

static_assert(static_cast<unsigned>(tile::terrain::last) < 64);

struct ec : tile::terrain::compos {};

static const auto max_entropy = static_cast<unsigned>(tile::terrain::last);

class eigen {
  unsigned m_entropy{};
  traits::ints::uint64_t m_ones{};
  unsigned m_value{};

public:
  constexpr void set_one(unsigned c) noexcept {
    auto mask = 1U << c;
    if (m_ones & mask)
      return;
    m_entropy++;
    m_ones |= mask;
  }
  constexpr void set_zero(unsigned c) noexcept {
    auto mask = 1U << c;
    if (m_ones & mask) {
      m_ones ^= 1 << c;
      m_entropy--;
    }
  }

  [[nodiscard]] constexpr auto entropy() const noexcept { return m_entropy; }
  [[nodiscard]] constexpr auto value() const noexcept { return m_value; }

  auto observe() {
    auto r = rng::rand(m_entropy);
    for (auto bit = 0U; bit < max_entropy; bit++) {
      if ((m_ones & (1 << bit)) == 0)
        continue;

      if (--r > 0)
        continue;

      m_ones = 1 << bit;
      m_value = bit;
      return bit;
    }
    // Should never happen
    m_ones = 0;
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
public:
  ieigen() : eigen{cs.e} {}
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
    m_states[min_y][min_x].observe();
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

    map m{};
    m.observe_minimal_entropy();
    m.print(&m_ec);

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
