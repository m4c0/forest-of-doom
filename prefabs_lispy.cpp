module prefabs;
import hai;
import hashley;
import jojo;
import jute;
import no;
import rng;
import silog;
import sires;

using namespace jute::literals;

class reader : no::no {
  jute::view m_data;
  unsigned m_pos {};

public:
  explicit reader(jute::view data) : m_data { data } {}

  explicit operator bool() const {
    return m_pos < m_data.size();
  }

  unsigned loc() const { return m_pos; }
  const char * mark() const { return m_data.begin() + m_pos; }

  char peek() {
    if (m_pos >= m_data.size()) return 0;
    return m_data[m_pos];
  }
  char take() {
    if (m_pos >= m_data.size()) return 0;
    return m_data[m_pos++];
  }

  [[noreturn]] void err(jute::view msg, unsigned loc) const {
    unsigned l = 1;
    unsigned last = 0;
    for (auto i = 0; i < loc; i++) {
      if (m_data[i] == '\n') {
        last = i;
        l++;
      }
    }
    throw prefabs::parser_error { msg, l, loc - last };
  }
  [[noreturn]] void err(jute::view msg) const {
    err(msg, m_pos);
  }

  jute::view token(const char * start) const {
    return { start, static_cast<unsigned>(mark() - start) };
  }
};

static bool is_atom_char(char c) {
  return c > ' ' && c <= '~' && c != ';' && c != '(' && c != ')';
}
static jute::view next_atom_token(const char * start, reader & r) {
  while (is_atom_char(r.peek())) r.take();
  return r.token(start);
}
static void comment(reader & r) {
  while (r && r.take() != '\n') continue;
}
static jute::view next_token(reader & r) {
  while (r) {
    auto start = r.mark();
    switch (char c = r.take()) {
      case '\n':
      case '\r':
      case '\t':
      case ' ': break;
      case '(': return r.token(start);
      case ')': return r.token(start);
      case ';': comment(r); break;
      default: {
        if (is_atom_char(c)) return next_atom_token(start, r);
        r.err("character not allowed here");
        break;
      }
    }
  }
  return {};
}

struct node {
  jute::view atom {};
  hai::uptr<node> list {};
  prefabs::tiledef tdef {};
  hai::uptr<prefabs::tilemap> tmap {};
  const reader * r {};
  unsigned loc {};
  bool has_entity   : 1;
  bool has_tile     : 1;
  bool has_collider : 1;

  hai::uptr<node> next {};
};

static node next_list(reader & r) {
  node res {
    .r = &r,
    .loc = r.loc(),
  };
  auto * n = &res.list;
  while (r) {
    auto token = next_token(r);
    if (token == ")") return res;
    if (token == "") break;

    auto nn = (token == "(") ?
      new node { next_list(r) } :
      new node {
        .atom = token,
        .r = &r,
        .loc = static_cast<unsigned>(r.loc() - token.size()),
      };
    n->reset(nn);
    n = &((*n)->next);
  }
  r.err("unbalanced open parenthesis");
}
static node next_node(reader & r) {
  if (!r) return {};

  auto token = next_token(r);
  if (token == "(") {
    return next_list(r);
  } else if (token == ")") {
    r.err("unbalanced close parenthesis");
  } else {
    return { 
      .atom = token,
      .r = &r,
      .loc = static_cast<unsigned>(r.loc() - token.size()),
    };
  }
}

static bool is_atom(const node & n) { return n.atom.size(); }

static auto ls(const node & n) {
  unsigned sz = 0;
  for (auto nn = &*n.list; nn; nn = &*nn->next) sz++;
  return sz;
}

static float to_f(const node & n) {
  if (!is_atom(n)) n.r->err("non-numerical coordinate", n.loc);
  try {
    return jute::to_f(n.atom);
  } catch (...) {
    n.r->err("invalid number", n.loc);
  }
}
static int to_i(const node & n) {
  if (!is_atom(n)) n.r->err("non-numerical coordinate", n.loc);
  try {
    return jute::to_u32(n.atom);
  } catch (...) {
    n.r->err("invalid number", n.loc);
  }
}

