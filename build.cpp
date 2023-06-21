#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../pog/build.hpp"
#include "../quack/build.hpp"
#include "../silog/build.hpp"
#include "../stubby/build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto casein = ::casein();
  auto pog = ::pog();
  auto quack = ::quack();
  auto silog = ::silog();
  auto stubby = ::stubby();

  auto all = unit::create<seq>("all");

  auto fod = all->add_unit<app>("fod");

  auto rect = fod->add_unit<mod>("rect");
  auto sprite = fod->add_unit<mod>("sprite");
  auto tile = fod->add_unit<mod>("tile");

  auto ecs = fod->add_unit<mod>("ecs");
  ecs->add_wsdep("pog", pog);

  auto tiles = fod->add_unit<mod>("tiles");
  auto tilemap = fod->add_unit<mod>("tilemap");

  auto qsu = fod->add_unit<mod>("qsu");
  qsu->add_wsdep("casein", casein);
  qsu->add_wsdep("pog", pog);
  qsu->add_wsdep("quack", quack);
  qsu->add_wsdep("silog", silog);
  qsu->add_wsdep("stubby", stubby);
  qsu->add_resource("11_Camping_16x16.png");

  auto main = fod->add_unit<mod>("main");
  main->add_wsdep("casein", casein);

  auto tme = all->add_unit<app>("tme");
  tme->add_ref(ecs);
  tme->add_ref(rect);
  tme->add_ref(qsu);
  tme->add_ref(sprite);
  tme->add_ref(tile);
  tme->add_ref(tiles);
  tme->add_ref(tilemap);
  tme->add_wsdep("casein", casein);
  tme->add_unit<mod>("tme");

  return run_main(all, argc, argv);
}
