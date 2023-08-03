export module wgen;
import casein;
import hai;
import prefabs;
import qsu;
import tile;
import tilemap;
import traits;

struct ec : tile::terrain::compos {};

class eigen {
  static_assert(static_cast<unsigned>(tile::terrain::last) < 64);
  unsigned m_entropy = static_cast<unsigned>(tile::terrain::last);
  traits::ints::uint64_t m_ones = (1 << m_entropy) - 1;

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
