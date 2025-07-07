#pragma leco add_impl prefabs_impl.cpp
export module prefabs;
export import :island_0;
export import :island_1;
import dotz;
import jute;
import hai;

using namespace jute::literals;

namespace prefabs {
  export struct error {
    hai::cstr msg;
    unsigned line_number;

    error(jute::heap h) : msg { (*h).cstr() } {}
  };

  export struct tiledef {
    jute::heap id;
    unsigned   tile;
    dotz::vec4 collision;
  };

  export constexpr const auto width = 16;
  export constexpr const auto height = 16;

  export class tilemap {
    hai::array<tiledef> m_data { width * height };
  public:
    [[nodiscard]] constexpr const auto & operator()(unsigned x, unsigned y) const {
      if (x >= width || y >= height) throw error { "out of map bounds"_hs };
      return m_data[y * width + x];
    }
    [[nodiscard]] constexpr auto & operator()(unsigned x, unsigned y) {
      if (x >= width || y >= height) throw error { "out of map bounds"_hs };
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

  export [[nodiscard]] tilemap load(jute::view filename);
}
