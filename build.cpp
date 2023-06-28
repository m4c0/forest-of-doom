#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../jute/build.hpp"
#include "../missingno/build.hpp"
#include "../pog/build.hpp"
#include "../quack/build.hpp"
#include "../silog/build.hpp"
#include "../stubby/build.hpp"
#include "../yoyo/build.hpp"

using namespace ecow;

auto base_app(const char *name) {
  auto a = unit::create<app>(name);
  a->add_wsdep("casein", casein());
  a->add_wsdep("jute", jute());
  a->add_wsdep("missingno", missingno());
  a->add_wsdep("pog", pog());
  a->add_wsdep("quack", quack());
  a->add_wsdep("silog", silog());
  a->add_wsdep("stubby", stubby());
  a->add_wsdep("yoyo", yoyo_libc());
  a->add_resource("11_Camping_16x16.png");
  a->add_resource("Modern_Exteriors_Characters_Scout_16x16_1.png");

  // Components
  a->add_unit<mod>("anime");
  a->add_unit<mod>("chunk");
  a->add_unit<mod>("move");
  a->add_unit<mod>("rect");
  a->add_unit<mod>("tile");

  // Second-order components
  a->add_unit<mod>("sprite");

  // Systems
  a->add_unit<mod>("animation");
  a->add_unit<mod>("cursor");
  a->add_unit<mod>("player");
  a->add_unit<mod>("qsu");
  a->add_unit<mod>("tiles");

  // Second-order systems
  a->add_unit<mod>("tilemap");

  // Third-order systems
  auto pf = a->add_unit<mod>("prefabs");
  pf->add_part("island_0");

  // ECS + Input + App
  a->add_unit<mod>("ecs");
  a->add_unit<mod>("input");
  a->add_unit<mod>(name);
  return a;
}

int main(int argc, char **argv) {
  auto all = unit::create<seq>("all");
  all->add_ref(base_app("tme"));
  all->add_ref(base_app("fod"));
  return run_main(all, argc, argv);
}
