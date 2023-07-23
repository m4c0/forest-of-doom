export module input;
import casein;

export namespace input {
template <casein::keys K> class button {
  bool m_down{};

public:
  [[nodiscard]] constexpr auto value() const noexcept { return m_down; }

  void process_event(const casein::event &e) {
    switch (e.type()) {
    case casein::KEY_DOWN: {
      auto key = *e.as<casein::events::key_down>();
      if (key == K)
        m_down = true;
      break;
    }
    case casein::KEY_UP: {
      auto key = *e.as<casein::events::key_down>();
      if (key == K)
        m_down = false;
      break;
    }
    default:
      break;
    }
  }
};

template <casein::keys N, casein::keys P> class axis {
  button<N> m_n;
  button<P> m_p;

public:
  [[nodiscard]] int value() const noexcept {
    if (m_n.value() && !m_p.value())
      return -1;
    if (m_p.value() && !m_n.value())
      return 1;
    return 0;
  }

  void process_event(const casein::event &e) {
    m_n.process_event(e);
    m_p.process_event(e);
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

  void process_event(const casein::event &e) {
    m_h.process_event(e);
    m_v.process_event(e);
  }
};

class state {
  dual_axis m_move;
  button<casein::K_SPACE> m_rest;

public:
  [[nodiscard]] int h_value() const noexcept { return m_move.h_value(); }
  [[nodiscard]] int v_value() const noexcept { return m_move.v_value(); }
  [[nodiscard]] bool rest() const noexcept { return m_rest.value(); }

  void process_event(const casein::event &e) {
    m_move.process_event(e);
    m_rest.process_event(e);
  }
};
} // namespace input
