export module stopwatch;
import sitime;

export class stopwatch {
  sitime::stopwatch m_watch{};

public:
  [[nodiscard]] auto current_millis() const noexcept {
    return m_watch.millis();
  }
  void reset_watch() { m_watch = {}; }
};
