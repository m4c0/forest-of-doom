#pragma leco test
import hai;
import hashley;
import jojo;
import jute;
import lispy;
import print;

int main() try {
  struct exit {
    jute::view key;
    jute::view value;
  };
  struct from {
    jute::view file;
    jute::view entry;
    jute::view start;
    hashley::fin<jute::view> exits { 13 };
  };
  enum ntype {
    n_empty, n_file, n_entry, n_exit, n_from, n_start
  };
  struct node : lispy::node {
    ntype type = n_empty;
    union u {
      jute::view str;
      exit exit;
      hai::sptr<from> from;
      ~u() {}
    } u;
  };

  lispy::ctx_w_mem<node> cm {};
  cm.ctx.fns["entry"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) lispy::err(n, "entry expects entry name");
    auto a = static_cast<const node *>(lispy::eval(ctx, aa[0]));
    if (!lispy::is_atom(a)) lispy::err(aa[0], "expecting atom as entry name");
    return new (ctx.allocator()) node { *aa[0], n_entry, { a->atom } };
  };
  cm.ctx.fns["file"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) lispy::err(n, "file expects prefab name");
    auto a = static_cast<const node *>(lispy::eval(ctx, aa[0]));
    if (!lispy::is_atom(a)) lispy::err(aa[0], "expecting atom as file name");
    return new (ctx.allocator()) node { *aa[0], n_file, { a->atom } };
  };
  cm.ctx.fns["exit"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 2) lispy::err(n, "exit expects name and target def");
    auto a0 = static_cast<const node *>(lispy::eval(ctx, aa[0]));
    if (!lispy::is_atom(a0)) lispy::err(aa[0], "expecting atom as point name");
    auto a1 = static_cast<const node *>(lispy::eval(ctx, aa[1]));
    if (!lispy::is_atom(a1)) lispy::err(aa[1], "expecting atom as def name");
    return new (ctx.allocator()) node { *n, n_exit, { .exit { a0->atom, a1->atom } } };
  };

  cm.ctx.fns["from"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as < 1) lispy::err(n, "from expects file, entry and one or more exit");
    auto f = hai::sptr<from>::make();
    for (auto i = 0; i < as; i++) {
      auto a = static_cast<const node *>(lispy::eval(ctx, aa[i]));
      switch (a->type) {
        case n_empty: break;
        case n_file: f->file = a->u.str; break;
        case n_entry: f->entry = a->u.str; break;
        case n_start: lispy::err(aa[i], "cannot have a start inside a from"); break;
        case n_exit: f->exits[a->u.exit.key] = a->u.exit.value; break;
        case n_from: lispy::err(aa[i], "no copy support yet"); break;
      }
    }
    return new (ctx.allocator()) node { {}, n_from, { .from { f } }};
  };
  cm.ctx.fns["start"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) lispy::err(n, "start expects the target");
    auto a = lispy::eval(ctx, aa[0]);
    if (!lispy::is_atom(a)) lispy::err(a, "start expects the def name");
    if (!ctx.defs.has(a->atom)) lispy::err(a, "missing def");
    return new (ctx.allocator()) node { *a, n_start, { a->atom } };
  };
  
  jute::heap start {};
  lispy::run(jojo::read_cstr("pathing.lsp"), cm.ctx, [&](auto * n) {
    auto * nn = static_cast<const node *>(n);
    if (nn->type == n_start) {
      start = nn->u.str;

      auto * tn = cm.ctx.defs[*start];
      if (!tn) lispy::err(nn, "undefined start");

      // Only used to validate the def
      [[maybe_unused]]
      auto * tt = static_cast<const node *>(lispy::eval(cm.ctx, tn));
    }
  });
  if (*start == "") errln("pathing.lsp:1:1: missing start");

} catch (const lispy::parser_error & e) {
  errln("pathing.lsp", ":", e.line, ":", e.col, ": ", e.msg);
  return 1;
}
