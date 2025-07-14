export module player;
import area;
import collision;
import dotz;
import fox;
import gauge;
import input;
import ranged;
import pog;
import sprite;

namespace player {
  static constexpr const auto energy_lost_per_sec = 0.025f;
  static constexpr const auto food_lost_per_sec = 0.1f;
  static constexpr const auto energy_gain_per_sec = food_lost_per_sec * 2.0f;

  static constexpr const auto starvation_limit = 0.25f;
  static constexpr const auto starvation_mental_loss_per_sec = 0.1f;
  static constexpr const auto starvation_health_loss_per_sec = 0.1f;

  struct anim {
    unsigned start_x;
    unsigned y;
    unsigned num_frames;
    unsigned frames_per_sec;
    unsigned frame = 0;
  };

  enum side {
    p_right = 0,
    p_up,
    p_left,
    p_down,
  };

  struct state {
    fox::sprite sprite {
      .pos { 8, 8 },
      .size { 1, 2 },
      .texid = 2,
    };
    anim anim;
    side side;
  } g_state;
  
  export void load(fox::memiter * m) {
    *m += g_state.sprite;
  }

export struct c {
  pog::eid eid;

  pog::eid energy;
  pog::eid happyness;
  pog::eid health;
  pog::eid satiation;
};

export class compos : public virtual gauge::compos,
                      public virtual collision::compos,
                      public virtual pog::entity_provider,
                      public virtual sprite::compos {
  c m_player;

public:
  c &player() noexcept { return m_player; }
};

export void add_entity(compos *ec) {
  constexpr const auto sx = 8.0f;
  constexpr const auto sy = 8.0f;

  rect pos{sx, sy, 1, 2};
  sprite::c spr{
      .uv = {0, 2, 1, 2},
      .layer = sprite::layers::scout,
      .dim = 1,
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
  g_state.anim = a;
  collision::add(ec, pid, sx, sy + 0.9f, 1, 1);
}

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

  if (a.start_x == g_state.anim.start_x && a.y == g_state.anim.y) {
    g_state.anim.frames_per_sec = a.frames_per_sec;
    return false;
  }

  g_state.anim = a;
  return true;
}
void update_animation(compos *ec, side s, anim a) {
  if (update_animation(ec, static_cast<unsigned>(s), a))
    g_state.side = s;
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
  update_animation(ec, g_state.side / 2U, a);
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
  update_animation(ec, g_state.side, a);
}

void update_anims(compos *ec, float ms) {
  g_state.anim.frame += ms * g_state.anim.frames_per_sec;

  auto frame = g_state.anim.frame / 1000;

  auto u = g_state.anim.start_x + (frame % g_state.anim.num_frames);
  g_state.sprite.uv = { u, g_state.anim.y };
}
void update_sprites(compos *c, float sx, float sy, float ms) {
  auto dx = sx * ms;
  auto dy = sy * ms;

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

  g_state.sprite.pos = g_state.sprite.pos + dotz::vec2 { dx, dy };
}
export void tick(compos *ec, float ms) {
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
    update_anims(ec, ms);
    return;
  }

  auto h = input::state(input::axis::X);
  auto v = input::state(input::axis::Y);
  auto s = g_state.side;
  if (v != 0) {
    s = v > 0 ? p_down : p_up;
  } else if (h != 0) {
    s = h > 0 ? p_right : p_left;
  } else {
    set_idle_animation(ec, s);
    update_anims(ec, ms);
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
    update_anims(ec, ms);
    return;
  }

  float f_speed = speed * energy;
  float d = dotz::sqrt(h * h + v * v);
  float sx = h * f_speed / d;
  float sy = v * f_speed / d;
  set_walk_animation(ec, s);
  exercise(ec, energy_lost_per_sec);

  ec->collisions.remove(pid);
  update_anims(ec, ms);
  update_sprites(ec, sx, sy, ms);
}

  export dotz::vec2 center() {
    return g_state.sprite.pos + g_state.sprite.size / 2.0;
  }
} // namespace player
