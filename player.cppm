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
export struct anim {
  unsigned start_x;
  unsigned y;
  unsigned num_frames;
  unsigned ticks{};
};
export struct c {
  side side;
  anim anim;
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
      .uv = {0, 2, 1, 2},
  };
  anim a{
      .start_x = 0,
      .y = 0,
      .num_frames = 1,
  };
  auto pid = ec->e().alloc();
  ec->player().set(pid, {.anim = a});
  ec->player_sprites().add(pid, spr);
}

void update_sprite(pog::eid pid, const c &p, sprite::compo &sprites) {
  constexpr const auto ticks_per_frame = 10;

  auto spr = sprites.get(pid);
  spr.uv.x =
      p.anim.start_x + ((p.anim.ticks / ticks_per_frame) % p.anim.num_frames);
  spr.uv.y = p.anim.y;
  sprites.update(pid, spr);
}

export void set_side(compos *ec, side s) {
  auto pid = ec->player().get_id();
  auto p = ec->player().get(pid);
  p.side = s;
  p.anim.start_x = static_cast<unsigned>(p.side) * p.anim.num_frames;
  ec->player().set(pid, p);

  update_sprite(pid, p, ec->player_sprites());
}
export void set_idle_animation(compos *ec) {
  constexpr const auto num_frames = 6;

  auto pid = ec->player().get_id();
  auto p = ec->player().get(pid);
  p.anim = {
      .start_x = static_cast<unsigned>(p.side) * num_frames,
      .y = 2,
      .num_frames = num_frames,
  };
  ec->player().set(pid, p);
}
export void set_walk_animation(compos *ec) {
  constexpr const auto num_frames = 6;

  auto pid = ec->player().get_id();
  auto p = ec->player().get(pid);
  p.anim = {
      .start_x = static_cast<unsigned>(p.side) * num_frames,
      .y = 4,
      .num_frames = num_frames,
  };
  ec->player().set(pid, p);
}

export void update_animation(compos *ec) {
  auto pid = ec->player().get_id();
  auto p = ec->player().get(pid);
  p.anim.ticks++;
  ec->player().set(pid, p);

  update_sprite(pid, p, ec->player_sprites());
}
} // namespace player
