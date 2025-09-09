module loots;
import jojo;
import lispy;
import rng;
import silog;
import sires;

struct loot_node : lispy::node {
  loots::item item;
  bool has_item : 1;
};
static constexpr const auto clone = lispy::clone<loot_node>;
static constexpr const auto eval = lispy::eval<loot_node>;

hai::array<loots::item> loots::parse(jute::view filename) {
  lispy::ctx_w_mem<loot_node> cm {};
  cm.ctx.fns["random"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as == 0) err(n, "rand requires at least a parameter");
    return eval(ctx, aa[rng::rand(as)]);
  };
  cm.ctx.fns["item"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 2) err(n, "item expects two coordinates");

    auto * nn = clone(&ctx, n);
    nn->item.sprite.x = to_i(aa[0]);
    nn->item.sprite.y = to_i(aa[1]);
    nn->has_item = true;
    return nn;
  };

  hai::array<loots::item> res { 8 };
  unsigned i = 0;
  run(jojo::read_cstr(filename), cm.ctx, [&](auto * node) {
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
  } catch (const lispy::parser_error & e) {
    silog::log(silog::error, "%s:%d:%d: %s", filename.begin(), e.line, e.col, e.msg.begin());
    return {};
  } catch (...) {
    silog::log(silog::error, "%s: unknown error", filename.begin());
    return {};
  }
}

