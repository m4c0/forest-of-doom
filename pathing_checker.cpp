#pragma leco test
import jojo;
import jute;
import lispy;
import print;

int main() try {
  struct node : lispy::node {
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
    return n;
  };
  
  lispy::run(jojo::read_cstr("pathing.lsp"), cm.ctx, [](auto * n) {
  });
} catch (const lispy::parser_error & e) {
  errln("pathing.lsp", ":", e.line, ":", e.col, ": ", e.msg);
  return 1;
}
