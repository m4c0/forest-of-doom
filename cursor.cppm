export module cursor;
import pog;
import sprite;
import tile;

namespace t = tile::camping;

export namespace cursor {
using compo = pog::singleton<sprite>;
class compos : public virtual pog::entity_provider {
  cursor::compo m_cursor{};

public:
  cursor::compo &cursor() noexcept { return m_cursor; }
};

void add_entity(compos *ec) {
  auto id = ec->e().alloc();
  ec->cursor().set(id, {});
}

void add_sprite(compos *ec, sprite::compo &spr) {
  auto id = ec->cursor().get_id();
  spr.add(id, ec->cursor().get(id));
}

void update_tile(compos *ec, t::c c) {
  auto id = ec->cursor().get_id();
  auto spr = ec->cursor().get(id);
  auto uv = tile::uv(c);
  spr.pos.w = uv.w;
  spr.pos.h = uv.h;
  spr.uv = uv;
  ec->cursor().set(id, spr);
}

void update_pos(compos *ec, float x, float y) {
  auto id = ec->cursor().get_id();
  auto spr = ec->cursor().get(id);
  spr.pos.x = static_cast<int>(x);
  spr.pos.y = static_cast<int>(y);
  ec->cursor().set(id, spr);
}
} // namespace cursor
