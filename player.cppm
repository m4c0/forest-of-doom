export module player;
import animation;
import collision;
import gauge;
import input;
import movement;
import ranged;
import pog;
import sprite;
import stopwatch;

extern "C" float sqrtf(float);

namespace player {
constexpr const auto energy_lost_per_sec = 0.25f;
constexpr const auto food_lost_per_sec = 0.1f;
constexpr const auto energy_gain_per_sec = food_lost_per_sec * 2.0f;

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
  pog::eid eid;
  side side;

  pog::eid energy;
  pog::eid happyness;
  pog::eid health;
  pog::eid satiation;
};

export class compos : public virtual animation::compos,
                      public virtual gauge::compos,
                      public virtual movement::compos,
                      public virtual pog::entity_provider,
                      public virtual stopwatch {
  c m_player;
  sprite::compo m_player_sprites{};
  input::state m_input{};

public:
  c &player() noexcept { return m_player; }
  sprite::compo &player_sprites() noexcept { return m_player_sprites; }
  input::state &input() noexcept { return m_input; }
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
  ec->player() = c{
      .eid = pid,
      .energy = gauge::add_gauge(ec),
      .happyness = gauge::add_gauge(ec),
      .health = gauge::add_gauge(ec),
      .satiation = gauge::add_gauge(ec),
  };
  ec->player_sprites().add(pid, spr);
  ec->animations().add(pid, a);
  ec->movements().add(pid, {});
  collision::add(ec, pid, sx, sy + 0.9f, 1, 1);
}

export rect get_area(compos *ec) {
  auto pid = ec->player().eid;
  auto spr = ec->player_sprites().get(pid);
  return spr.pos;
}

auto get_side(compos *ec) { return ec->player().side; }
void set_side(compos *ec, side s) { ec->player().side = s; }

void starve(compos *ec, float val_per_sec) {
  gauge::add_drain(ec, ec->player().health, val_per_sec);
}
void depress(compos *ec, float val_per_sec) {
  gauge::add_drain(ec, ec->player().happyness, val_per_sec);
}
void exercise(compos *ec, float val_per_sec) {
  gauge::add_drain(ec, ec->player().energy, val_per_sec);
}
void rest(compos *ec, float val_per_sec) { exercise(ec, -val_per_sec); }
void burn_callories(compos *ec, float val_per_sec) {
  gauge::add_drain(ec, ec->player().satiation, val_per_sec);
}

bool update_animation(compos *ec, unsigned s, animation::c a) {
  a.start_x = s * a.num_frames;

  auto pid = ec->player().eid;

  auto cur_a = ec->animations().get(pid);
  if (a.start_x == cur_a.start_x && a.y == cur_a.y) {
    cur_a.frames_per_sec = a.frames_per_sec;
    ec->animations().update(pid, cur_a);
    return false;
  }

  ec->animations().update(pid, a);
  return true;
}
void update_animation(compos *ec, side s, animation::c a) {
  if (update_animation(ec, static_cast<unsigned>(s), a))
    set_side(ec, s);
}

void set_idle_animation(compos *ec, side s) {
  constexpr const animation::c a{
      .y = 2,
      .num_frames = 6,
      .frames_per_sec = 6,
  };
  update_animation(ec, s, a);
}
void set_sit_animation(compos *ec) {
  // TODO: realign "sitting" because he is actually floating
  constexpr const animation::c a{
      .y = 8,
      .num_frames = 6,
      .frames_per_sec = 6,
  };
  update_animation(ec, get_side(ec) / 2U, a);
}
void set_walk_animation(compos *ec, side s) {
  constexpr const animation::c a{
      .y = 4,
      .num_frames = 6,
      .frames_per_sec = 24,
  };

  auto energy = ec->gauges.get(ec->player().energy).value;
  auto aa = a;
  aa.frames_per_sec = static_cast<unsigned>(a.frames_per_sec * energy);
  update_animation(ec, s, aa);
}
void set_pick_animation(compos *ec) {
  constexpr const animation::c a{
      .y = 18,
      .num_frames = 12,
      .frames_per_sec = 12,
  };
  update_animation(ec, get_side(ec), a);
}

export void tick(compos *ec) {
  constexpr const auto blocks_per_sec = 4.0f;
  constexpr const auto speed = blocks_per_sec / 1000.0f;
  const auto pid = ec->player().eid;
  const auto energy = ec->gauges.get(ec->player().energy).value;
  const auto satiation = ec->gauges.get(ec->player().satiation).value;

  if (ec->input().rest()) {
    if (energy < 1 && satiation > 0) {
      burn_callories(ec, food_lost_per_sec);
      rest(ec, energy_gain_per_sec);
    }
    set_sit_animation(ec);
    ec->movements().update(pid, {});
    return;
  }

  auto h = ec->input().h_value();
  auto v = ec->input().v_value();
  auto s = ec->player().side;
  if (v != 0) {
    s = v > 0 ? p_down : p_up;
  } else if (h != 0) {
    s = h > 0 ? p_right : p_left;
  } else {
    set_idle_animation(ec, s);
    ec->movements().update(pid, {});
    return;
  }

  if (energy == 0 && satiation < starvation_limit) {
    auto adj_food = 1.0f - (satiation - starvation_limit) / starvation_limit;
    depress(ec, adj_food * starvation_mental_loss_per_sec);
    starve(ec, adj_food * starvation_health_loss_per_sec);
  }
  if (energy == 0) {
    // TODO: use "hurt" animation or something
    set_idle_animation(ec, s);
    ec->movements().update(pid, {});
    return;
  }

  float f_speed = speed * energy;
  float d = sqrtf(h * h + v * v);
  float sx = h * f_speed / d;
  float sy = v * f_speed / d;
  set_walk_animation(ec, s);
  exercise(ec, energy_lost_per_sec);

  ec->collisions.remove(pid);
  ec->movements().update(pid, {sx, sy});
}
} // namespace player
