export module bitmask;
import traits;

export class bitmask {
  traits::ints::uint64_t m_ones{};
  unsigned m_bit_count{};

public:
  constexpr void set(unsigned c) noexcept {
    auto mask = 1U << c;
    if (m_ones & mask)
      return;
    m_bit_count++;
    m_ones |= mask;
  }
  constexpr void reset(unsigned c) noexcept {
    auto mask = 1U << c;
    if (m_ones & mask) {
      m_ones ^= 1 << c;
      m_bit_count--;
    }
  }

  constexpr void merge(bitmask b) noexcept {
    auto i = (m_ones &= b.m_ones);
    m_bit_count = 0;
    while (i > 0) {
      if (i & 1)
        m_bit_count++;
      i >>= 1;
    }
  }

  [[nodiscard]] constexpr bool operator[](unsigned bit) const noexcept {
    return (m_ones & (1 << bit)) != 0;
  }

  [[nodiscard]] constexpr auto bits() const noexcept { return m_ones; }
  [[nodiscard]] constexpr auto bit_count() const noexcept {
    return m_bit_count;
  }
};
constexpr auto fail = []() -> bool { throw 0; };
static_assert([] {
  bitmask a{};
  a.set(1);
  a.set(2);
  a.reset(1);

  (a.bit_count() == 1) || fail();
  !a[1] || fail();
  a[2] || fail();

  return true;
}());
static_assert([] {
  bitmask a{};
  a.set(2);
  a.set(8);

  (!a[1] || fail());
  (a[2] || fail());
  (a[8] || fail());
  (a.bits() == 0b100000100) || fail();
  (a.bit_count() == 2 || fail());

  return true;
}());
static_assert([] {
  bitmask a{};
  a.set(2);
  a.set(6);
  a.set(8);

  bitmask b{};
  b.set(6);
  b.set(7);
  b.set(8);

  a.merge(b);

  (a.bit_count() == 2 || fail());

  (!a[2] || fail());
  (a[6] || fail());
  (!a[7] || fail());
  (a[8] || fail());
  (a.bits() == 0b101000000) || fail();
  return true;
}());
