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

  auto all = unit::create<app>("fod");

  auto rect = all->add_unit<mod>("rect");
  auto sprite = all->add_unit<mod>("sprite");
  auto tile = all->add_unit<mod>("tile");

  auto ecs = all->add_unit<mod>("ecs");
  ecs->add_wsdep("pog", pog);

  auto tiles = all->add_unit<mod>("tiles");
  auto tilemap = all->add_unit<mod>("tilemap");

  auto qsu = all->add_unit<mod>("qsu");
  qsu->add_wsdep("casein", casein);
  qsu->add_wsdep("pog", pog);
  qsu->add_wsdep("quack", quack);
  qsu->add_wsdep("silog", silog);
  qsu->add_wsdep("stubby", stubby);

  auto main = all->add_unit<mod>("main");
  main->add_wsdep("casein", casein);
  main->add_resource("11_Camping_16x16.png");

  return run_main(all, argc, argv);
}
