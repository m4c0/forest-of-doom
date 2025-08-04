module loots;
import lispy;

using namespace lispy;

struct loot_node : node {
  loots::item item;
};

hai::array<loots::item> loots::load(jute::view table_name) {
  hai::array<loot_node> buf { 10240 };
  auto cur = buf.begin();
  alloc_node = [&] -> void * {
    if (cur == buf.end()) throw 0;
    return cur++;
  };

  context ctx {};
  ctx.fns["item"] = [](auto ctx, auto n, auto aa, auto as) -> const node * {
    if (as != 2) err(n, "item expects two coordinates");

    auto * nn = new loot_node { *n };
    nn->item.sprite.x = to_i(aa[0]);
    nn->item.sprite.y = to_i(aa[1]);
    return nn;
  };

  hai::array<loots::item> res { 8 };
  run(table_name, ctx, [&](auto * node) {
  });

  alloc_node = [] -> void * { return nullptr; };
  return res;
}
