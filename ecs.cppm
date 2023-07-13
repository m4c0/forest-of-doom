export module ecs;
import animation;
import pog;
import player;
import tilemap;

namespace ecs {
export class ec : public player::compos, public tilemap::compos {
  animation::compo m_animations{};

public:
  animation::compo &animations() noexcept override { return m_animations; }
};
} // namespace ecs
