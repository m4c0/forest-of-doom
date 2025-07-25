#pragma leco add_impl prefabs_lispy.cpp
#pragma leco add_resource_dir prefabs
export module prefabs;
import dotz;
import jute;
import hai;

using namespace jute::literals;

namespace prefabs {
  export struct assert_error {
    hai::cstr msg;

    explicit assert_error(jute::heap h) : msg { (*h).cstr() } {}
  };
  export struct parser_error {
    jute::heap msg;
    unsigned line;
    unsigned col;
  };

  export struct sprite {
    dotz::vec2 uv {};
    dotz::vec2 size {};
    unsigned texid {};
  };

  export struct tiledef {
    jute::heap id {};
    jute::heap behaviour {};
    sprite tile {};
    sprite entity {};
    dotz::vec4 collision {};
  };

  export constexpr const auto width = 16;
  export constexpr const auto height = 16;

  export class tilemap {
    hai::array<tiledef> m_data { width * height };
  public:
    [[nodiscard]] constexpr const auto & operator()(unsigned x, unsigned y) const {
      if (x >= width || y >= height) throw assert_error { "out of map bounds"_hs };
      return m_data[y * width + x];
    }
    [[nodiscard]] constexpr auto & operator()(unsigned x, unsigned y) {
      if (x >= width || y >= height) throw assert_error { "out of map bounds"_hs };
      return m_data[y * width + x];
    }

    void for_each(auto && fn) const {
      for (auto y = 0; y < height; y++) {
        for (auto x = 0; x < width; x++) {
          fn(x, y, (*this)(x, y));
        }
      }
    }
  };

  export const tilemap * parse(jute::view filename);
  export [[nodiscard]] const tilemap * load(jute::view filename);
}
