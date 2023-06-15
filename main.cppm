export module main;

import casein;
import quack;
import sires;
import silog;

class game {
  quack::renderer *m_r;

public:
  explicit constexpr game(quack::renderer *r) : m_r{r} {}

  void setup() {
    constexpr const auto w = 512;
    constexpr const auto h = 2704;
    m_r->load_atlas(w, h, [](auto *p) {
      sires::open("atlas.img")
          .fmap([p](auto &rdr) { return rdr->read(p, w * h * 4); })
          .map([] { silog::log(silog::info, "%dx%d atlas loaded", w, h); })
          .take([](auto err) {
            silog::log(silog::error, "Error loading atlas: %s", err);
          });
    });
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static quack::renderer r{1};
  static game gg{&r};

  static constexpr const auto k_map = [] {
    casein::key_map res{};
    // res[casein::K_DOWN] = [](auto) { gg.down(); };
    // res[casein::K_LEFT] = [](auto) { gg.left(); };
    // res[casein::K_RIGHT] = [](auto) { gg.right(); };
    // res[casein::K_UP] = [](auto) { gg.up(); };
    return res;
  }();
  static constexpr const auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.setup(); };
    res[casein::KEY_DOWN] = [](auto e) { k_map.handle(e); };
    return res;
  }();

  r.process_event(e);
  // gg.process_event(e);
  map.handle(e);
}
