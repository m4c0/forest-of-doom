export module area;
import pog;
import rect;

export namespace area {
using c = pog::aabb;
using compo = pog::rtree;
class compos {
  compo m_areas{};

public:
  compo &areas() noexcept { return m_areas; }
};

void add(compos *ec, pog::eid id, rect r) {
  ec->areas().add(id, c{r.x, r.y, r.x + r.w, r.y + r.h});
}
void remove(compos *ec, pog::eid id) { ec->areas().remove(id); }
} // namespace area
