export module collision;
import pog;

namespace collision {
export using compo = pog::quadtree;
export struct compos {
  virtual pog::entity_list &e() noexcept = 0;
  virtual compo &bodies() noexcept = 0;
};

export void add(compos *c, pog::eid id, float x, float y, float w, float h) {
  c->bodies().add(id, {{x, y}, {x + w, y + h}});
}

export bool move_by(compos *c, pog::eid id, float dx, float dy) {
  auto aabb = c->bodies().get(id);
  auto old = aabb;
  aabb.aa.x += dx;
  aabb.aa.y += dy;
  aabb.bb.x += dx;
  aabb.bb.y += dy;
  c->bodies().remove(id);
  if (c->bodies().has_in(aabb)) {
    c->bodies().add(id, old);
    return false;
  }
  c->bodies().add(id, aabb);
  return true;
}
} // namespace collision
