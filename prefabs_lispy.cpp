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

struct data {
  prefabs::tiledef tdef {};
  hai::uptr<prefabs::tilemap> tmap {};
  bool has_entity   : 1;
  bool has_tile     : 1;
  bool has_collider : 1;
};

struct node : no::move {
  jute::view atom {};
  const node * list {};
  const node * next {};
  const reader * r {};
  unsigned loc {};

  void * operator new(traits::size_t n);
};
struct tdef_node : node {
  prefabs::tiledef tdef {};
  hai::sptr<prefabs::tilemap> tmap {};
  bool has_entity   : 1;
  bool has_tile     : 1;
  bool has_collider : 1;
};

[[noreturn]] static void err(const node & n, jute::view msg) { n.r->err(msg, n.loc); }
[[noreturn]] static void err(const node * n, jute::view msg) { n->r->err(msg, n->loc); }

static node * next_list(reader & r) {
  auto * res = new node {
    .r = &r,
    .loc = r.loc(),
  };
  auto * n = &res->list;
  while (r) {
    auto token = next_token(r);
    if (token == ")") return res;
    if (token == "") break;

    auto nn = (token == "(") ?
      next_list(r) :
      new node {
        .atom = token,
        .r = &r,
        .loc = static_cast<unsigned>(r.loc() - token.size()),
      };
    *n = nn;
    n = &(nn->next);
  }
  err(res, "unbalanced open parenthesis");
}
static node * next_node(reader & r) {
  if (!r) return {};

  auto token = next_token(r);
  if (token == "(") {
    return next_list(r);
  } else if (token == ")") {
    r.err("unbalanced close parenthesis");
  } else {
    return new node { 
      .atom = token,
      .r = &r,
      .loc = static_cast<unsigned>(r.loc() - token.size()),
    };
  }
}

static bool is_atom(const node * n) { return n->atom.size(); }

static auto ls(const node * n) {
  unsigned sz = 0;
  for (auto nn = n->list; nn; nn = nn->next) sz++;
  return sz;
}

static float to_f(const node * n) {
  if (!is_atom(n)) err(n, "non-numerical coordinate");
  try {
    return jute::to_f(n->atom);
  } catch (...) {
    err(n, "invalid number");
  }
}
static int to_i(const node * n) {
  if (!is_atom(n)) err(n, "non-numerical coordinate");
  try {
    return jute::to_u32(n->atom);
  } catch (...) {
    err(n, "invalid number");
  }
}

