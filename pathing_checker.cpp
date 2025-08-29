#pragma leco test
import jojo;
import jute;
import lispy;
import print;

int main() try {
  struct node : lispy::node {
    jute::heap start;
  };

  lispy::ctx_w_mem<node> cm {};
  cm.ctx.fns["from"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as < 1) lispy::err(n, "from expects file, enter and one or more exit");

    auto nn = new (ctx.allocator()) node {
    };
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
    if (*nn->start != "") start = nn->start;
  });
  if (*start == "") errln("pathing.lsp:1:1: missing start");
} catch (const lispy::parser_error & e) {
  errln("pathing.lsp", ":", e.line, ":", e.col, ": ", e.msg);
  return 1;
}
