export module qsu:hud;
import :layer;
import casein;

namespace qsu {
class hud_layer : public layer {
public:
  using layer::layer;

  void process_event(const casein::event &e) {
    layer::process_event(e);

    if (e.type() == casein::CREATE_WINDOW) {
      (**this)->center_at(0, 0);
      (**this)->set_grid(16, 16);
    }
  }
};
} // namespace qsu
