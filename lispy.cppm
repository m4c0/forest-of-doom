#pragma leco tool
export module lispy;
import jojo;
import jute;
import hai;
import no;
import prefabs;
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

  const char * mark() const { return m_data.begin() + m_pos; }

  char peek() {
    if (m_pos >= m_data.size()) return 0;
    return m_data[m_pos];
  }
  char take() {
    if (m_pos >= m_data.size()) return 0;
    return m_data[m_pos++];
  }

  [[noreturn]] void err(jute::view msg) {
    unsigned l = 1;
    unsigned last = 0;
    for (auto i = 0; i < m_pos; i++) {
      if (m_data[i] == '\n') {
        last = i;
        l++;
      }
    }
    throw error { msg, l, m_pos - last };
  }

  jute::view token(const char * start) {
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
};

static node next_list(reader & r) {
  node res { .list { 16 } };
  while (r) {
    auto token = next_token(r);
    if (token == ")") return res;

    if (token == "(") {
      res.list.push_back_doubling(next_list(r));
    } else if (token == "") {
      break;
    } else {
      res.list.push_back_doubling(node { token });
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
    return { token };
  }
}

static bool is_atom(const node & n) { return n.list.size() == 0; }

static int to_f(const node & n) {
  if (!is_atom(n)) throw error { "non-numerical coordinate"_hs };
  return jute::to_f(n.atom);
}
static int to_i(const node & n) {
  if (!is_atom(n)) throw error { "non-numerical coordinate"_hs };
  return jute::to_u32(n.atom);
}
static int texid(const node & n) {
  if (n.atom == "one_terrains_and_fences") return 0;
  throw error { "invalid texture"_hs };
}

static void eval(node & n) {
  if (is_atom(n)) return;

  for (auto & child : n.list) eval(child);
  
  auto & fn = n.list[0];
  if (!is_atom(fn)) throw error { "trying to eval a list as a function"_hs };

  if (fn.atom == "tiledef") {
  } else if (fn.atom == "tile") {
    if (n.list.size() != 6) throw error { "tile should have uv, size and texid"_hs };

    auto & t = n.tdef.tile;
    t.uv.x = to_i(n.list[1]);
    t.uv.y = to_i(n.list[2]);
    t.size.x = to_i(n.list[3]);
    t.size.y = to_i(n.list[4]);
    t.texid = texid(n.list[5]);
  } else if (fn.atom == "collision") {
    if (n.list.size() != 5) throw error { "collision should have pos and size"_hs };

    auto & c = n.tdef.collision;
    c.x = to_f(n.list[1]);
    c.y = to_f(n.list[2]);
    c.z = to_f(n.list[3]);
    c.w = to_f(n.list[4]);
  } else if (fn.atom == "prefab") {
  } else {
    throw error { "invalid function name: "_hs + fn.atom };
  }
}

int main() try {
  jute::view file = "prefabs/prefabs-ocean-0.lsp";
  auto code = jojo::read_cstr(file);

  reader r { code };
  try {
    while (r) {
      auto n = next_node(r);
      eval(n);
    }
  } catch (const error & e) {
    die(file, ":", e.line, ":", e.col, ": ", e.msg);
  }
} catch (...) {
  return 1;
}

