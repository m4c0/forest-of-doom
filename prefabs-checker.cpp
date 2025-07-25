#pragma leco tool
import jute;
import pprent;
import prefabs;
import print;

int main() {
  for (auto p : pprent::list("prefabs")) {
    auto fn = jute::heap{"prefabs/"} + jute::view::unsafe(p);
    if (!(*fn).ends_with(".lsp")) continue;
    try {
      auto prefab = prefabs::parse(*fn);
      if (!prefab) errln(fn, ": missing prefab definition");
    } catch (const prefabs::parser_error & e) {
      errln(fn, ":", e.line, ":", e.col, ": ", e.msg);
    }
  }
}
