export module wgen;
import casein;
import hai;
import prefabs;
import qsu;
import tile;
import tilemap;

struct ec : tile::terrain::compos {};

class eigen {
  hai::varray<tile::terrain::c> m_ones{64};

public:
  void set_one(tile::terrain::c c) {
    for (auto n : m_ones) {
      if (n == c)
        return;
    }
    m_ones.push_back(c);
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
static const eigen initial_eigen = [] {
  eigen res{};
  pat.for_each([&](auto x, auto y, auto t) {
    res.set_one(static_cast<tile::terrain::c>(t));
  });
  return res;
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
