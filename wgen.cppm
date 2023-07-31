export module wgen;
import casein;
import qsu;
import tile;

struct ec : tile::terrain::compos {};

class app {
  qsu::main m_q{};
  ec m_ec{};

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
