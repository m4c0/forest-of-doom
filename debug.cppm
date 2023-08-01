export module debug;
import collision;
import pog;
import rect;
import sprite;

export namespace debug {
struct compos : virtual collision::compos, virtual sprite::compos {
  pog::sparse_set<pog::marker> debug_marker{};
};

void show_collisions_around(compos *ec, float cx, float cy, float r) {
  const pog::aabb area{{cx - r, cy - r}, {cx + r, cy + r}};
  ec->debug_marker.remove_if([&](auto eid, auto) {
    sprite::remove(ec, eid);
    return true;
  });
  ec->bodies.for_each_in(area, [&](auto eid, auto area) {
    sprite::c s{
        .layer = sprite::layers::debug,
        .dim = 1,
    };
    if (ec->collisions.has(eid)) {
      s.colour = {1, 0, 0, 0.5};
    } else {
      s.colour = {1, 1, 1, 0.5};
    }
    auto nid = sprite::add(ec, s, rect_of(area));
    ec->debug_marker.add(nid, {});
  });
}
} // namespace debug
