#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../pog/build.hpp"
#include "../quack/build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto casein = ::casein();
  auto pog = ::pog();
  auto quack = ::quack();

  auto all = unit::create<app>("fod");

  auto main = all->add_unit<mod>("main");
  main->add_wsdep("casein", casein);
  main->add_wsdep("quack", quack);

  return run_main(all, argc, argv);
}