struct context {
  hashley::fin<node> defs { 127 };
};
static void eval(context & ctx, node & n) {
  if (!n.list) return;
  if (!is_atom(*n.list)) n.r->err("expecting an atom", n.list->loc);

  auto fn = n.list->atom;
  auto args = &*n.list->next;

  if (fn == "def") {
    if (ls(n) != 3) n.r->err("def requires a name and a value");
    if (!is_atom(*args)) n.r->err("def name must be an atom");
    ctx.defs[args->atom] = traits::move(*args->next.release());
    return;
  }

  for (auto nn = &*n.list; nn; nn = &*nn->next) eval(ctx, *nn);
  
  if (fn == "tiledef") {
    if (ls(n) < 2) n.r->err("tiledef must have at least name", n.loc);

    for (auto * c = args; c; c = &*c->next) {
      bool valid = false;
      if (c->tdef.behaviour.size()) {
        n.tdef.behaviour = c->tdef.behaviour;
        valid = true;
      }
      if (c->tdef.loot.size()) {
        n.tdef.loot = c->tdef.loot;
        valid = true;
      }
      if (c->has_tile) {
        n.tdef.tile = c->tdef.tile;
        n.has_tile = true;
        valid = true;
      }
      if (c->has_entity) {
        n.tdef.entity = c->tdef.entity;
        n.has_entity = true;
        valid = true;
      }
      if (c->has_collider) {
        n.tdef.collision = c->tdef.collision;
        n.has_collider = true;
        valid = true;
      }
      if (!valid) n.r->err("invalid element in tiledef", c->loc);
    }
  } else if (fn == "tile") {
    if (ls(n) != 6) n.r->err("tile should have uv, size and texid", n.loc);

    auto & t = n.tdef.tile;
    t.uv.x   = to_i(*args);
    t.uv.y   = to_i(*(args = &*args->next));
    t.size.x = to_i(*(args = &*args->next));
    t.size.y = to_i(*(args = &*args->next));
    t.texid  = to_i(*(args = &*args->next));
    n.has_tile = true;
  } else if (fn == "entity") {
    if (ls(n) != 6) n.r->err("entity should have uv, size and texid", n.loc);

    auto & t = n.tdef.entity;
    t.uv.x   = to_i(*args);
    t.uv.y   = to_i(*(args = &*args->next));
    t.size.x = to_i(*(args = &*args->next));
    t.size.y = to_i(*(args = &*args->next));
    t.texid  = to_i(*(args = &*args->next));
    n.has_entity = true;
  } else if (fn == "collision") {
    if (ls(n) != 5) n.r->err("collision should have pos and size", n.loc);

    auto & c = n.tdef.collision;
    c.x = to_f(*args);
    c.y = to_f(*(args = &*args->next));
    c.z = to_f(*(args = &*args->next));
    c.w = to_f(*(args = &*args->next));
    n.has_collider = true;
  } else if (fn == "behaviour") {
    if (ls(n) != 2) n.r->err("behaviour requires a value", n.loc);
    if (!is_atom(*args)) n.r->err("behaviour must be an atom", n.loc);
    n.tdef.behaviour = args->atom;
  } else if (fn == "loot") {
    if (ls(n) != 2) n.r->err("loot table requires a value", n.loc);
    if (!is_atom(*args)) n.r->err("loot table must be an atom", n.loc);
    n.tdef.loot = args->atom;
  } else if (fn == "prefab") {
    if (ls(n) != prefabs::height + 1) n.r->err("incorrect number of rows in prefab", n.loc);

    n.tmap.reset(new prefabs::tilemap {});
    auto & map = *n.tmap;
    auto i = 0;
    for (auto * c = args; c; c = &*c->next, i++) {
      if (!is_atom(*c)) n.r->err("rows in prefabs must be atoms", c->loc);
      if (c->atom.size() != prefabs::width) n.r->err("incorrect number of symbols in prefab", c->loc);
      for (auto x = 0; x < c->atom.size(); x++) {
        auto id = c->atom.subview(x, 1).middle;
        if (!ctx.defs.has(id)) n.r->err("unknown tiledef", c->loc + x);
        auto & c = ctx.defs[id];
        eval(ctx, c);
        map(x, i) = c.tdef;
      }
    }
  } else if (fn == "random") {
    if (ls(n) == 0) n.r->err("rand requires at least a parameter");
    int r = rng::rand(ls(n) - 1);
    for (auto i = 0; i < r; i++) args = &*args->next;
    n.atom = args->atom;
    n.tdef = args->tdef;
    n.has_tile = args->has_tile;
    n.has_entity = args->has_entity;
    n.has_collider = args->has_collider;
  } else if (ctx.defs.has(fn)) {
    auto & c = ctx.defs[fn];
    eval(ctx, c);
    n.atom = c.atom;
    n.tdef = c.tdef;
    n.has_tile = c.has_tile;
    n.has_entity = c.has_entity;
    n.has_collider = c.has_collider;
  } else {
    n.r->err(*("invalid function name: "_hs + fn), n.loc);
  }
}

// TODO: eviction rules
hashley::fin<prefabs::tilemap> g_cache { 127 };

const prefabs::tilemap * prefabs::parse(jute::view filename) {
  if (g_cache.has(filename)) return &g_cache[filename];

  auto code = jojo::read_cstr(filename);

  context ctx {};
  reader r { code };
  hai::uptr<prefabs::tilemap> prefab {};

  while (r) {
    auto n = next_node(r);
    eval(ctx, n);
    if (n.tmap) prefab = traits::move(n.tmap);
  }
  if (!prefab) return nullptr;

  g_cache[filename] = traits::move(*prefab.release());
  return &g_cache[filename];
}
const prefabs::tilemap * prefabs::load(jute::view filename) {
  try {
    auto prefab = parse(sires::real_path_name(filename)); 
    if (!prefab) silog::die("missing prefab definition");
    return prefab;
  } catch (const prefabs::parser_error & e) {
    silog::log(silog::error, "%s:%d:%d: %s", filename.cstr().begin(), e.line, e.col, e.msg.begin());
    throw;
  }
}
