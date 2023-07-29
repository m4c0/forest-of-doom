export module looting;
import player;
import sprite;
import tile;

namespace looting {
export struct compos : virtual player::compos, virtual tile::compos {
  pog::eid selected_lootable{};
  pog::sparse_set<pog::marker> lootable{};
};

export void add_backpack(compos *ec, tile::camping::c c, float x, float y) {
  auto id = tile::camping::add_tile(ec, c, x, y);
  ec->lootable.add(id, {});
}

void select_lootable(compos *ec) {
  auto pid = ec->player().eid;
  auto cid = ec->collisions.get(pid);
  if (!cid)
    return;

  if (!ec->lootable.has(cid))
    return;

  ec->selected_lootable = cid;
}
export void mark_lootable(compos *ec) {
  auto old = ec->selected_lootable;
  select_lootable(ec);

  if (old == ec->selected_lootable)
    return;

  // Just tempsie, until the real fx is decided
  if (auto cid = ec->selected_lootable) {
    auto spr = ec->sprites.get(cid);
    spr.dim = 0.1;
    ec->sprites.update(cid, spr);
  }
  if (old) {
    auto spr = ec->sprites.get(old);
    spr.dim = 0;
    ec->sprites.update(old, spr);
  }
}
} // namespace looting
