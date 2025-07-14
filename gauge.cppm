export module gauge;
import dotz;
import pog;

namespace gauge {
export struct c {
  float value = 1;
  float drain = 0;
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
    g.value = dotz::clamp(g.value - g.drain * ms / 1000.0f, 0.f, 1.f);

    g.drain = 0;
    ec->gauges.update(id, g);
  }
}
} // namespace gauge
