#pragma leco test
import hai;
import jojo;
import jute;
import lispy;
import pprent;
import prefabs;
import print;

// TODO: validate entry/exit
int main() {
  int result = 0;

  jojo::on_error([&](void *, jute::view msg) {
    errln(msg);
    result = 1;
  });

  for (auto p : pprent::list("prefabs")) {
    jute::heap fn = jute::heap{"prefabs/"} + jute::view::unsafe(p);
    if (!(*fn).ends_with(".lsp")) continue;
    jojo::read(*fn, nullptr, [&](void *, const hai::cstr & lsp) {
      try {
        if (!prefabs::parse(lsp)) {
          errln(fn, ": missing prefab definition");
          result = 1;
        }
      } catch (const lispy::parser_error & e) {
        errln(fn, ":", e.line, ":", e.col, ": ", e.msg);
        result = 1;
      }
    });
  }
  return result;
}
