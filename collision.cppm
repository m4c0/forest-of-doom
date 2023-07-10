export module collision;
import pog;

namespace collision {
export using compo = pog::rtree;
export struct compos {
  virtual pog::entity_list &e() noexcept = 0;
  virtual compo &bodies() noexcept = 0;
};

export void add(compos *c, pog::eid id, float x, float y, float w, float h) {
  pog::aabb area{{x, y}, {x + w, y + h}};
  c->bodies().add(id, area);
}
export void remove(compos *c, pog::eid id) { c->bodies().remove(id); }

export bool move_by(compos *c, pog::eid id, float dx, float dy) {
  auto aabb = c->bodies().get(id);
  auto old = aabb;
  aabb.a.x += dx;
  aabb.a.y += dy;
  aabb.b.x += dx;
  aabb.b.y += dy;

  auto collides = false;
  c->bodies().for_each_in(aabb,
                          [&](pog::eid oid, auto) { collides |= (id != oid); });
  if (collides)
    return false;

  c->bodies().remove(id);
  c->bodies().add(id, aabb);
  return true;
}
} // namespace collision
