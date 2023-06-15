#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../pog/build.hpp"
#include "../quack/build.hpp"
#include "../silog/build.hpp"
#include "../sires/build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto casein = ::casein();
  auto pog = ::pog();
  auto quack = ::quack();
  auto silog = ::silog();
  auto sires = ::sires();

  auto all = unit::create<app>("fod");

  auto main = all->add_unit<mod>("main");
  main->add_wsdep("casein", casein);
  main->add_wsdep("quack", quack);
  main->add_wsdep("silog", silog);
  main->add_wsdep("sires", sires);

  return run_main(all, argc, argv);
}
