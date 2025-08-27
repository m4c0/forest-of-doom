#pragma leco test
import jute;
import lispy;
import print;

int main() try {
  struct node : lispy::node {
    jute::heap name;
  };

  lispy::ctx_w_mem<node> cm {};
  cm.ctx.fns["from"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as < 1) lispy::err(n, "from expects at least a name");

    if (!lispy::is_atom(aa[0])) lispy::err(aa[0], "name should be an atom");

    auto nn = new (ctx.allocator()) node {
      .name = aa[0]->atom,
    };
    return nn;
  };
  cm.ctx.fns["start"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) lispy::err(n, "start expects the target");
    return n;
  };
  
  lispy::run("pathing.lsp", cm.ctx, [](auto * n) {
  });
} catch (const lispy::parser_error & e) {
  errln("pathing.lsp", ":", e.line, ":", e.col, ": ", e.msg);
  return 1;
}
