export module backpack;
import fox;
import hai;

namespace backpack {
  hai::varray<fox::sprite> list { 128 };

  export void add(fox::sprite t) {
    list.push_back_doubling(t);
  }
  export void load(fox::memiter * m) {
    for (auto & i : list) {
      *m += i;
    }
  }
}
