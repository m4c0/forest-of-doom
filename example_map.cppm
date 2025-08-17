export module example_map;
import backpack;
import collision;
import dotz;
import fox;
import jute;
import loots;
import player;
import prefabs;

namespace example_map {
  void load_prefab(fox::memiter * m, jute::view name) {
    try {
      auto o0 = prefabs::load(name);

      // Add the field with a margin (otherwise we only limit the player from
      // fully leaving the field)
      player::playfield().add_aabb({1}, o0->size() - 1, 'fild', 1);

      o0->for_each([&](float x, float y, const auto & def) {
        if (*def.entry != "start") return;
        player::teleport({ x, y - 1.f }); // compensate player height
      });

      o0->for_each([&](float x, float y, const auto & def) {
        *m += fox::sprite {
          .pos   { x, y },
          .uv    = def.tile.uv,
          .size  = def.tile.size,
          .texid = static_cast<fox::texids>(def.tile.texid),
        };
      });
      o0->for_each([&](float x, float y, const auto & def) {
        if (dotz::length(def.collision.zw()) > 0) {
          auto aa = def.collision.xy() + dotz::vec2 { x, y };
          auto bb = aa + def.collision.zw();
          collision::bodies().add_aabb(aa, bb, 'body', 1);
        }

        if (*def.behaviour == "backpack") {
          backpack::add({
            .loot = def.loot,
            .sprite = {
              .pos   { x, y },
              .uv    = def.entity.uv,
              .size  = def.entity.size,
              .texid = static_cast<fox::texids>(def.entity.texid),
            },
          });
        } else if (def.entity.size.x) {
          *m += {
            .pos   { x, y },
            .uv    = def.entity.uv,
            .size  = def.entity.size,
            .texid = static_cast<fox::texids>(def.entity.texid),
          };
        }
      });
    } catch (...) {
    }
  }

  export void load(fox::memiter * m) {
    load_prefab(m, "prefabs-plains-0.lsp");
  }
}
