#pragma leco add_impl loots_lispy
#pragma leco add_resource_dir loots
export module loots;
import dotz;
import hai;
import jute;

namespace loots {
  export struct item {
    dotz::vec2 sprite;

    [[nodiscard]] constexpr operator bool() {
      return sprite.x || sprite.y;
    }
  };

  export hai::array<item> load(jute::view table_name);
}
