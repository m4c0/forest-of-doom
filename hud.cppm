export module hud;
import pog;
import sprite;
import tile;

namespace hud {
export class compos : public virtual pog::entity_provider {
  sprite::compo m_sprites{};

public:
  [[nodiscard]] auto &sprites() noexcept { return m_sprites; }
};
export void add_entities(compos *ec) {
  sprite spr{
      .pos = {0, 0, 1, 1},
      .uv = tile::uv(tile::ui::heart_l),
  };
  auto id = ec->e().alloc();
  ec->sprites().add(id, spr);
}
} // namespace hud
