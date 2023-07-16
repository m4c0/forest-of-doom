export module hud;
import tile;
import sprite;

namespace hud {
export class compos {
  sprite::compo m_sprites{};

public:
  [[nodiscard]] constexpr auto &sprites() noexcept { return m_sprites; }
};
export void add_entities(compos *ec) {
  //
}
} // namespace hud
