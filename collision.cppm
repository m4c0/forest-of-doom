export module collision;
import pog;

namespace collision {
export struct compos {
  virtual pog::entity_list &e() noexcept = 0;
  virtual pog::btree<pog::aabb> &body_index() noexcept = 0;
  virtual pog::rtree &bodies() noexcept = 0;
};

export void add(compos *c, pog::eid id, float x, float y, float w, float h) {
  pog::aabb area{{x, y}, {x + w, y + h}};
  c->body_index().add(id, area);
  c->bodies().add(id, area);
}

export bool move_by(compos *c, pog::eid id, float dx, float dy) {
  auto aabb = c->body_index().get(id);
  auto old = aabb;
  aabb.a.x += dx;
  aabb.a.y += dy;
  aabb.b.x += dx;
  aabb.b.y += dy;
  c->bodies().remove(id, aabb);
  if (c->bodies().has_in(aabb)) {
    c->bodies().add(id, old);
    return false;
  }
  c->body_index().update(id, aabb);
  c->bodies().add(id, aabb);
  return true;
}
} // namespace collision
