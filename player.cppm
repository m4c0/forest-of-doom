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

  ranged energy;
  pog::eid happyness;
  pog::eid health;
  ranged satiation;
};

export class compos : public virtual animation::compos,
                      public virtual gauge::compos,
                      public virtual movement::compos,
                      public virtual pog::entity_provider,
                      public virtual stopwatch {
  c m_player;
  sprite::compo m_player_sprites{};

public:
  c &player() noexcept { return m_player; }
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
  ec->player().eid = pid;
  ec->player().happyness = gauge::add_gauge(ec);
  ec->player().health = gauge::add_gauge(ec);
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

export ranged get_satiation(compos *ec) { return ec->player().satiation; }
export ranged get_energy(compos *ec) { return ec->player().energy; }

auto get_side(compos *ec) { return ec->player().side; }
void set_side(compos *ec, side s) { ec->player().side = s; }

void starve(compos *ec, float val_per_sec) {
  gauge::add_drain(ec, ec->player().health, val_per_sec);
}
void depress(compos *ec, float val_per_sec) {
  gauge::add_drain(ec, ec->player().happyness, val_per_sec);
}
void exercise(compos *ec, float val_per_sec) {
  ec->player().energy -= val_per_sec * ec->current_millis() / 1000.f;
}
void rest(compos *ec, float val_per_sec) { exercise(ec, -val_per_sec); }
bool burn_callories(compos *ec, float val_per_sec) {
  ec->player().satiation -= val_per_sec * ec->current_millis() / 1000.0f;
  return ec->player().satiation > 0;
}

bool update_animation(compos *ec, unsigned s, animation::c a) {
  a.start_x = s * a.num_frames;

  auto pid = ec->player().eid;

  auto cur_a = ec->animations().get(pid);
  if (a.start_x == cur_a.start_x && a.y == cur_a.y)
    return false;

  ec->animations().update(pid, a);
  return true;
}
void update_animation(compos *ec, side s, animation::c a) {
  if (update_animation(ec, static_cast<unsigned>(s), a))
    set_side(ec, s);
}

void process_starvation(compos *ec) {
  auto satiation = ec->player().satiation;
  if (satiation > starvation_limit)
    return;

  auto adj_food = 1.0f - (satiation - starvation_limit) / starvation_limit;
  depress(ec, adj_food * starvation_mental_loss_per_sec);
  starve(ec, adj_food * starvation_health_loss_per_sec);
}

void set_idle_animation(compos *ec) {
  constexpr const animation::c a{
      .y = 2,
      .num_frames = 6,
      .frames_per_sec = 6,
  };
  update_animation(ec, get_side(ec), a);
}
void set_sit_animation(compos *ec) {
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

  auto energy = ec->player().energy;
  if (energy == 0) {
    process_starvation(ec);
    set_sit_animation(ec);
    return;
  }

  exercise(ec, energy_lost_per_sec);

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

export void process_input(input::dual_axis in, compos *ec) {
  constexpr const auto blocks_per_sec = 4.0f;
  constexpr const auto speed = blocks_per_sec / 1000.0f;

  auto h = in.h_value();
  auto v = in.v_value();
  if (v != 0) {
    set_walk_animation(ec, v > 0 ? p_down : p_up);
  } else if (h != 0) {
    set_walk_animation(ec, h > 0 ? p_right : p_left);
  } else {
    set_idle_animation(ec);
  }

  auto pid = ec->player().eid;

  if (v == 0 && h == 0) {
    if (ec->player().energy < 1) {
      if (burn_callories(ec, food_lost_per_sec))
        rest(ec, energy_gain_per_sec);
    }
    ec->movements().update(pid, {});
    return;
  }

  float f_speed = speed * ec->player().energy;

  float d = sqrtf(h * h + v * v);
  float sx = h * f_speed / d;
  float sy = v * f_speed / d;

  ec->movements().update(pid, {sx, sy});
}
} // namespace player
