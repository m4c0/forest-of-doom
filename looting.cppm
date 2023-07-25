export module looting;
import tile;

export namespace looting {
struct compos : tile::camping::compos {
  pog::sparse_set<pog::marker> lootable{};
};

void add_backpack(compos *ec, tile::camping::c c, float x, float y) {
  auto id = tile::camping::add_tile(ec, c, x, y);
  ec->lootable.add(id, {});
}

} // namespace looting
