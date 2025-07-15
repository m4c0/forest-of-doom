export module player;
import area;
import collision;
import dotz;
import fox;
import input;
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
    unsigned start_x = 0;
    unsigned y = 0;
    unsigned num_frames = 1;
    unsigned frames_per_sec = 0;
    unsigned frame = 0;
  };

  enum side : unsigned {
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
    anim anim {};
    side side;
    float energy = 1;
    float happyness = 1;
    float health = 1;
    float satiation = 1;

    pog::eid eid;
  } g_state;
  
  export void load(fox::memiter * m) {
    *m += g_state.sprite;
  }

export class compos : public virtual collision::compos,
                      public virtual pog::entity_provider {};

export void add_entity(compos *ec) {
  auto pid = g_state.eid = ec->e().alloc();
  collision::add(ec, pid, g_state.sprite.pos.x, g_state.sprite.pos.y + 0.9f, 1, 1);
}

  static void drain(float & f, float vps, float ms) {
    f = dotz::clamp(f - vps * ms / 1000.0f, 0.f, 1.f);
  }
  static void starve(float vps, float ms) { drain(g_state.health, vps, ms); }
  static void depress(float vps, float ms) { drain(g_state.happyness, vps, ms); }
  static void exercise(float vps, float ms) { drain(g_state.energy, vps, ms); }
  static void rest(float vps, float ms) { exercise(-vps, ms); }
  static void burn_callories(float vps, float ms) { drain(g_state.satiation, vps, ms); }

  static void tick_animation(float ms) {
    g_state.anim.frame += ms * g_state.anim.frames_per_sec;
  
    auto frame = g_state.anim.frame / 1000;
  
    auto u = g_state.anim.start_x + (frame % g_state.anim.num_frames);
    g_state.sprite.uv = { u, g_state.anim.y };
  }

  static bool set_animation(float ms, unsigned s, anim a) {
    a.start_x = s * a.num_frames;

    if (a.start_x == g_state.anim.start_x && a.y == g_state.anim.y) {
      g_state.anim.frames_per_sec = a.frames_per_sec;
      tick_animation(ms);
      return false;
    }

    g_state.anim = a;
    return true;
  }
  static void set_animation(float ms, side s, anim a) {
    if (set_animation(ms, static_cast<unsigned>(s), a)) g_state.side = s;
  }
  static void idle_animation(float ms, side s) {
    set_animation(ms, s, {
      .y = 2,
      .num_frames = 6,
      .frames_per_sec = 6,
    });
  }
  static void sit_animation(float ms) {
    // TODO: realign "sitting" because he is actually floating
    set_animation(ms, g_state.side / 2U, {
      .y = 8,
      .num_frames = 6,
      .frames_per_sec = 6,
    });
  }
  static void walk_animation(float ms, side s) {
    set_animation(ms, s, {
      .y = 4,
      .num_frames = 6,
      .frames_per_sec = static_cast<unsigned>(24 * g_state.energy),
    });
  }

  static void rest(float ms) {
    if (g_state.energy < 1 && g_state.satiation > 0) {
      burn_callories(food_lost_per_sec, ms);
      rest(energy_gain_per_sec, ms);
    }
    sit_animation(ms);
  }
export void tick(compos *ec, float ms) {
  constexpr const auto blocks_per_sec = 4.0f;
  constexpr const auto speed = blocks_per_sec / 1000.0f;

  if (input::state(input::buttons::REST)) return rest(ms);

  dotz::vec2 in {
    input::state(input::axis::X),
    input::state(input::axis::Y),
  };
  auto s = g_state.side;
  if (in.y != 0) {
    s = in.y > 0 ? p_down : p_up;
  } else if (in.x != 0) {
    s = in.x > 0 ? p_right : p_left;
  } else {
    idle_animation(ms, s);
    return;
  }

  if (g_state.energy == 0 && g_state.satiation < starvation_limit) {
    auto adj_food = 1.0f - (g_state.satiation - starvation_limit) / starvation_limit;
    depress(adj_food * starvation_mental_loss_per_sec, ms);
    starve(adj_food * starvation_health_loss_per_sec, ms);
  }
  if (g_state.energy == 0) {
    // TODO: use "hurt" animation or something
    idle_animation(ms, s);
    return;
  }

  exercise(energy_lost_per_sec, ms);
  walk_animation(ms, s);

  ec->collisions.remove(g_state.eid);

  auto d = dotz::normalise(in) * ms * speed * g_state.energy;
  if (collision::move_by(ec, g_state.eid, d.x, d.y)) {
  } else if (collision::move_by(ec, g_state.eid, 0, d.y)) {
    d.x = 0;
  } else if (collision::move_by(ec, g_state.eid, d.x, 0)) {
    d.y = 0;
  } else {
    // TODO: do something
    return;
  }

  g_state.sprite.pos = g_state.sprite.pos + d;
}

  export dotz::vec2 center() {
    return g_state.sprite.pos + g_state.sprite.size / 2.0;
  }
} // namespace player
