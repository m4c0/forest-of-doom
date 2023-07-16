export module prefabs:ui_0;
import tile;
import tilemap;

namespace prefabs {
export constexpr const tilemap::map ui_0 = [] {
  using cs = tile::ui::compos;
  using c = cs::tile_t;
  tilemap::map<cs> res{};
  return res;
}();
}
