export module backpack;
import dotz;
import fox;
import hai;
import jute;
import silog;

namespace backpack {
  struct t {
    jute::heap loot;
    fox::sprite sprite;
  };

  hai::varray<t> list { 128 };

  export void add(t t) {
    list.push_back_doubling(t);
  }

  export void load(dotz::vec2 player, fox::memiter * m) {
    // TODO: use tiledef collision
    for (auto [loot, i] : list) {
      auto c = i.pos + i.size / 2.0;
      if (dotz::length(player - c) < 0.5) i.alpha = 0.6;
      *m += i;
    }
  }
  
  export int open(dotz::vec2 player) {
    for (auto i = 0; i < list.size(); i++) {
      auto c = list[i].sprite.pos + list[i].sprite.size / 2.0;
      if (dotz::length(player - c) < 0.5) return i;
    }
    return -1;
  }

  export hai::array<dotz::vec2> inventory(unsigned id) {
    if (id >= list.size()) silog::die("trying to use invalid backpack %d", id);

    hai::array<dotz::vec2> res { 8 };
    for (auto & n: res) n = {};
    res[0] = { 8, 0 };
    if (id > 0) res[1] = { 11, 3 };
    if (id > 1) res[2] = { 12, 3 };
    res[3] = { 12, 2 };

    return res;
  }
}
