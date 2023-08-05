#include "../ecow/ecow.hpp"
#include "../quack/build.hpp"
#include "../rng/build.hpp"
#include "../sitime/build.hpp"

using namespace ecow;

auto base_app(const char *name) {
  auto a = unit::create<app>(name);
  a->add_resource("1_Terrains_and_Fences_16x16.png");
  a->add_resource("11_Camping_16x16.png");
  a->add_resource("Modern_Exteriors_Characters_Scout_16x16_1.png");
  a->add_resource("Modern_UI_Style_1.png");

  auto b = a->add_unit<box>(name);
  b->add_wsdep("quack", quack());
  b->add_wsdep("rng", rng());
  b->add_wsdep("sitime", sitime());
  return a;
}

int main(int argc, char **argv) {
  auto all = unit::create<seq>("all");
  all->add_ref(base_app("wgen"));
  all->add_ref(base_app("tme"));
  all->add_ref(base_app("fod"));
  return run_main(all, argc, argv);
}
