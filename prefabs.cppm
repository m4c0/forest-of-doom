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

  export struct sprite {
    dotz::vec2 uv {};
    dotz::vec2 size {};
    unsigned texid {};
  };

  export struct tiledef {
    jute::heap behaviour {};
    jute::heap loot {};
    jute::heap entry {};
    jute::heap exit {};
    sprite tile {};
    sprite entity {};
    sprite over {};
    dotz::vec4 collision {};
  };

  export class tilemap {
    unsigned m_width = 0;
    unsigned m_height = 0;
    hai::array<tiledef> m_data {};
  public:
    constexpr tilemap() = default;
    constexpr tilemap(unsigned w, unsigned h) : 
      m_width { w }
    , m_height { h }
    , m_data { w * h }
    {}

    [[nodiscard]] constexpr operator bool() const { return m_data.size() > 0; }

    [[nodiscard]] constexpr const auto & operator()(unsigned x, unsigned y) const {
      if (x >= m_width || y >= m_height) throw assert_error { "out of map bounds"_hs };
      return m_data[y * m_width + x];
    }
    [[nodiscard]] constexpr auto & operator()(unsigned x, unsigned y) {
      if (x >= m_width || y >= m_height) throw assert_error { "out of map bounds"_hs };
      return m_data[y * m_width + x];
    }

    [[nodiscard]] constexpr dotz::ivec2 size() const { return { m_width, m_height }; }

    void for_each(auto && fn) const {
      for (auto y = 0; y < m_height; y++) {
        for (auto x = 0; x < m_width; x++) {
          fn(x, y, (*this)(x, y));
        }
      }
    }
  };

  export tilemap parse(jute::view src);
  export void load(jute::view name, hai::fn<void, tilemap> callback);
}
