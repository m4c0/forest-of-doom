#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../hai/build.hpp"
#include "../jute/build.hpp"
#include "../missingno/build.hpp"
#include "../pog/build.hpp"
#include "../quack/build.hpp"
#include "../rng/build.hpp"
#include "../silog/build.hpp"
#include "../sitime/build.hpp"
#include "../stubby/build.hpp"
#include "../traits/build.hpp"
#include "../yoyo/build.hpp"

using namespace ecow;

auto base_app(const char *name) {
  auto a = unit::create<app>(name);
  a->add_wsdep("casein", casein());
  a->add_wsdep("hai", hai());
  a->add_wsdep("jute", jute());
  a->add_wsdep("missingno", missingno());
  a->add_wsdep("pog", pog());
  a->add_wsdep("rng", rng());
  a->add_wsdep("quack", quack());
  a->add_wsdep("silog", silog());
  a->add_wsdep("sitime", sitime());
  a->add_wsdep("stubby", stubby());
  a->add_wsdep("traits", traits());
  a->add_wsdep("yoyo", yoyo_libc());
  a->add_resource("1_Terrains_and_Fences_16x16.png");
  a->add_resource("11_Camping_16x16.png");
  a->add_resource("Modern_Exteriors_Characters_Scout_16x16_1.png");
  a->add_resource("Modern_UI_Style_1.png");
  a->add_unit<box>(name);
  return a;
}

int main(int argc, char **argv) {
  auto all = unit::create<seq>("all");
  all->add_ref(base_app("wgen"));
  all->add_ref(base_app("tme"));
  all->add_ref(base_app("fod"));
  return run_main(all, argc, argv);
}
