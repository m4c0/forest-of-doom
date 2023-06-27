export module player;
import pog;
import sprite;

namespace player {
export using compo = pog::singleton<>;

export void add_entity(pog::entity_list *e, compo *player,
                       sprite::compo *sprites) {
  sprite spr{
      .pos = {8, 8, 1, 2},
      .uv = {0, 0, 1, 2},
  };
  auto pid = e->alloc();
  player->set(pid, {});
  sprites->add(pid, spr);
}

export void update_sprite(compo *player, sprite::compo *sprites, float x) {
  auto pid = player->get_id();
  auto spr = sprites->get(pid);
  spr.uv.x = x;
  sprites->update(pid, spr);
}
} // namespace player
