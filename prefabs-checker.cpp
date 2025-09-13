#pragma leco test
import jute;
import lispy;
import pprent;
import prefabs;
import print;

static void ignore(prefabs::tilemap) {}

// TODO: validate entry/exit
int main() {
  int result = 0;
  for (auto p : pprent::list("prefabs")) {
    auto fn = jute::heap{"prefabs/"} + jute::view::unsafe(p);
    if (!(*fn).ends_with(".lsp")) continue;
    prefabs::load(*fn, ignore);
    //try {
    //  if (!prefab) {
    //    errln(fn, ": missing prefab definition");
    //    result = 1;
    //  }
    //} catch (const lispy::parser_error & e) {
    //  errln(fn, ":", e.line, ":", e.col, ": ", e.msg);
    //  result = 1;
    //}
  }
  return result;
}
