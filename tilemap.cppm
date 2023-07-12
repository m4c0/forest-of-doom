export module tilemap;
import missingno;
import tile;

export namespace tilemap {
constexpr const unsigned width = 16;
constexpr const unsigned height = 16;

struct compos : virtual tile::camping::compos, virtual tile::terrain::compos {};

class map {
  tile::camping::c m_data[height][width]{};

public:
  constexpr void fill(tile::camping::c t) noexcept {
    for (auto &row : m_data) {
      for (auto &tile : row) {
        tile = t;
      }
    }
  }
  constexpr mno::req<tile::camping::c> get(unsigned x,
                                           unsigned y) const noexcept {
    if (x < 0 || x >= width || y < 0 || y >= height)
      return mno::req<tile::camping::c>::failed("out-of-bounds");

    return mno::req<tile::camping::c>{m_data[y][x]};
  }
  constexpr void set(unsigned x, unsigned y, tile::camping::c t) noexcept {
    if (x < 0 || x >= width || y < 0 || y >= height)
      return;

    m_data[y][x] = t;
  }

  void add_entities(compos *ec, float dx, float dy) const noexcept {
    for (auto y = 0; y < height; y++) {
      for (auto x = 0; x < width; x++) {
        auto px = x + dx;
        auto py = y + dy;

        auto t = m_data[y][x];
        if (t == tile::camping::blank)
          continue;

        tile::camping::add_tile(ec, t, px, py);
      }
    }
  }
};
} // namespace tilemap
