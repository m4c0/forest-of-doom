#pragma leco test
import jojo;
import jute;
import lispy;
import print;

int main() try {
  struct exit {
    jute::heap entry;
    jute::heap target;
  };
  struct node : lispy::node {
    jute::heap file;
    jute::heap entry;
    jute::heap start;
    exit exit;
  };

  lispy::ctx_w_mem<node> cm {};
  cm.ctx.fns["entry"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) lispy::err(n, "entry expects entry name");
    auto a = static_cast<const node *>(lispy::eval(ctx, aa[0]));
    if (!lispy::is_atom(a)) lispy::err(aa[0], "expecting atom as entry name");

    auto nn = new (ctx.allocator()) node {};
    nn->entry = a->atom;
    return nn;
  };
  cm.ctx.fns["file"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) lispy::err(n, "file expects prefab name");
    auto a = static_cast<const node *>(lispy::eval(ctx, aa[0]));
    if (!lispy::is_atom(a)) lispy::err(aa[0], "expecting atom as file name");

    auto nn = new (ctx.allocator()) node {};
    nn->file = a->atom;
    return nn;
  };
  cm.ctx.fns["exit"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 2) lispy::err(n, "exit expects name and target def");
    auto a0 = static_cast<const node *>(lispy::eval(ctx, aa[0]));
    if (!lispy::is_atom(a0)) lispy::err(aa[0], "expecting atom as point name");
    auto a1 = static_cast<const node *>(lispy::eval(ctx, aa[1]));
    if (!lispy::is_atom(a1)) lispy::err(aa[1], "expecting atom as def name");

    auto nn = new (ctx.allocator()) node {};
    nn->exit = exit { a0->atom, a1->atom };
    return n;
  };

  cm.ctx.fns["from"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as < 1) lispy::err(n, "from expects file, entry and one or more exit");
    auto nn = new (ctx.allocator()) node {};
    for (auto i = 0; i < as; i++) {
      auto a = static_cast<const node *>(lispy::eval(ctx, aa[i]));
      if (*a->file != "") nn->file = a->file;
      if (*a->entry != "") nn->entry = a->entry;
    }
    return nn;
  };
  cm.ctx.fns["start"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) lispy::err(n, "start expects the target");
    auto a = lispy::eval(ctx, aa[0]);
    if (!lispy::is_atom(a)) lispy::err(aa[0], "start expects the def name");

    auto nn = new (ctx.allocator()) node { *n };
    nn->start = a->atom;
    return nn;
  };
  
  jute::heap start {};
  lispy::run(jojo::read_cstr("pathing.lsp"), cm.ctx, [&](auto * n) {
    auto * nn = static_cast<const node *>(n);
    if (*nn->start != "") {
      start = nn->start;

      auto * tn = cm.ctx.defs[*nn->start];
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
