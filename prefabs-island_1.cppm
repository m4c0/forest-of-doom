export module prefabs:island_1;
import tile;
import tilemap;

namespace prefabs {
export constexpr const tilemap::map island_1 = [] {
  tilemap::map<tile::terrain::compos> res{};
  return res;
}();
}