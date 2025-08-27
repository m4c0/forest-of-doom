module loots;
import lispy;
import rng;
import silog;
import sires;

using namespace lispy;

struct loot_node : node {
  loots::item item;
  bool has_item : 1;
};

hai::array<loots::item> loots::parse(jute::view filename) {
  hai::array<loot_node> buf { 10240 };
  auto cur = buf.begin();

  context ctx {};
  ctx.allocator = [&] -> node * {
    if (cur == buf.end()) throw 0;
    return cur++;
  };
  ctx.fns["random"] = [](auto ctx, auto n, auto aa, auto as) -> const node * {
    if (as == 0) err(n, "rand requires at least a parameter");
    return eval(ctx, aa[rng::rand(as)]);
  };
  ctx.fns["item"] = [](auto ctx, auto n, auto aa, auto as) -> const node * {
    if (as != 2) err(n, "item expects two coordinates");

    auto * nn = new (ctx.allocator()) loot_node { *n };
    nn->item.sprite.x = to_i(aa[0]);
    nn->item.sprite.y = to_i(aa[1]);
    nn->has_item = true;
    return nn;
  };

  hai::array<loots::item> res { 8 };
  unsigned i = 0;
  run(filename, ctx, [&](auto * node) {
    auto nn = static_cast<const loot_node *>(node);
    if (!nn->has_item) return; // defs, etc

    if (i == res.size()) err(nn, "too many items for this inventory");
    res[i++] = nn->item;
  });

  return res;
}
hai::array<loots::item> loots::load(jute::view table_name) {
  auto filename = (table_name + ".lsp").cstr();
  try {
    return parse(sires::real_path_name(filename));
  } catch (const parser_error & e) {
    silog::log(silog::error, "%s:%d:%d: %s", filename.begin(), e.line, e.col, e.msg.begin());
    return {};
  } catch (...) {
    silog::log(silog::error, "%s: unknown error", filename.begin());
    return {};
  }
}

