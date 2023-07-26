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
  constexpr void set(unsigned x, unsigned y, c t) noexcept {
    if (x < 0 || x >= width || y < 0 || y >= height)
      return;

    m_data[y][x] = t;
  }

  void add_entities(tile::compos *ec, sprite::layers l, float dx,
                    float dy) const noexcept {
    for (auto y = 0; y < height; y++) {
      for (auto x = 0; x < width; x++) {
        auto px = x + dx;
        auto py = y + dy;

        auto t = m_data[y][x];
        if (!t)
          continue;

        add_tile(ec, t, l, px, py);
      }
    }
  }
};
} // namespace tilemap
