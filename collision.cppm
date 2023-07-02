export module collision;
import pog;

namespace collision {
export using compo = pog::quadtree;
export struct compos {
  virtual pog::entity_list &e() noexcept = 0;
  virtual compo &bodies() noexcept = 0;
};

export void add_to_entity(compos *c, pog::eid id, float x, float y) {
  c->bodies().add(id, {{x, y}, {x + 1, y + 1}});
}
export auto add_entity(compos *c, float x, float y) {
  auto e = c->e().alloc();
  add_to_entity(c, e, x, y);
  return e;
}

export void move_by(compos *c, pog::eid id, float dx, float dy) {
  auto aabb = c->bodies().get(id);
  aabb.aa.x += dx;
  aabb.aa.y += dy;
  aabb.bb.x += dx;
  aabb.bb.y += dy;
  c->bodies().remove(id);
  c->bodies().add(id, aabb);
}
} // namespace collision