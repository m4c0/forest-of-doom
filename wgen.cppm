export module wgen;
import casein;
import hai;
import prefabs;
import rng;
import qsu;
import tile;
import tilemap;
import traits;

struct ec : tile::terrain::compos {};

static constexpr const auto max_entropy =
    static_cast<unsigned>(tile::terrain::last);
static_assert(max_entropy < 64);

class eigen {
  unsigned m_entropy = max_entropy;
  traits::ints::uint64_t m_ones = (1UL << max_entropy) - 1UL;
  unsigned m_value = max_entropy;

public:
  constexpr void set_one(tile::terrain::c c) noexcept {
    m_ones |= 1 << (static_cast<unsigned>(c));
    m_entropy++;
  }
  constexpr void set_zero(tile::terrain::c c) noexcept {
    m_ones ^= 1 << (static_cast<unsigned>(c));
    m_entropy--;
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
class map {
  static constexpr const auto margin = 2U;
  static constexpr const auto width = tilemap::width;
  static constexpr const auto height = tilemap::height;

  eigen m_states[height][width]{};

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
};

static const tilemap::map pat = [] {
  tile::terrain::compos tmp{};
  prefabs::wgen(&tmp, 0, 0);
  tilemap::map pat{};
  for (auto &[id, t] : tmp.tiles) {
    auto [x, y, w, h] = area::get(&tmp, id);
    pat.set(x, y, t);
  }
  return pat;
}();

class app {
  qsu::main m_q{};
  ec m_ec{};
  tilemap::map m_pat = pat;

  void setup() {
    m_q.set_grid(16, 16);
    tile::terrain::add_tile(&m_ec, tile::terrain::water, 0, 0);
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
