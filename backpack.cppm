export module backpack;
import dotz;
import fox;
import hai;

namespace backpack {
  hai::varray<fox::sprite> list { 128 };

  export void add(fox::sprite t) {
    list.push_back_doubling(t);
  }

  export void load(dotz::vec2 player, fox::memiter * m) {
    // TODO: use tiledef collision
    for (auto i : list) {
      auto c = i.pos + i.size / 2.0;
      if (dotz::length(player - c) < 0.5) i.alpha = 0.6;
      *m += i;
    }
  }
  
  export bool open(dotz::vec2 player) {
    for (auto i : list) {
      auto c = i.pos + i.size / 2.0;
      if (dotz::length(player - c) < 0.5) return true;
    }
    return false;
  }
}
