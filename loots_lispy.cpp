module loots;
import jojo;
import lispy;
import rng;
import silog;
import sires;

struct loot_node : lispy::node {
};
static constexpr const auto eval = lispy::eval<loot_node>;

struct context : lispy::context {
  hai::array<loots::item> * res;
  unsigned i = 0;
};

hai::array<loots::item> loots::parse(jute::view filename) {
  hai::array<loots::item> res { 8 };

  context ctx {
    { .allocator = lispy::allocator<loot_node>() },
  };
  ctx.res = &res;
  ctx.fns["random"] = [](auto n, auto aa, auto as) -> const lispy::node * {
    if (as == 0) err(n, "rand requires at least a parameter");
    return eval(n->ctx, aa[rng::rand(as)]);
  };
  ctx.fns["item"] = [](auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 2) err(n, "item expects two coordinates");

    auto & res = *static_cast<context *>(n->ctx)->res;
    auto & i = static_cast<context *>(n->ctx)->i;
    if (i == res.size()) err(n, "too many items for this inventory");

    res[i++] = {
      .sprite { to_i(aa[0]), to_i(aa[1]) },
    };
    return n;
  };

  lispy::run<loot_node>(jojo::read_cstr(filename), &ctx);

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

