export module qsu:hud;
import :layer;
import casein;
import jute;
import quack;
import sprite;

namespace qsu {
class hud_layer : public layer {
public:
  hud_layer(quack::renderer *m_r, unsigned max_sprites, jute::view atlas)
      : layer{m_r, sprite::layers::last, max_sprites, atlas} {}

  void process_event(const casein::event &e) {
    layer::process_event(e);

    if (e.type() == casein::CREATE_WINDOW) {
      (**this)->center_at(0, 0);
      (**this)->set_grid(16, 16);
    }
  }
};
} // namespace qsu
