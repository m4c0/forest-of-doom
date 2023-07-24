export module collision;
import pog;

namespace collision {
export struct compos : virtual pog::entity_provider {
  pog::rtree bodies{};
  pog::map collisions{};
};

export void add(compos *c, pog::eid id, float x, float y, float w, float h) {
  pog::aabb area{{x, y}, {x + w, y + h}};
  c->bodies.add(id, area);
}
export void remove(compos *c, pog::eid id) { c->bodies.remove(id); }

export bool move_by(compos *c, pog::eid id, float dx, float dy) {
  auto aabb = c->bodies.get(id);
  auto old = aabb;
  aabb.a.x += dx;
  aabb.a.y += dy;
  aabb.b.x += dx;
  aabb.b.y += dy;

  pog::eid hit{};
  c->bodies.for_each_in(aabb, [&](pog::eid oid, auto) {
    if (id != oid)
      hit = oid;
  });
  if (hit) {
    c->collisions.set(id, hit);
    return false;
  }

  c->bodies.remove(id);
  c->bodies.add(id, aabb);
  return true;
}
} // namespace collision
