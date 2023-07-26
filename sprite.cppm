export module sprite;
import area;
import pog;
export import rect;

export namespace sprite {
enum class layers { terrain, camping, scout, last };

struct c {
  rect pos;
  rect uv;
  layers layer;
};
using compo = pog::sparse_set<c>;

struct compos : virtual pog::entity_provider, virtual area::compos {
  pog::sparse_set<c> sprites{};
};

auto add(compos *ec, c spr, rect r) {
  auto id = ec->e().alloc();
  area::add(ec, id, r);
  ec->sprites.add(id, spr);
  return id;
}
void set_pos(compos *ec, pog::eid id, float x, float y) {
  auto r = area::get(ec, id);
  r.x = x;
  r.y = y;

  area::remove(ec, id);
  area::add(ec, id, r);

  auto spr = ec->sprites.get(id);
  spr.pos.x = x;
  spr.pos.y = y;
  ec->sprites.update(id, spr);
}
void move_by(compos *ec, pog::eid id, float dx, float dy) {
  auto r = area::get(ec, id);
  set_pos(ec, id, r.x + dx, r.y + dy);
}
void set_uv(compos *ec, pog::eid id, float u, float v) {
  auto spr = ec->sprites.get(id);
  spr.uv.x = u;
  spr.uv.y = v;
  ec->sprites.update(id, spr);
}
} // namespace sprite
