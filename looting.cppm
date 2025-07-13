export module looting;
import player;
import sprite;
import tile;

namespace looting {
export struct compos : virtual player::compos {
  pog::eid selected_lootable{};
  pog::sparse_set<pog::marker> lootable{};
};

pog::eid select_lootable(compos *ec) {
  auto pid = ec->player().eid;
  auto cid = ec->collisions.get(pid);
  if (!cid)
    return {};

  if (!ec->lootable.has(cid))
    return {};

  return cid;
}
export void mark_lootable(compos *ec) {
  auto old = ec->selected_lootable;
  auto cid = ec->selected_lootable = select_lootable(ec);

  if (old == cid)
    return;

  // Just tempsie, until the real fx is decided
  if (cid) {
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
