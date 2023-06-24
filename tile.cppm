export module tile;

export enum tile : unsigned {
  blank = 0,
  island_tl = 0x00010101,
  island_t = 0x01010101,
  island_tr = 0x03010101,
  island_l = 0x00020101,
  grass_0 = 0x01020101,
  grass_1 = 0x02020101,
  island_r = 0x03020101,
  island_bl = 0x00030102,
  island_b = 0x01030102,
  island_br = 0x03030102,
  lake_tl = 0x04010102,
  lake_tr = 0x07010102,
  lake_bl = 0x04040101,
  lake_br = 0x07040101,
  water = 0x05030101,
};
