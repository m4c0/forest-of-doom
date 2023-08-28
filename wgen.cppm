export module wgen;
import casein;
import mapgen;
import rng;
import tile;
import qsu;
import silog;
import hai;

struct ec : tile::terrain::compos {};

class app {
  static const auto map_size = mapgen::map_size;

  qsu::main m_q{};
  ec m_ec{};
  bool m_frozen = false;

  void print() {
    m_ec = {};
    mapgen::gen(&m_ec);
    m_q.fill(&m_ec);
  }

  void setup() {
    m_q.set_grid(map_size, map_size);
    m_q.center_at(map_size / 2, map_size / 2);

    rng::seed(69);

    print();
  }

  void step() {
    if (m_frozen)
      return;

    // m_map.fill_random_spot();
    print();
  }

  void move_map(int dx, int dy) {
    auto [cx, cy] = m_q.center();
    m_q.center_at(cx + dx, cy + dy);
  }

public:
  void process_event(const casein::event &e) {
    m_q.process_event(e);

    switch (e.type()) {
    case casein::CREATE_WINDOW:
      setup();
      break;
    case casein::KEY_DOWN: {
      switch (*e.as<casein::events::key_down>()) {
      case casein::K_LEFT:
        move_map(-1, 0);
        break;
      case casein::K_RIGHT:
        move_map(1, 0);
        break;
      case casein::K_UP:
        move_map(0, 1);
        break;
      case casein::K_DOWN:
        move_map(0, -1);
        break;
      default:
        step();
        break;
      }
      break;
    }
    default:
      break;
    }
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static app gg{};
  gg.process_event(e);
}
