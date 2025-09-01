#pragma leco test
import lispy;
import pathing;
import print;

int main() try {
  auto ignore = pathing::load();
} catch (const lispy::parser_error & e) {
  errln("pathing.lsp:", e.line, ":", e.col, ": ", e.msg);
  return 1;
}
