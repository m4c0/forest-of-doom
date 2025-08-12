export module drops;
import dotz;
import fox;
import hai;

// TODO: move items to avoid collisions
//       This should move away from blocks etc

namespace drops {
  struct t {
    fox::sprite sprite;
  };
  hai::varray<t> list { 128 };

  export void add(t t) {
    list.push_back_doubling(t);
  }

  export void load(dotz::vec2 player, fox::memiter * m) {
    // TODO: use tiledef collision
    for (auto & item : list) {
      auto i = item.sprite;
      auto c = i.pos + i.size / 2.0;
      if (dotz::length(player - c) < 0.5) i.alpha = 0.6;
      *m += i;
    }
  }
}
