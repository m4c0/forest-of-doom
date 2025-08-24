export module exit;
import collision;
import dotz;
import fox;
import hai;
import jute;
import loots;
import player;
import silog;

namespace exits {
  struct t {
    dotz::vec2 pos;
    jute::heap file;
    jute::heap entry;
  };

  hai::varray<t> list { 128 };
  collision::layer collisions {};

  export void add(t t) {
    list.push_back_doubling(t);
    // add margin to avoid triggering from neighbours
    collisions.add_aabb(t.pos + 0.25f, t.pos + 0.75f, 'exit', list.size());
  }

  export void purge() {
    list.truncate(0);
    collisions.purge();
  }

  export t * open() {
    auto [aa, bb] = player::aabb();
    aa = aa + 0.25;
    bb = bb + 0.25;

    t * result = nullptr;
    collisions.collides_aabb(aa, bb, [&](auto, auto id) {
      // TODO: maybe select largest area if colliding with more than one
      result = &list[id - 1];
      return true;
    });
    return result;
  }
}
