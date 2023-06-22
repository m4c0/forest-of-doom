export module cursor;
import pog;
import sprite;

export namespace cursor {
struct c {};
using compo = pog::singleton<c>;

void add_entity(pog::entity_list *e, compo *cur, sprite::compo *sprites) {
  auto id = e->alloc();
  cur->set(id, {});
  sprites->add(id, {.pos = {0, 0, 1, 1}, .uv = {1, 2, 1, 1}});
}

void update(compo *cur, sprite::compo *sprites, float x, float y) {
  auto id = cur->get_id();
  auto spr = sprites->get(id);
  spr.pos.x = static_cast<int>(x);
  spr.pos.y = static_cast<int>(y);
  sprites->update(id, spr);
}
} // namespace cursor