struct context {
  hashley::fin<const node *> defs { 127 };
};
[[nodiscard]] static const node * eval(context & ctx, const node * n) {
  if (!n->list) return n;
  if (!is_atom(n->list)) err(*n->list, "expecting an atom");

  auto fn = n->list->atom;

  if (fn == "def") {
    if (ls(n) != 3) err(n, "def requires a name and a value");

    auto args = n->list->next;
    if (!is_atom(args)) err(*args, "def name must be an atom");
    ctx.defs[args->atom] = args->next;
    return args->next;
  }

  const node * aa[128] {};
  if (ls(n) >= 127) err(n, "too many parameters");
  auto ap = aa;
  for (auto nn = n->list->next; nn; nn = nn->next) *ap++ = eval(ctx, nn);
  
  if (fn == "tiledef") {
    if (ls(n) < 2) err(n, "tiledef must have at least name");

    auto * nn = new tdef_node { *n };
    for (auto * c = aa; *c; c++) {
      auto * cc = static_cast<const tdef_node *>(*c);
      bool valid = false;
      if (cc->tdef.behaviour.size()) {
        nn->tdef.behaviour = cc->tdef.behaviour;
        valid = true;
      }
      if (cc->tdef.loot.size()) {
        nn->tdef.loot = cc->tdef.loot;
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
      if (cc->has_collider) {
        nn->tdef.collision = cc->tdef.collision;
        nn->has_collider = true;
        valid = true;
      }
      if (!valid) err(*c, "invalid element in tiledef");
    }
    return nn;
  } else if (fn == "tile") {
    if (ls(n) != 6) err(n, "tile should have uv, size and texid");

    auto * nn = new tdef_node { *n };
    auto & t = nn->tdef.tile;
    t.uv.x   = to_i(aa[0]);
    t.uv.y   = to_i(aa[1]);
    t.size.x = to_i(aa[2]);
    t.size.y = to_i(aa[3]);
    t.texid  = to_i(aa[4]);
    nn->has_tile = true;
    return nn;
  } else if (fn == "entity") {
    if (ls(n) != 6) err(n, "entity should have uv, size and texid");

    auto * nn = new tdef_node { *n };
    auto & t = nn->tdef.entity;
    t.uv.x   = to_i(aa[0]);
    t.uv.y   = to_i(aa[1]);
    t.size.x = to_i(aa[2]);
    t.size.y = to_i(aa[3]);
    t.texid  = to_i(aa[4]);
    nn->has_entity = true;
    return nn;
  } else if (fn == "collision") {
    if (ls(n) != 5) err(n, "collision should have pos and size");

    auto * nn = new tdef_node { *n };
    auto & c = nn->tdef.collision;
    c.x = to_f(aa[0]);
    c.y = to_f(aa[1]);
    c.z = to_f(aa[2]);
    c.w = to_f(aa[3]);
    nn->has_collider = true;
    return nn;
  } else if (fn == "behaviour") {
    if (ls(n) != 2) err(n, "behaviour requires a value");
    if (!is_atom(aa[0])) err(n, "behaviour must be an atom");
    auto * nn = new tdef_node { *n };
    nn->tdef.behaviour = aa[0]->atom;
    return nn;
  } else if (fn == "loot") {
    if (ls(n) != 2) err(n, "loot table requires a value");
    if (!is_atom(aa[0])) err(n, "loot table must be an atom");
    auto * nn = new tdef_node { *n };
    nn->tdef.loot = aa[0]->atom;
    return nn;
  } else if (fn == "prefab") {
    if (ls(n) != prefabs::height + 1) err(n, "incorrect number of rows in prefab");

    auto * nn = new tdef_node { *n };
    nn->tmap = hai::sptr { new prefabs::tilemap {} };
    auto & map = *nn->tmap;
    for (auto i = 0; aa[i]; i++) {
      auto c = aa[i];
      if (!is_atom(c)) err(*c, "rows in prefabs must be atoms");
      if (c->atom.size() != prefabs::width) err(*c, "incorrect number of symbols in prefab");
      for (auto x = 0; x < c->atom.size(); x++) {
        auto id = c->atom.subview(x, 1).middle;
        if (!ctx.defs.has(id)) n->r->err("unknown tiledef", c->loc + x);
        auto cid = static_cast<const tdef_node *>(eval(ctx, ctx.defs[id]));
        // TODO: validate tiledef
        map(x, i) = cid->tdef;
      }
    }
    return nn;
  } else if (fn == "random") {
    if (ls(n) == 0) err(n, "rand requires at least a parameter");
    return aa[rng::rand(ls(n) - 1)];
  } else if (ctx.defs.has(fn)) {
    return eval(ctx, ctx.defs[fn]);
  } else {
    err(n, *("invalid function name: "_hs + fn));
  }
}

// TODO: eviction rules
hashley::fin<hai::sptr<prefabs::tilemap>> g_cache { 127 };

static tdef_node * g_instances {};
static tdef_node * g_cur_instance;
void * node::operator new(traits::size_t sz) {
  if (g_cur_instance == g_instances + 10240) throw 0;
  return g_cur_instance++;
}

const prefabs::tilemap * prefabs::parse(jute::view filename) {
  if (g_cache.has(filename)) return &*g_cache[filename];

  auto code = jojo::read_cstr(filename);

  g_instances = new tdef_node[10240] {};
  g_cur_instance = g_instances;
  context ctx {};
  reader r { code };
  const tdef_node * prefab {};

  while (r) {
    auto n = static_cast<const tdef_node *>(eval(ctx, next_node(r)));
    if (n->tmap) prefab = n;
  }
  if (!prefab) return nullptr;

  g_cache[filename] = prefab->tmap;
  return &*g_cache[filename];
}
const prefabs::tilemap * prefabs::load(jute::view filename) {
  try {
    silog::log(silog::debug, "before");
    auto prefab = parse(sires::real_path_name(filename)); 
    if (!prefab) silog::die("missing prefab definition");
    silog::log(silog::debug, "used %d lisp node instances", static_cast<unsigned>(g_cur_instance - g_instances));
    return prefab;
  } catch (const prefabs::parser_error & e) {
    silog::log(silog::error, "%s:%d:%d: %s", filename.cstr().begin(), e.line, e.col, e.msg.begin());
    throw;
  }
}
