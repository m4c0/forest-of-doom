export module tilemap;
import sprite;
import tile;

export namespace tilemap {
constexpr const unsigned width = 16;
constexpr const unsigned height = 16;

class map {
  using c = tile::c_t;

  c m_data[height][width]{};

public:
  constexpr auto get(unsigned x, unsigned y) const noexcept {
    return m_data[y][x];
  }

  constexpr void set(unsigned x, unsigned y, c t) noexcept {
    if (x < 0 || x >= width || y < 0 || y >= height)
      return;

    m_data[y][x] = t;
  }

  void for_each(auto &&fn) const {
    for (auto y = 0; y < height; y++) {
      for (auto x = 0; x < width; x++) {
        fn(x, y, get(x, y));
      }
    }
  }

  void add_entities(auto &&add_fn, float dx, float dy) const noexcept {
    for_each([&](auto x, auto y, auto t) {
      if (!t)
        return;

      auto px = x + dx;
      auto py = y + dy;
      add_fn(px, py, t);
    });
  }
};
} // namespace tilemap
