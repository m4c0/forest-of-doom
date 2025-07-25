module prefabs;
import jojo;
import jute;
import hai;
import hashley;
import no;
import print;

using namespace jute::literals;

struct error {
  jute::heap msg;
  unsigned line;
  unsigned col;
};

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
    throw error { msg, l, loc - last };
  }
  [[noreturn]] void err(jute::view msg) const {
    err(msg, m_pos);
  }

  jute::view token(const char * start) const {
    return { start, static_cast<unsigned>(mark() - start) };
  }
};

static bool is_atom_char(char c) {
  auto upc = c | 0x20;
  return (upc >= 'a' && upc <= 'z') || (c == '_') || (c >= '0' && c <= '9');
}
static jute::view next_atom_token(const char * start, reader & r) {
  while (is_atom_char(r.peek())) r.take();
  return r.token(start);
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
  hai::varray<node> list {};
  prefabs::tiledef tdef {};
  hai::uptr<prefabs::tilemap> tmap {};
  const reader * r {};
  unsigned loc {};
  bool has_tile     : 1;
  bool has_collider : 1;
};

static node next_list(reader & r) {
  node res { .list { 16 }, .r = &r, .loc = r.loc() };
  while (r) {
    auto token = next_token(r);
    if (token == ")") return res;

    if (token == "(") {
      res.list.push_back_doubling(next_list(r));
    } else if (token == "") {
      break;
    } else {
      res.list.push_back_doubling(node { .atom = token, .r = &r, .loc = static_cast<unsigned>(r.loc() - token.size()) });
    }
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
    return { .atom = token, .r = &r, .loc = static_cast<unsigned>(r.loc() - token.size()) };
  }
}

static bool is_atom(const node & n) { return n.list.size() == 0; }

static int to_f(const node & n) {
  if (!is_atom(n)) n.r->err("non-numerical coordinate", n.loc);
  return jute::to_f(n.atom);
}
static int to_i(const node & n) {
  if (!is_atom(n)) n.r->err("non-numerical coordinate", n.loc);
  return jute::to_u32(n.atom);
}
static int texid(const node & n) {
  if (n.atom == "one_terrains_and_fences") return 0;
  n.r->err("invalid texture", n.loc);
}

struct context {
  hashley::fin<prefabs::tiledef> tiledefs { 127 };
};
static void eval(context & ctx, node & n) {
  if (is_atom(n)) return;

  for (auto & child : n.list) eval(ctx, child);
  
  auto & fn = n.list[0];
  if (!is_atom(fn)) n.r->err("trying to eval a list as a function", n.loc);

  if (fn.atom == "tiledef") {
    if (n.list.size() < 2) n.r->err("tiledef must have at least name", n.loc);

    for (auto i = 1; i < n.list.size(); i++) {
      auto & c = n.list[i];
      bool valid = false;
      if (c.tdef.id.size()) {
        n.tdef.id = c.tdef.id;
        valid = true;
      }
      if (c.has_tile) {
        n.tdef.tile = c.tdef.tile;
        n.has_tile = true;
        valid = true;
      }
      if (c.has_collider) {
        n.tdef.collision = c.tdef.collision;
        n.has_collider = true;
        valid = true;
      }
      if (!valid) n.r->err("invalid element in tiledef", c.loc);
    }

    if (n.tdef.id.size()) {
      ctx.tiledefs[*n.tdef.id] = n.tdef;
    }
  } else if (fn.atom == "tile") {
    if (n.list.size() != 6) n.r->err("tile should have uv, size and texid", n.loc);

    auto & t = n.tdef.tile;
    t.uv.x = to_i(n.list[1]);
    t.uv.y = to_i(n.list[2]);
    t.size.x = to_i(n.list[3]);
    t.size.y = to_i(n.list[4]);
    t.texid = texid(n.list[5]);
    n.has_tile = true;
  } else if (fn.atom == "collision") {
    if (n.list.size() != 5) n.r->err("collision should have pos and size", n.loc);

    auto & c = n.tdef.collision;
    c.x = to_f(n.list[1]);
    c.y = to_f(n.list[2]);
    c.z = to_f(n.list[3]);
    c.w = to_f(n.list[4]);
    n.has_collider = true;
  } else if (fn.atom == "id") {
    if (n.list.size() != 2) n.r->err("id requires a value", n.loc);
    if (!is_atom(n.list[1])) n.r->err("id must be an atom", n.loc);
    n.tdef.id = n.list[1].atom;
  } else if (fn.atom == "prefab") {
    if (n.list.size() != prefabs::height + 1) n.r->err("incorrect number of rows in prefab", n.loc);

    n.tmap.reset(new prefabs::tilemap {});
    auto & map = *n.tmap;
    for (auto i = 1; i < n.list.size(); i++) {
      auto & c = n.list[i];
      if (!is_atom(c)) n.r->err("rows in prefabs must be atoms", c.loc);
      if (c.atom.size() != prefabs::width) n.r->err("incorrect number of symbols in prefab", c.loc);
      for (auto x = 0; x < c.atom.size(); x++) {
        auto id = c.atom.subview(x, 1).middle;
        if (!ctx.tiledefs.has(id)) n.r->err("unknown tiledef", c.loc + x);
        map(i - 1, x) = ctx.tiledefs[id];
      }
    }
  } else {
    n.r->err("invalid function name", n.loc);
  }
}

// TODO: eviction rules
hashley::fin<prefabs::tilemap> g_cache { 127 };

const prefabs::tilemap * prefabs::load(jute::view filename) {
  if (g_cache.has(filename)) return &g_cache[filename];

  auto code = jojo::read_cstr(filename);

  context ctx {};
  reader r { code };
  hai::uptr<prefabs::tilemap> prefab {};

  try {
    while (r) {
      auto n = next_node(r);
      eval(ctx, n);
      if (n.tmap) prefab = traits::move(n.tmap);
    }
  } catch (const error & e) {
    die(filename, ":", e.line, ":", e.col, ": ", e.msg);
  }

  if (!prefab) die("missing prefab definition");

  g_cache[filename] = traits::move(*prefab.release());
  return &g_cache[filename];
}
