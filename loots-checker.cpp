#pragma leco test
import jute;
import lispy;
import pprent;
import loots;
import print;

int main() {
  int result = 0;
  for (auto p : pprent::list("loots")) {
    jute::heap fn = "loots/" + jute::view::unsafe(p);
    if (!(*fn).ends_with(".lsp")) continue;
    try {
      auto inv = loots::parse(*fn);
      if (inv.size() == 0) {
        errln(fn, ": missing items");
        result = 1;
      }
    } catch (const lispy::parser_error & e) {
      errln(fn, ":", e.line, ":", e.col, ": ", e.msg);
      result = 1;
    }
  }
  return result;
}
