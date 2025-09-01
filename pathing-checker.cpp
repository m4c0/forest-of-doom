#pragma leco test
import pathing;

import hai;
import hashley;
import jojo;
import jute;
import lispy;
import print;

int main() try {
  pathing::load();
} catch (const lispy::parser_error & e) {
  errln("pathing.lsp:", e.line, ":", e.col, ": ", e.msg);
  return 1;
}
