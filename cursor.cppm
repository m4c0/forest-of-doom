export module cursor;
import pog;
import tile;

export namespace cursor {
using compo = pog::singleton<>;
class compos : public virtual tile::camping::compos {
  cursor::compo m_cursor{};

public:
  virtual pog::entity_list &e() noexcept = 0;
  cursor::compo &cursor() noexcept { return m_cursor; }
};

void add_entity(compos *ec) {
  auto id = tile::camping::add_tile(ec, {}, 0, 0);
  ec->cursor().set(id, {});
  // ec->sprite_layer().add(id, 100);
}

void update_tile(compos *ec, tile::camping::c t) {
  auto id = ec->cursor().get_id();
  ec->camping_tiles().update(id, t);
}

void update_pos(compos *ec, float x, float y) {
  auto id = ec->cursor().get_id();
  tile::camping::update_tile_pos(ec, id, static_cast<int>(x),
                                 static_cast<int>(y));
}
} // namespace cursor
