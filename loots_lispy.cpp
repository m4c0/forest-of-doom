module loots;
import lispy;
import silog;

using namespace lispy;

struct loot_node : node {
  loots::item item;
};

hai::array<loots::item> loots::parse(jute::view filename) {
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
  run(filename, ctx, [&](auto * node) {
  });

  alloc_node = [] -> void * { return nullptr; };
  return res;
}
hai::array<loots::item> loots::load(jute::view table_name) {
  auto filename = (table_name + ".lsp").cstr();
  try {
    return parse(filename);
  } catch (const parser_error & e) {
    silog::log(silog::error, "%s:%d:%d: %s", filename.begin(), e.line, e.col, e.msg.begin());
    return {};
  } catch (...) {
    silog::log(silog::error, "%s: unknown error", filename.begin());
    return {};
  }
}

