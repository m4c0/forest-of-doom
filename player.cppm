export module player;
import anime;
import input;
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
  virtual anime::compo &animations() noexcept = 0;
  virtual sprite::compo &player_sprites() noexcept = 0;
};

export void add_entity(compos *ec) {
  sprite spr{
      .pos = {8, 8, 1, 2},
      .uv = {0, 2, 1, 2},
  };
  anime::c a{
      .start_x = 0,
      .y = 0,
      .num_frames = 1,
  };
  auto pid = ec->e().alloc();
  ec->player().set(pid, {});
  ec->player_sprites().add(pid, spr);
  ec->animations().add(pid, a);
}

void update_compo(compos *ec, side s, anime::c a) {
  auto pid = ec->player().get_id();
  ec->player().set(pid, {s});
  ec->animations().update(pid, a);
}

void set_idle_animation(compos *ec) {
  constexpr const auto num_frames = 6;
  auto pid = ec->player().get_id();
  auto s = ec->player().get(pid).side;

  update_compo(ec, s,
               {
                   .start_x = static_cast<unsigned>(s) * num_frames,
                   .y = 2,
                   .num_frames = num_frames,
               });
}
void set_walk_animation(compos *ec, side s) {
  constexpr const auto num_frames = 6;
  update_compo(ec, s,
               {
                   .start_x = static_cast<unsigned>(s) * num_frames,
                   .y = 4,
                   .num_frames = num_frames,
               });
}

export void process_input(input::dual_axis in, compos *ec) {
  auto h = in.h_value();
  auto v = in.v_value();
  if (v != 0) {
    player::set_walk_animation(ec, v > 0 ? p_down : p_up);
  } else if (h != 0) {
    player::set_walk_animation(ec, h > 0 ? p_right : p_left);
  } else {
    player::set_idle_animation(ec);
  }
}
} // namespace player
