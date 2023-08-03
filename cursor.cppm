export module cursor;
import pog;
import sprite;

export namespace cursor {
struct compos : virtual sprite::compos {
  pog::eid cursor;
};

void add_entity(compos *ec) { ec->cursor = sprite::add(ec, {}, {}); }

void update_sprite(compos *ec, rect uv) {
  auto id = ec->cursor;

  sprite::set_uv(ec, id, uv);
  sprite::set_size(ec, id, uv.w, uv.h);
}

void update_pos(compos *ec, float x, float y) {
  auto xx = static_cast<int>(x);
  auto yy = static_cast<int>(y);
  sprite::set_pos(ec, ec->cursor, xx, yy);
}
} // namespace cursor
