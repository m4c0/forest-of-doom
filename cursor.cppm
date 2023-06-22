export module cursor;
import pog;
import sprite;

export namespace cursor {
struct c {};
using compo = pog::singleton<c>;

void add_entity(pog::entity_list *e, compo *cur, sprite::compo *sprites) {
  auto id = e->alloc();
  cur->set(id, {});
  sprites->add(id, {.pos = {0, 0, 5, 5}, .uv = {1, 1, 1, 1}});
}

void update(compo *cur, sprite::compo *sprites, float x, float y) {
  auto id = cur->get_id();
  auto spr = sprites->get(id);
  spr.pos.x = x;
  spr.pos.y = y;
  sprites->update(id, spr);
}
} // namespace cursor
