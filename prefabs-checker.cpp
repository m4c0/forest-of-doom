#pragma leco test
import jute;
import pprent;
import prefabs;
import print;

int main() {
  int result = 0;
  for (auto p : pprent::list("prefabs")) {
    auto fn = jute::heap{"prefabs/"} + jute::view::unsafe(p);
    if (!(*fn).ends_with(".lsp")) continue;
    try {
      auto prefab = prefabs::parse(*fn);
      if (!prefab) errln(fn, ": missing prefab definition");
    } catch (const prefabs::parser_error & e) {
      errln(fn, ":", e.line, ":", e.col, ": ", e.msg);
      result = 1;
    }
  }
  return result;
}
