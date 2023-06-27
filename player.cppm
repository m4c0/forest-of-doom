export module player;
import pog;
import sprite;

namespace player {
export using compo = pog::singleton<>;
export struct compos {
  virtual pog::entity_list &e() noexcept = 0;
  virtual compo &player() noexcept = 0;
  virtual sprite::compo &player_sprites() noexcept = 0;
};

export void add_entity(compos *ec) {
  sprite spr{
      .pos = {8, 8, 1, 2},
      .uv = {0, 0, 1, 2},
  };
  auto pid = ec->e().alloc();
  ec->player().set(pid, {});
  ec->player_sprites().add(pid, spr);
}

export void update_sprite(compos *ec, float x) {
  auto pid = ec->player().get_id();
  auto spr = ec->player_sprites().get(pid);
  spr.uv.x = x;
  ec->player_sprites().update(pid, spr);
}
} // namespace player
