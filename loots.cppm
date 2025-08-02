export module loots;
import dotz;
import hai;
import jute;

namespace loots {
  export struct item {
    dotz::vec2 sprite;
  };

  export auto load(jute::view table_name) {
    hai::array<dotz::vec2> inv { 8 };
    if (table_name == "loot0") {
      inv[0] = { 11, 3 };
    } else if (table_name == "loot1") {
      inv[0] = { 13, 3 };
    } else if (table_name == "loot2") {
      inv[0] = { 12, 2 };
    }
    return inv;
  }
}
