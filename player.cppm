export module player;
import area;
import collision;
import dotz;
import gauge;
import input;
import ranged;
import pog;
import sprite;
import stopwatch;

namespace player {
constexpr const auto energy_lost_per_sec = 0.025f;
constexpr const auto food_lost_per_sec = 0.1f;
constexpr const auto energy_gain_per_sec = food_lost_per_sec * 2.0f;

constexpr const auto starvation_limit = 0.25f;
constexpr const auto starvation_mental_loss_per_sec = 0.1f;
constexpr const auto starvation_health_loss_per_sec = 0.1f;

export struct anim {
  unsigned start_x;
  unsigned y;
  unsigned num_frames;
  unsigned frames_per_sec;
  unsigned frame{};
};
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

export class compos : public virtual gauge::compos,
                      public virtual collision::compos,
                      public virtual pog::entity_provider,
                      public virtual sprite::compos,
                      public virtual stopwatch {
  c m_player;
  anim m_animation;
  dotz::vec2 m_movement;

public:
  c &player() noexcept { return m_player; }
  anim &animation() noexcept { return m_animation; }
  auto &movement() noexcept { return m_movement; }
};

export void add_entity(compos *ec) {
  constexpr const auto sx = 8.0f;
  constexpr const auto sy = 8.0f;

  rect pos{sx, sy, 1, 2};
  sprite::c spr{
      .uv = {0, 2, 1, 2},
      .layer = sprite::layers::scout,
  };
  anim a{
      .start_x = 0,
      .y = 0,
      .num_frames = 1,
  };
  auto pid = sprite::add(ec, spr, pos);
  ec->player() = c{
      .eid = pid,
      .energy = gauge::add_gauge(ec),
      .happyness = gauge::add_gauge(ec),
      .health = gauge::add_gauge(ec),
      .satiation = gauge::add_gauge(ec),
  };
  ec->animation() = a;
  ec->movement() = {};
  collision::add(ec, pid, sx, sy + 0.9f, 1, 1);
}

export rect get_area(compos *ec) {
  auto pid = ec->player().eid;
  return area::get(ec, pid);
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

bool update_animation(compos *ec, unsigned s, anim a) {
  a.start_x = s * a.num_frames;

  auto cur_a = ec->animation();
  if (a.start_x == cur_a.start_x && a.y == cur_a.y) {
    cur_a.frames_per_sec = a.frames_per_sec;
    ec->animation() = cur_a;
    return false;
  }

  ec->animation() = a;
  return true;
}
void update_animation(compos *ec, side s, anim a) {
  if (update_animation(ec, static_cast<unsigned>(s), a))
    set_side(ec, s);
}

void set_idle_animation(compos *ec, side s) {
  constexpr const anim a{
      .y = 2,
      .num_frames = 6,
      .frames_per_sec = 6,
  };
  update_animation(ec, s, a);
}
void set_sit_animation(compos *ec) {
  // TODO: realign "sitting" because he is actually floating
  constexpr const anim a{
      .y = 8,
      .num_frames = 6,
      .frames_per_sec = 6,
  };
  update_animation(ec, get_side(ec) / 2U, a);
}
void set_walk_animation(compos *ec, side s) {
  constexpr const anim a{
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
  constexpr const anim a{
      .y = 18,
      .num_frames = 12,
      .frames_per_sec = 12,
  };
  update_animation(ec, get_side(ec), a);
}

void update_anims(compos *ec) {
  auto millis = ec->current_millis();
  auto & a = ec->animation();
  a.frame += millis * a.frames_per_sec;

  auto frame = a.frame / 1000;

  auto u = a.start_x + (frame % a.num_frames);
  const auto id = ec->player().eid;
  sprite::set_uv(ec, id, u, a.y);
}
void update_sprites(compos *c) {
  float ms = c->current_millis();
  auto dx = c->movement().x * ms;
  auto dy = c->movement().y * ms;

  const auto id = c->player().eid;

  if (collision::move_by(c, id, dx, dy)) {
  } else if (collision::move_by(c, id, 0, dy)) {
    dx = 0;
  } else if (collision::move_by(c, id, dx, 0)) {
    dy = 0;
  } else {
    // TODO: do something
    return;
  }

  sprite::move_by(c, id, dx, dy);
}
export void tick(compos *ec) {
  constexpr const auto blocks_per_sec = 4.0f;
  constexpr const auto speed = blocks_per_sec / 1000.0f;
  const auto pid = ec->player().eid;
  const auto energy = ec->gauges.get(ec->player().energy).value;
  const auto satiation = ec->gauges.get(ec->player().satiation).value;

  if (input::state(input::buttons::REST)) {
    if (energy < 1 && satiation > 0) {
      burn_callories(ec, food_lost_per_sec);
      rest(ec, energy_gain_per_sec);
    }
    set_sit_animation(ec);
    ec->movement() = {};
    update_anims(ec);
    return;
  }

  auto h = input::state(input::axis::X);
  auto v = input::state(input::axis::Y);
  auto s = ec->player().side;
  if (v != 0) {
    s = v > 0 ? p_down : p_up;
  } else if (h != 0) {
    s = h > 0 ? p_right : p_left;
  } else {
    set_idle_animation(ec, s);
    ec->movement() = {};
    update_anims(ec);
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
    ec->movement() = {};
    update_anims(ec);
    return;
  }

  float f_speed = speed * energy;
  float d = dotz::sqrt(h * h + v * v);
  float sx = h * f_speed / d;
  float sy = v * f_speed / d;
  set_walk_animation(ec, s);
  exercise(ec, energy_lost_per_sec);

  ec->collisions.remove(pid);
  ec->movement() = {sx, sy};
  update_anims(ec);
  update_sprites(ec);
}

export dotz::vec2 center(player::compos * ec) {
  auto area = get_area(ec);
  auto x = area.x + area.w / 2;
  auto y = area.y + area.h / 2;
  return { x, y };
}
} // namespace player
