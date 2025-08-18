export module backpack;
import dotz;
import drops;
import fox;
import hai;
import jute;
import loots;
import silog;

namespace backpack {
  struct t {
    hai::array<loots::item> inv {};
    jute::heap loot;
    fox::sprite sprite;
  };

  hai::varray<t> list { 128 };

  export void add(t t) {
    list.push_back_doubling(t);
  }

  export void purge() {
    list.truncate(0);
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
  
  export hai::array<loots::item> * open(dotz::vec2 player) {
    for (auto i = 0; i < list.size(); i++) {
      auto & pack = list[i];

      auto c = pack.sprite.pos + pack.sprite.size / 2.0;
      if (dotz::length(player - c) > 0.5) continue;

      if (pack.inv.size() == 0) pack.inv = loots::load(*pack.loot);
      return &pack.inv;
    }
    return nullptr;
  }
}
