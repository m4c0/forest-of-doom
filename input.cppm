export module input;
import casein;

namespace input {
template <casein::keys N, casein::keys P> class axis {
  bool m_n{};
  bool m_p{};

public:
  [[nodiscard]] int value() const noexcept {
    if (m_n && !m_p)
      return -1;
    if (m_p && !m_n)
      return 1;
    return 0;
  }

  void process_event(const casein::event &e) {
    switch (e.type()) {
    case casein::KEY_DOWN: {
      auto key = *e.as<casein::events::key_down>();
      if (key == N)
        m_n = true;
      if (key == P)
        m_p = true;
      break;
    }
    case casein::KEY_UP: {
      auto key = *e.as<casein::events::key_down>();
      if (key == N)
        m_n = false;
      if (key == P)
        m_p = false;
      break;
    }
    default:
      break;
    }
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
} // namespace input
