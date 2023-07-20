export module player;
import animation;
import collision;
import input;
import movement;
import ranged;
import pog;
import sprite;
import stopwatch;

extern "C" float sqrtf(float);

namespace player {
constexpr const auto energy_lost_per_sec = 0.025f;
constexpr const auto food_lost_per_sec = 0.1f;
constexpr const auto food_energy_ratio = 2.f;

constexpr const auto starvation_limit = 0.25f;
constexpr const auto starvation_mental_loss_per_sec = 0.1f;
constexpr const auto starvation_health_loss_per_sec = 0.1f;

export enum side {
  p_right = 0,
  p_up,
  p_left,
  p_down,
};
export struct c {
  side side;

  ranged energy;
  ranged happyness;
  ranged health;
  ranged satiation;
};

export using compo = pog::singleton<c>;
export class compos : public virtual movement::compos,
                      public virtual pog::entity_provider,
                      public virtual animation::compos,
                      public virtual stopwatch {
  player::compo m_player{};
  sprite::compo m_player_sprites{};

public:
  player::compo &player() noexcept { return m_player; }
  sprite::compo &player_sprites() noexcept { return m_player_sprites; }
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

bool update_compo(compos *ec, unsigned s, animation::c a) {
  a.start_x = s * a.num_frames;

  auto pid = ec->player().get_id();

  auto cur_a = ec->animations().get(pid);
  if (a.start_x == cur_a.start_x && a.y == cur_a.y)
    return false;

  ec->animations().update(pid, a);
  return true;
}
void update_compo(compos *ec, side s, animation::c a) {
  if (!update_compo(ec, static_cast<unsigned>(s), a))
    return;

  auto pid = ec->player().get_id();
  auto p = ec->player().get(pid);
  p.side = s;
  ec->player().set(pid, p);
}

void process_starvation(compos *ec) {
  auto pid = ec->player().get_id();
  auto p = ec->player().get(pid);

  if (p.satiation > starvation_limit)
    return;

  auto adj_food = 1.0f - (p.satiation - starvation_limit) / starvation_limit;

  p.happyness -=
      adj_food * starvation_mental_loss_per_sec * ec->current_millis() / 1000.f;
  p.health -=
      adj_food * starvation_health_loss_per_sec * ec->current_millis() / 1000.f;

  ec->player().set(pid, p);
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
void set_sit_animation(compos *ec) {
  constexpr const auto num_frames = 6;
  constexpr const auto frames_per_sec = 6;

  auto pid = ec->player().get_id();
  unsigned s = ec->player().get(pid).side / 2U;

  update_compo(ec, s,
               {
                   .y = 8,
                   .num_frames = num_frames,
                   .frames_per_sec = frames_per_sec,
               });
}
void set_walk_animation(compos *ec, side s) {
  constexpr const auto num_frames = 6;
  constexpr const auto frames_per_sec = 24;

  auto pid = ec->player().get_id();
  auto p = ec->player().get(pid);
  if (p.energy == 0) {
    process_starvation(ec);
    set_sit_animation(ec);
    return;
  }

  auto ms = ec->current_millis();
  p.energy -= energy_lost_per_sec * ms / 1000.f;
  ec->player().set(pid, p);

  auto fps = static_cast<unsigned>(frames_per_sec * p.energy);
  update_compo(ec, s,
               {
                   .y = 4,
                   .num_frames = num_frames,
                   .frames_per_sec = fps,
               });
}
void set_pick_animation(compos *ec) {
  constexpr const auto num_frames = 12;
  constexpr const auto frames_per_sec = 12;

  auto pid = ec->player().get_id();
  auto s = ec->player().get(pid).side;

  update_compo(ec, s,
               {
                   .y = 18,
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

  auto p = ec->player().get(pid);

  if (v == 0 && h == 0) {
    if (p.energy < 1) {
      auto delta = food_lost_per_sec * ec->current_millis() / 1000.f;
      delta = (delta > p.satiation) ? p.satiation : delta;
      p.satiation -= delta;
      p.energy += delta * food_energy_ratio;
      ec->player().set(pid, p);
    }
    ec->movements().update(pid, {});
    return;
  }

  float f_speed = speed * p.energy;

  float d = sqrtf(h * h + v * v);
  float sx = h * f_speed / d;
  float sy = v * f_speed / d;

  ec->movements().update(pid, {sx, sy});
}

export rect get_area(compos *ec) {
  auto pid = ec->player().get_id();
  auto spr = ec->player_sprites().get(pid);
  return spr.pos;
}
} // namespace player
