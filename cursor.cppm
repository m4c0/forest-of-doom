export module cursor;
import pog;
import sprite;

export namespace cursor {
using compo = pog::singleton<>;
struct compos {
  virtual pog::entity_list &e() noexcept = 0;
  virtual compo &cursor() noexcept = 0;
  virtual sprite::compo &sprites() noexcept = 0;
};

void add_entity(compos *ec) {
  auto id = ec->e().alloc();
  ec->cursor().set(id, {});
  ec->sprites().add(id, {.layer = 100});
}

void update(compos *ec, float x, float y) {
  auto id = ec->cursor().get_id();
  auto spr = ec->sprites().get(id);
  spr.pos.x = static_cast<int>(x);
  spr.pos.y = static_cast<int>(y);
  ec->sprites().update(id, spr);
}
} // namespace cursor
