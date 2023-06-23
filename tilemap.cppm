export module tilemap;
import chunk;
import tile;
import tiles;

export namespace tilemap {
constexpr const unsigned width = 16;
constexpr const unsigned height = 16;

class map {
  tile m_data[height][width]{};
  chunk::c m_chunk;

public:
  explicit constexpr map(chunk::c ch) : m_chunk{ch} {}

  constexpr void fill(tile t) noexcept {
    for (auto &row : m_data) {
      for (auto &tile : row) {
        tile = t;
      }
    }
  }
  constexpr void set(unsigned x, unsigned y, tile t) noexcept {
    if (x < 0 || x >= width || y < 0 || y >= height)
      return;

    m_data[y][x] = t;
  }

  void add_entities(tiles::builder tb, chunk::compo *c, float dx,
                    float dy) const noexcept {
    for (auto y = 0; y < height; y++) {
      for (auto x = 0; x < width; x++) {
        auto t = m_data[y][x];
        if (t == blank)
          continue;

        auto e = tiles::add_tile(tb, t, x + dx, y + dy);
        c->add(e, m_chunk);
      }
    }
  }
};
} // namespace tilemap
