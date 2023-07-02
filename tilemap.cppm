export module tilemap;
import chunk;
import collision;
import missingno;
import tile;
import tiles;

export namespace tilemap {
constexpr const unsigned width = 16;
constexpr const unsigned height = 16;

struct compos : collision::compos, tiles::compos {
  virtual chunk::compo &chunks() noexcept = 0;
};

class map {
  tile m_data[height][width]{};

public:
  constexpr void fill(tile t) noexcept {
    for (auto &row : m_data) {
      for (auto &tile : row) {
        tile = t;
      }
    }
  }
  constexpr mno::req<tile> get(unsigned x, unsigned y) const noexcept {
    if (x < 0 || x >= width || y < 0 || y >= height)
      return mno::req<tile>::failed("out-of-bounds");

    return mno::req<tile>{m_data[y][x]};
  }
  constexpr void set(unsigned x, unsigned y, tile t) noexcept {
    if (x < 0 || x >= width || y < 0 || y >= height)
      return;

    m_data[y][x] = t;
  }

  void add_entities(compos *ec, chunk::c chunk, float dx,
                    float dy) const noexcept {
    for (auto y = 0; y < height; y++) {
      for (auto x = 0; x < width; x++) {
        auto t = m_data[y][x];
        if (t == blank)
          continue;

        auto e = tiles::add_tile(ec, t, x + dx, y + dy);
        ec->chunks().add(e, chunk);
      }
    }
  }
};
} // namespace tilemap
