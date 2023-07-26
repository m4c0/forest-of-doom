export module looting;
import player;
import silog;
import tile;

export namespace looting {
struct compos : virtual player::compos, virtual tile::compos {
  pog::sparse_set<pog::marker> lootable{};
};

void add_backpack(compos *ec, tile::camping::c c, float x, float y) {
  auto id = tile::camping::add_tile(ec, c, x, y);
  ec->lootable.add(id, {});
}

void mark_lootable(compos *ec) {
  auto pid = ec->player().eid;
  auto cid = ec->collisions.get(pid);
  if (!cid)
    return;

  if (!ec->lootable.has(cid))
    return;

  silog::log(silog::debug, "got %d", (unsigned)cid);
}
} // namespace looting
