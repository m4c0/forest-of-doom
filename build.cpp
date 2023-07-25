#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../hai/build.hpp"
#include "../jute/build.hpp"
#include "../missingno/build.hpp"
#include "../pog/build.hpp"
#include "../quack/build.hpp"
#include "../silog/build.hpp"
#include "../sitime/build.hpp"
#include "../stubby/build.hpp"
#include "../yoyo/build.hpp"

using namespace ecow;

auto base_app(const char *name) {
  auto a = unit::create<app>(name);
  a->add_wsdep("casein", casein());
  a->add_wsdep("hai", hai());
  a->add_wsdep("jute", jute());
  a->add_wsdep("missingno", missingno());
  a->add_wsdep("pog", pog());
  a->add_wsdep("quack", quack());
  a->add_wsdep("silog", silog());
  a->add_wsdep("sitime", sitime());
  a->add_wsdep("stubby", stubby());
  a->add_wsdep("yoyo", yoyo_libc());
  a->add_resource("1_Terrains_and_Fences_16x16.png");
  a->add_resource("11_Camping_16x16.png");
  a->add_resource("Modern_Exteriors_Characters_Scout_16x16_1.png");
  a->add_resource("Modern_UI_Style_1.png");

  auto b = a->add_unit<box>("box");
  b->add_mod("animation");
  b->add_mod("area");
  b->add_mod("collision");
  b->add_mod("cursor");
  b->add_mod("gauge");
  b->add_mod("hud");
  b->add_mod("input");
  b->add_mod("looting");
  b->add_mod("misc");
  b->add_mod("movement");
  b->add_mod("player");
  b->add_mod("ranged");
  b->add_mod("rect");
  b->add_mod("sprite");
  b->add_mod("stopwatch");
  b->add_mod("tilemap");

  auto tile = b->add_mod("tile");
  tile->add_part("camping");
  tile->add_part("common");
  tile->add_part("terrain");
  tile->add_part("ui");

  // Quack Sprite Unit
  auto qsu = b->add_mod("qsu");
  qsu->add_part("debug");
  qsu->add_part("hud");
  qsu->add_part("layer");

  auto pf = b->add_mod("prefabs");
  pf->add_part("island_0");
  pf->add_part("island_1");
  pf->add_part("ocean_0");
  pf->add_part("ui_0");

  // App
  a->add_unit<mod>(name);
  return a;
}

int main(int argc, char **argv) {
  auto all = unit::create<seq>("all");
  all->add_ref(base_app("tme"));
  all->add_ref(base_app("fod"));
  return run_main(all, argc, argv);
}
