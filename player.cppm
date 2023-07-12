export module player;
import animation;
import collision;
import input;
import move;
import pog;
import sprite;

extern "C" float sqrtf(float);

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
export struct compos : virtual collision::compos {
  virtual pog::entity_list &e() noexcept = 0;
  virtual compo &player() noexcept = 0;
  virtual animation::compo &animations() noexcept = 0;
  virtual move::compo &movements() noexcept = 0;
  virtual sprite::compo &player_sprites() noexcept = 0;
};

export void add_entity(compos *ec) {
  constexpr const auto sx = 8.0f;
  constexpr const auto sy = 8.0f;

  sprite spr{
      .pos = {sx, sy, 1, 2},
      .uv = {0, 2, 1, 2},
  };
  animation::c a{
      .start_x = 0,
      .y = 0,
      .num_frames = 1,
  };
  auto pid = ec->e().alloc();
  ec->player().set(pid, {});
  ec->player_sprites().add(pid, spr);
  ec->animations().add(pid, a);
  ec->movements().add(pid, {});
  collision::add(ec, pid, sx, sy + 0.9f, 1, 1);
}

void update_compo(compos *ec, side s, animation::c a) {
  a.start_x = static_cast<unsigned>(s) * a.num_frames;

  auto pid = ec->player().get_id();

  auto cur_a = ec->animations().get(pid);
  if (a.start_x == cur_a.start_x && a.y == cur_a.y)
    return;

  ec->player().set(pid, {s});
  ec->animations().update(pid, a);
}

void set_idle_animation(compos *ec) {
  constexpr const auto num_frames = 6;
  constexpr const auto frames_per_sec = 6;

  auto pid = ec->player().get_id();
  auto s = ec->player().get(pid).side;

  update_compo(ec, s,
               {
                   .y = 2,
                   .num_frames = num_frames,
                   .frames_per_sec = frames_per_sec,
               });
}
void set_walk_animation(compos *ec, side s) {
  constexpr const auto num_frames = 6;
  constexpr const auto frames_per_sec = 24;

  update_compo(ec, s,
               {
                   .y = 4,
                   .num_frames = num_frames,
                   .frames_per_sec = frames_per_sec,
               });
}

export void process_input(input::dual_axis in, compos *ec) {
  constexpr const auto blocks_per_sec = 4.0f;
  constexpr const auto speed = blocks_per_sec / 1000.0f;

  auto pid = ec->player().get_id();
  auto h = in.h_value();
  auto v = in.v_value();
  if (v != 0) {
    set_walk_animation(ec, v > 0 ? p_down : p_up);
  } else if (h != 0) {
    set_walk_animation(ec, h > 0 ? p_right : p_left);
  } else {
    set_idle_animation(ec);
  }

  if (v == 0 && h == 0) {
    ec->movements().update(pid, {});
    return;
  }

  float d = sqrtf(h * h + v * v);
  float sx = h * speed / d;
  float sy = v * speed / d;

  ec->movements().update(pid, {sx, sy});
}

export rect get_area(compos *ec) {
  auto pid = ec->player().get_id();
  auto spr = ec->player_sprites().get(pid);
  return spr.pos;
}
} // namespace player
