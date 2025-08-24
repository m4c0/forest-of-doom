export module exit;
import collision;
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
  collision::layer collisions {};

  export void add(t t) {
    list.push_back_doubling(t);
    collisions.add_aabb(t.pos, t.pos + 1, 'exit', list.size());
  }

  export void purge() {
    list.truncate(0);
    collisions.purge();
  }

  export t * open(dotz::vec2 aa, dotz::vec2 bb) {
    t * result = nullptr;
    collisions.collides_aabb(aa, bb, [&](auto, auto id) {
      // TODO: maybe select largest area if colliding with more than one
      result = &list[id - 1];
      return true;
    });
    return result;
  }
}
