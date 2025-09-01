#pragma leco add_resource "pathing.lsp"
export module pathing;
import jute;
import hai;
import hashley;
import jojo;
import lispy;

using namespace jute::literals;

namespace pathing {
  struct exit {
    jute::view key;
    jute::view value;
  };
  struct from {
    jute::view file;
    jute::view entry;
    hashley::fin<jute::view> exits { 13 };
    hai::varray<jute::view> exit_names { 16 };
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

  export void load();
}

void pathing::load() {
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
        case n_exit: {
          f->exits[a->u.exit.key] = a->u.exit.value;
          f->exit_names.push_back_doubling(a->u.exit.key);
          break;
        }
        case n_from: lispy::err(aa[i], "no copy support yet"); break;
      }
    }
    return new (ctx.allocator()) node { *n, n_from, { .from { f } }};
  };
  cm.ctx.fns["start"] = [](auto ctx, auto n, auto aa, auto as) -> const lispy::node * {
    if (as != 1) lispy::err(n, "start expects the target");
    auto a = lispy::eval(ctx, aa[0]);
    if (!lispy::is_atom(a)) lispy::err(a, "start expects the def name");
    if (!ctx.defs.has(a->atom)) lispy::err(a, "missing def");
    return new (ctx.allocator()) node { *a, n_start, { a->atom } };
  };
  
  auto src = jojo::read_cstr("pathing.lsp");
  hashley::fin<hai::sptr<from>> froms { 127 };
  jute::view start {};
  lispy::run(src, cm.ctx, [&](auto * n) {
    auto * nn = static_cast<const node *>(n);
    if (nn->type == n_start) {
      start = nn->u.str;

      const auto rec = [&](auto & rec, auto * nn, jute::view key) -> void {
        auto * tn = cm.ctx.defs[key];
        if (!tn) lispy::err(nn, "undefined key");

        auto * tt = static_cast<const node *>(lispy::eval(cm.ctx, tn));
        if (tt->type != n_from) lispy::err(nn, "expecting a key to a 'from'");

        froms[key] = tt->u.from;

        for (auto key : tt->u.from->exit_names) {
          auto val = tt->u.from->exits[key];
          if (froms.has(val)) continue;
          rec(rec, tt, val);
        }
      };
      rec(rec, nn, start);
    }
  });
  if (start == "") lispy::fail({ "missing start"_hs, 1, 1 });
}
