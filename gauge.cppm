export module gauge;
import pog;

namespace gauge {
class ranged {
  float m_value{1.0f};

public:
  constexpr operator float() const noexcept { return m_value; }

  constexpr auto &operator+=(float delta) noexcept {
    auto n = m_value + delta;
    m_value = n > 1 ? 1 : n;
    return *this;
  }
  constexpr auto &operator-=(float delta) noexcept {
    auto n = m_value - delta;
    m_value = n < 0 ? 0 : n;
    return *this;
  }
};

export struct c {
  ranged value;
  float drain;
};
export struct compos : virtual pog::entity_provider {
  pog::sparse_set<c> gauges{};
};

export auto add_gauge(compos *ec) {
  auto id = ec->e().alloc();
  ec->gauges.add(id, {});
  return id;
}

export void add_drain(compos *ec, pog::eid id, float val_per_sec) {
  auto v = ec->gauges.get(id);
  v.drain += val_per_sec;
  ec->gauges.update(id, v);
}

export void run_drains(compos *ec, float ms) {
  for (auto [id, g] : ec->gauges) {
    g.value -= g.drain * ms / 1000.0f;
    g.drain = 0;
    ec->gauges.update(id, g);
  }
}
} // namespace gauge
