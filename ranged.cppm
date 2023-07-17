export module ranged;

export class ranged {
  float m_value{1.0f};

public:
  constexpr operator float() const noexcept { return m_value; }

  constexpr auto &operator+=(float delta) noexcept {
    auto n = m_value + delta;
    m_value = n > 1 ? 1 : n;
    return *this;
  }
  constexpr auto &operator-=(float delta) noexcept {
    m_value = delta > m_value ? 0 : m_value - delta;
    return *this;
  }
};
