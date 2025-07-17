#pragma leco app
#pragma leco add_impl fod_open_backpack.cpp
#pragma leco add_impl fod_poc.cpp
#pragma leco add_impl fod_start.cpp
export module fod;
import hai;

namespace fod {
  void poc();
  void open_backpack();

  hai::fn<void, float> on_frame {};
};
