export module sprite;
import area;
import pog;
export import rect;

export namespace sprite {
enum class layers { terrain, camping, scout, ui, last };

struct c {
  rect uv;
  layers layer;
};
using compo = pog::sparse_set<c>;

struct compos : virtual pog::entity_provider, virtual area::compos {
  pog::sparse_set<c> sprites{};
};

auto add(compos *ec, pog::eid id, c spr, rect r) {
  area::add(ec, id, r);
  ec->sprites.add(id, spr);
  return id;
}
auto add(compos *ec, c spr, rect r) {
  auto id = ec->e().alloc();
  return add(ec, id, spr, r);
}
void remove(compos *ec, pog::eid id) {
  ec->sprites.remove(id);
  area::remove(ec, id);
  ec->e().dealloc(id);
}
void set_pos(compos *ec, pog::eid id, float x, float y) {
  auto r = area::get(ec, id);
  r.x = x;
  r.y = y;

  area::remove(ec, id);
  area::add(ec, id, r);
}
void set_size(compos *ec, pog::eid id, float w, float h) {
  auto r = area::get(ec, id);
  r.w = w;
  r.h = h;

  area::remove(ec, id);
  area::add(ec, id, r);
}
void move_by(compos *ec, pog::eid id, float dx, float dy) {
  auto r = area::get(ec, id);
  set_pos(ec, id, r.x + dx, r.y + dy);
}
void set_uv(compos *ec, pog::eid id, rect uv) {
  auto spr = ec->sprites.get(id);
  spr.uv = uv;
  ec->sprites.update(id, spr);
}
void set_uv(compos *ec, pog::eid id, float u, float v) {
  auto spr = ec->sprites.get(id);
  spr.uv.x = u;
  spr.uv.y = v;
  ec->sprites.update(id, spr);
}
} // namespace sprite
