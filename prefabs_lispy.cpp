module prefabs;
import hai;
import hashley;
import jojo;
import jute;
import lispy;
import no;
import rng;
import silog;
import sires;

using namespace jute::literals;

// TODO: eviction rules
static hashley::fin<hai::sptr<prefabs::tilemap>> g_cache { 127 };

struct tdef_node : lispy::node {
  prefabs::tiledef tdef {};
  bool has_entity   : 1;
  bool has_over     : 1;
  bool has_tile     : 1;
  bool has_collider : 1;
};
static constexpr const auto eval = lispy::eval<tdef_node>;

struct context : lispy::context {
  hai::sptr<prefabs::tilemap> res;
};

const prefabs::tilemap * prefabs::parse(jute::view filename) {
  if (g_cache.has(filename)) return &*g_cache[filename];

  context ctx {
    { .allocator = lispy::allocator<tdef_node>() },
  };
  ctx.fns["random"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as == 0) err(n, "rand requires at least a parameter");
    return eval(ctx, aa[rng::rand(as)]);
  };
  ctx.fns["tiledef"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as == 0) err(n, "tiledef must have at least one attribute");

    auto * nn = new (ctx.allocator()) tdef_node { *n };
    for (auto * c = aa; *c; c++) {
      auto * cc = eval(ctx, *c);
      bool valid = false;
      if (cc->tdef.behaviour.size()) {
        nn->tdef.behaviour = cc->tdef.behaviour;
        valid = true;
      }
      if (cc->tdef.entry.size()) {
        nn->tdef.entry = cc->tdef.entry;
        valid = true;
      }
      if (cc->tdef.loot.size()) {
        nn->tdef.loot = cc->tdef.loot;
        valid = true;
      }
      if (cc->tdef.exit.size()) {
        nn->tdef.exit = cc->tdef.exit;
        valid = true;
      }
      if (cc->has_tile) {
        nn->tdef.tile = cc->tdef.tile;
        nn->has_tile = true;
        valid = true;
      }
      if (cc->has_entity) {
        nn->tdef.entity = cc->tdef.entity;
        nn->has_entity = true;
        valid = true;
      }
      if (cc->has_over) {
        nn->tdef.over = cc->tdef.over;
        nn->has_over = true;
        valid = true;
      }
      if (cc->has_collider) {
        nn->tdef.collision = cc->tdef.collision;
        nn->has_collider = true;
        valid = true;
      }
      if (!valid) err(*c, "invalid element in tiledef");
    }
    return nn;
  };
  ctx.fns["tile"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 5) err(n, "tile should have uv, size and texid");

    auto * nn = new (ctx.allocator()) tdef_node { *n };
    auto & t = nn->tdef.tile;
    t.uv.x   = to_i(eval(ctx, aa[0]));
    t.uv.y   = to_i(eval(ctx, aa[1]));
    t.size.x = to_i(eval(ctx, aa[2]));
    t.size.y = to_i(eval(ctx, aa[3]));
    t.texid  = to_i(eval(ctx, aa[4]));
    nn->has_tile = true;
    return nn;
  };
  ctx.fns["entity"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 5) err(n, "entity should have uv, size and texid");

    auto * nn = new (ctx.allocator()) tdef_node { *n };
    auto & t = nn->tdef.entity;
    t.uv.x   = to_i(eval(ctx, aa[0]));
    t.uv.y   = to_i(eval(ctx, aa[1]));
    t.size.x = to_i(eval(ctx, aa[2]));
    t.size.y = to_i(eval(ctx, aa[3]));
    t.texid  = to_i(eval(ctx, aa[4]));
    nn->has_entity = true;
    return nn;
  };
  ctx.fns["over"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 5) err(n, "over should have uv, size and texid");

    auto * nn = new (ctx.allocator()) tdef_node { *n };
    auto & t = nn->tdef.over;
    t.uv.x   = to_i(eval(ctx, aa[0]));
    t.uv.y   = to_i(eval(ctx, aa[1]));
    t.size.x = to_i(eval(ctx, aa[2]));
    t.size.y = to_i(eval(ctx, aa[3]));
    t.texid  = to_i(eval(ctx, aa[4]));
    nn->has_over = true;
    return nn;
  };
  ctx.fns["collision"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 4) err(n, "collision should have pos and size");

    auto * nn = new (ctx.allocator()) tdef_node { *n };
    auto & c = nn->tdef.collision;
    c.x = to_f(eval(ctx, aa[0]));
    c.y = to_f(eval(ctx, aa[1]));
    c.z = to_f(eval(ctx, aa[2]));
    c.w = to_f(eval(ctx, aa[3]));
    nn->has_collider = true;
    return nn;
  };
  ctx.fns["behaviour"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) err(n, "behaviour requires a value");
    auto val = eval(ctx, aa[0]);
    if (!is_atom(val)) err(n, "behaviour must be an atom");
    auto * nn = new (ctx.allocator()) tdef_node { *n };
    nn->tdef.behaviour = val->atom;
    return nn;
  };
  ctx.fns["exit"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) err(n, "exit point requires a name");
    auto val = eval(ctx, aa[0]);
    if (!is_atom(val)) err(n, "exit point name must be an atom");
    auto * nn = new (ctx.allocator()) tdef_node { *n };
    nn->tdef.exit = val->atom;
    return nn;
  };
  ctx.fns["entry"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) err(n, "entry point requires a name");
    auto val = eval(ctx, aa[0]);
    if (!is_atom(val)) err(n, "entry point must be an atom");
    auto * nn = new (ctx.allocator()) tdef_node { *n };
    nn->tdef.entry = val->atom;
    return nn;
  };
  ctx.fns["loot"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) err(n, "loot table requires a value");
    auto val = eval(ctx, aa[0]);
    if (!is_atom(val)) err(n, "loot table must be an atom");
    auto * nn = new (ctx.allocator()) tdef_node { *n };
    nn->tdef.loot = val->atom;
    return nn;
  };
  ctx.fns["prefab"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    unsigned w = 0;
    unsigned h = as;

    auto & tmap = static_cast<context &>(ctx).res;
    if (tmap) err(n, "multiple prefabs defined");

    for (auto i = 0; i < h; i++) {
      auto c = aa[i];
      if (!is_atom(c)) err(c, "rows in prefabs must be atoms");
      if (w == 0) w = c->atom.size();
      if (c->atom.size() != w) err(c, "inconsistent number of symbols in prefab");
      for (auto x = 0; x < c->atom.size(); x++) {
        auto id = c->atom.subview(x, 1).middle;
        if (!ctx.defs.has(id)) err(c, "unknown tiledef", x);
        auto cid = eval(ctx, ctx.defs[id]);
        // TODO: validate tiledef

        if (!tmap) tmap = hai::sptr { new prefabs::tilemap { w, h } };
        (*tmap)(x, i) = cid->tdef;
      }
    }
    return n;
  };

  run(jojo::read_cstr(filename), ctx);
  if (!ctx.res) return nullptr;

  g_cache[filename] = ctx.res;
  return &*g_cache[filename];
}
const prefabs::tilemap * prefabs::load(jute::view filename) {
  try {
    auto fn = (filename + ".lsp").cstr();
    auto prefab = parse(sires::real_path_name(fn)); 
    if (!prefab) silog::die("missing prefab definition");
    return prefab;
  } catch (const lispy::parser_error & e) {
    silog::log(silog::error, "%s:%d:%d: %s", filename.cstr().begin(), e.line, e.col, e.msg.begin());
    throw;
  }
}
