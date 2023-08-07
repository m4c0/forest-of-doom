export module eigen;
import bitmask;

namespace eigen {
export class world_decayed {};

export const auto max_entropy = ~0UL;
export class state {
  bitmask m_bits{};
  unsigned m_value{};

public:
  constexpr void set_one(unsigned c) noexcept { m_bits.set(c); }
  constexpr void merge(bitmask b) noexcept { m_bits.merge(b); }

  [[nodiscard]] constexpr bool operator[](unsigned bit) const noexcept {
    return m_bits[bit];
  }

  [[nodiscard]] constexpr auto bits() const noexcept { return m_bits; }
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
static constexpr auto fail = []() -> bool { throw 0; };
static_assert([] {
  state e{};
  e.set_one(12);
  e.set_one(3);
  e.set_one(10);
  e.set_one(9);

  e.observe(2) == 10 || fail();
  e.value() == 10 || fail();
  e.entropy() == 1 || fail();
  e.bits().bits() == 1 << 10 || fail();
  return true;
}());
} // namespace eigen
