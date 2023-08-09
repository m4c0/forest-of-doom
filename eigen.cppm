export module eigen;
import area;
import bitmask;
import pog;
import tile;
import tilemap;

namespace eigen {
export class world_decayed {};

export const auto max_entropy = 64; // number of bits in uint64
export class state {
  bitmask m_bits{};
  bitmask m_stage{};
  unsigned m_value{};

public:
  constexpr state() = default;
  explicit constexpr state(bitmask b) : m_bits{b} {}

  constexpr void reset_stage() { m_stage = {}; }
  constexpr void set_stage(unsigned i) { m_stage.set(i); }
  constexpr void apply_stage() { m_bits.merge(m_stage); }

  [[nodiscard]] constexpr bool operator[](unsigned bit) const noexcept {
    return m_bits[bit];
  }

  [[nodiscard]] constexpr auto bits() const noexcept { return m_bits.bits(); }
  [[nodiscard]] constexpr auto entropy() const noexcept {
    return m_bits.bit_count();
  }
  [[nodiscard]] constexpr auto value() const noexcept { return m_value; }

  constexpr auto observe(unsigned r) {
    for (auto bit = 0U; bit < max_entropy; bit++) {
      if (!m_bits[bit])
        continue;

      if (r-- > 0)
        continue;

      m_bits = {};
      m_bits.set(bit);
      m_value = bit;
      return bit;
    }
    // Should never happen
    throw world_decayed{};
  }
};

export struct consts {
  tilemap::map pat;
  bitmask mask;
};
export const consts create_consts(auto &&fn) {
  tile::terrain::compos tmp{};
  fn(&tmp, 0, 0);

  tilemap::map pat{};
  bitmask e{};
  for (auto &[id, t] : tmp.tiles) {
    auto [x, y, w, h] = area::get(&tmp, id);
    pat.set(x, y, t);
    e.set(t);
  }
  return consts{pat, e};
}

export struct compos : virtual area::compos {
  pog::sparse_set<state> eigen_states{};
  pog::sparse_set<bitmask> eigen_stages{};
};
} // namespace eigen

module :private;
static constexpr auto fail = []() -> bool { throw 0; };
static_assert([] {
  bitmask b{};
  b.set(12);
  b.set(3);
  b.set(10);
  b.set(9);

  eigen::state e{b};
  e.observe(2) == 10 || fail();
  e.value() == 10 || fail();
  e.entropy() == 1 || fail();
  e.bits() == 1 << 10 || fail();
  return true;
}());
