export module player:inv;
import hai;
import loots;

namespace player::inv {
  export auto & inv() {
    static hai::array<loots::item> i { 8 };
    return i;
  }
}
