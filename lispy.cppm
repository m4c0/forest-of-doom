#pragma leco tool
export module lispy;
import jojo;
import jute;
import hai;
import no;
import print;

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

static void dump(const node & n) {
  if (n.list.size()) {
    put("(");
    for (auto & nn : n.list) dump(nn);
    put(")");
  } else {
    put(n.atom, " ");
  }
}

int main() try {
  jute::view file = "prefabs/prefabs-ocean-0.lsp";
  auto code = jojo::read_cstr(file);

  reader r { code };
  try {
    while (r) {
      auto n = next_node(r);
      dump(n);
    }
  } catch (const error & e) {
    die(file, ":", e.line, ":", e.col, ": ", e.msg);
  }
} catch (...) {
  return 1;
}

