module loots;
import lispy;

using namespace lispy;

hai::array<loots::item> loots::load(jute::view table_name) {
  hai::array<node> buf { 10240 };
  auto cur = buf.begin();
  alloc_node = [&] -> void * {
    if (cur == buf.end()) throw 0;
    return cur++;
  };

  context ctx {};
  hai::array<loots::item> res { 8 };
  run(table_name, ctx, [&](auto * node) {
  });

  alloc_node = [] -> void * { return nullptr; };
  return res;
}
