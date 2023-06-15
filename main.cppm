export module main;

import casein;
import quack;

extern "C" void casein_handle(const casein::event &e) {
  static quack::renderer r{1};
  // static game gg{};

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
    // res[casein::CREATE_WINDOW] = [](auto) { gg.reset(); };
    res[casein::KEY_DOWN] = [](auto e) { k_map.handle(e); };
    return res;
  }();

  r.process_event(e);
  // gg.process_event(e);
  map.handle(e);
}
