export module player;
import pog;
import sprite;

namespace player {
export enum side {
  p_right = 0,
  p_up,
  p_left,
  p_down,
};
export struct c {
  side side;
};

export using compo = pog::singleton<c>;
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

export void set_side(compos *ec, side s) {
  auto pid = ec->player().get_id();
  auto p = ec->player().get(pid);
  p.side = s;
  ec->player().set(pid, p);

  auto spr = ec->player_sprites().get(pid);
  spr.uv.x = static_cast<int>(p.side);
  ec->player_sprites().update(pid, spr);
}
} // namespace player
