export module prefabs:island_0;
import tile;
import tilemap;

namespace prefabs {
export constexpr const tilemap::map island_0 = [] {
  tilemap::map res{};
  res.set(0, 0, island_tl);
  return res;
}();
}
