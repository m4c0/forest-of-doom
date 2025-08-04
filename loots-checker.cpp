#pragma leco test
import jute;
import pprent;
import loots;
import print;

int main() {
  int result = 0;
  for (auto p : pprent::list("loots")) {
    auto fn = jute::heap{"loots/"} + jute::view::unsafe(p);
    if (!(*fn).ends_with(".lsp")) continue;
    try {
      auto inv = loots::load(*fn);
      if (inv.size() == 0) {
        errln(fn, ": missing items");
        result = 1;
      }
    } catch (...) {
      errln(fn, ": failed to check");
      result = 1;
    }
  }
  return result;
}
