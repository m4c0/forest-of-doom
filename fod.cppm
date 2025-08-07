#pragma leco app
#pragma leco add_impl fod_open_backpack.cpp
#pragma leco add_impl fod_open_player_inv.cpp
#pragma leco add_impl fod_poc.cpp
#pragma leco add_impl fod_start.cpp
export module fod;
import hai;
import loots;

namespace fod {
  void poc();
  void open_backpack(hai::array<loots::item> *);
  void open_player_inv();

  hai::fn<void, float> on_frame {};
};
