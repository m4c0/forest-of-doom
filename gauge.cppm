export module gauge;
import pog;
import ranged;

namespace gauge {
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
} // namespace gauge
