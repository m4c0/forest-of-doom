export module tilemap;
import ecs;
import tile;
import tiles;

export class tilemap {
  static constexpr unsigned width = 16;
  static constexpr unsigned height = 16;

  tile m_data[height][width];

public:
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
    if (tiles::tile_uv(t).h > 1)
      set(x, y + 1, blank);
  }

  constexpr void add_island(unsigned cx, unsigned cy) noexcept {
    set(cx - 1, cy - 1, island_tl);
    set(cx, cy - 1, island_t);
    set(cx + 1, cy - 1, island_tr);
    set(cx - 1, cy, island_l);
    set(cx, cy, grass_0);
    set(cx + 1, cy, island_r);
    set(cx - 1, cy + 1, island_bl);
    set(cx, cy + 1, island_b);
    set(cx + 1, cy + 1, island_br);
  }

  void add_entities(ecs::ec *ec, float dx, float dy) const noexcept {
    for (auto y = 0; y < height; y++) {
      for (auto x = 0; x < width; x++) {
        auto t = m_data[y][x];
        if (t == blank)
          continue;

        tiles::add_tile(ec, t, x + dx, y + dy);
      }
    }
  }
};
