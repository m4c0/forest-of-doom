export module exit;
import dotz;
import fox;
import hai;
import jute;
import loots;
import silog;

namespace exits {
  struct t {
    dotz::vec2 pos;
    jute::heap file;
    jute::heap entry;
  };

  hai::varray<t> list { 128 };

  export void add(t t) {
    list.push_back_doubling(t);
  }

  export void purge() {
    list.truncate(0);
  }

  export t * open(dotz::vec2 player) {
    for (auto i = 0; i < list.size(); i++) {
      auto & e = list[i];

      auto c = e.pos + 0.5;
      if (dotz::length(player - c) > 0.5) continue;

      return &e;
    }
    return nullptr;
  }
}
