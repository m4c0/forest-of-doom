export module mapgen;
import eigen;
import tile;

namespace mapgen {
constexpr const auto pats = [] {
  eigen::pat_list p{};

  p[0].set_row(0, "...");
  p[0].set_row(1, ".XX");
  p[0].set_row(2, ".XX");
  // p[0].probability() = 1.f;

  p[1].set_row(0, ".XX");
  p[1].set_row(1, ".XX");
  p[1].set_row(2, ".XX");
  // p[1].probability() = 1.f;

  p[2].set_row(0, "XXX");
  p[2].set_row(1, "XXX");
  p[2].set_row(2, "XXX");
  // p[2].probability() = 1.f;

  p[3].set_row(0, "...");
  p[3].set_row(1, "...");
  p[3].set_row(2, "...");
  // p[3].probability() = 1.f;

  (p[4] = p[0]).rotate();
  (p[5] = p[4]).rotate();
  (p[6] = p[5]).rotate();

  (p[7] = p[1]).rotate();
  (p[8] = p[7]).rotate();
  (p[9] = p[8]).rotate();

  p[10].set_row(0, "XXX");
  p[10].set_row(1, "XXX");
  p[10].set_row(2, ".XX");
  // p[10].probability() = 1.f;

  (p[11] = p[10]).rotate();
  (p[12] = p[11]).rotate();
  (p[13] = p[12]).rotate();

  return p;
}();

void print(tile::terrain::compos *ec, unsigned x, unsigned y, unsigned pat) {
  static_assert(static_cast<unsigned>(tile::terrain::last) < 64);
  constexpr const tile::terrain::c tiles[pats.size()]{
      tile::terrain::grass_tl, tile::terrain::grass_l,  tile::terrain::grass_0,
      tile::terrain::dirt_0,   tile::terrain::grass_bl, tile::terrain::grass_br,
      tile::terrain::grass_tr, tile::terrain::grass_b,  tile::terrain::grass_r,
      tile::terrain::grass_t,  tile::terrain::dirt_tr,  tile::terrain::dirt_tl,
      tile::terrain::dirt_bl,  tile::terrain::dirt_br,
  };
  if (pat == eigen::nil)
    return;

  auto t = tiles[pat];
  tile::terrain::add_tile(ec, t, x, y);
}

export inline constexpr const auto map_size = 64;

export void gen(tile::terrain::compos *ec) {
  eigen::map map{&pats, map_size, map_size};
  map.draw_border(2);
  map.fill();

  unsigned y = 0;
  for (auto row : map) {
    unsigned x = 0;
    for (auto col : row) {
      print(ec, x, y, col);
      x++;
    }
    y++;
  }
}
} // namespace mapgen
