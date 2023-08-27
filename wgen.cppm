export module wgen;
import casein;
import eigen;
import rng;
import tile;
import qsu;
import silog;
import hai;

static_assert(static_cast<unsigned>(tile::terrain::last) < 64);

struct ec : tile::terrain::compos {};

static constexpr const auto width = 32;
static constexpr const auto height = 32;

constexpr const auto pats = [] {
  eigen::pat_list p{};

  p[0].set_row(0, "...");
  p[0].set_row(1, ".XX");
  p[0].set_row(2, ".XX");
  // p[0].probability() = 1.f;

  p[1].set_row(0, ".XX");
  p[1].set_row(1, ".XX");
  p[1].set_row(2, ".XX");
  // p[1].probability() = 1.f;

  p[2].set_row(0, "XXX");
  p[2].set_row(1, "XXX");
  p[2].set_row(2, "XXX");
  // p[2].probability() = 1.f;

  p[3].set_row(0, "...");
  p[3].set_row(1, "...");
  p[3].set_row(2, "...");
  // p[3].probability() = 1.f;

  (p[4] = p[0]).rotate();
  (p[5] = p[4]).rotate();
  (p[6] = p[5]).rotate();

  (p[7] = p[1]).rotate();
  (p[8] = p[7]).rotate();
  (p[9] = p[8]).rotate();

  p[10].set_row(0, "XXX");
  p[10].set_row(1, "XXX");
  p[10].set_row(2, ".XX");
  // p[10].probability() = 1.f;

  (p[11] = p[10]).rotate();
  (p[12] = p[11]).rotate();
  (p[13] = p[12]).rotate();

  return p;
}();

class app {
  qsu::main m_q{};
  ec m_ec{};
  eigen::map m_map{&pats, width, height};
  bool m_frozen = false;

  void print() {
    m_ec = {};
    // m_map->print(&m_ec);
    m_q.fill(&m_ec);
  }

  void setup() {
    m_q.set_grid(32, 32);
    m_q.center_at(16, 16);

    rng::seed(69);

    m_map.draw_border(2);
    print();
  }

  void step() {
    if (m_frozen)
      return;

    m_map.fill_random_spot();
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
