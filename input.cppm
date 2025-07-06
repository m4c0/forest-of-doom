export module input;
import casein;

export namespace input {
template <casein::keys K> class button {
  bool m_down{};

public:
  button() {
    using namespace casein;
    handle(KEY_DOWN, K, [this] { m_down = true; });
    handle(KEY_UP,   K, [this] { m_down = false; });
  }

  [[nodiscard]] constexpr auto value() const noexcept { return m_down; }

};

template <casein::keys N, casein::keys P> class axis {
  button<N> m_n;
  button<P> m_p;

public:
  [[nodiscard]] int value() const noexcept {
    if (m_n.value() && !m_p.value()) return -1;
    if (m_p.value() && !m_n.value()) return 1;
    return 0;
  }
};

using h_axis = axis<casein::K_LEFT, casein::K_RIGHT>;
using v_axis = axis<casein::K_UP, casein::K_DOWN>;

class dual_axis {
  h_axis m_h{};
  v_axis m_v{};

public:
  [[nodiscard]] int h_value() const noexcept { return m_h.value(); }
  [[nodiscard]] int v_value() const noexcept { return m_v.value(); }
};

class state {
  dual_axis m_move;
  button<casein::K_SPACE> m_rest;

public:
  [[nodiscard]] int h_value() const noexcept { return m_move.h_value(); }
  [[nodiscard]] int v_value() const noexcept { return m_move.v_value(); }
  [[nodiscard]] bool rest() const noexcept { return m_rest.value(); }
};
} // namespace input
