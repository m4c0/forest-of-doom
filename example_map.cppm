export module example_map;
import backpack;
import collision;
import dotz;
import fox;
import jute;
import prefabs;

namespace example_map {
  void load_prefab(fox::memiter * m, jute::view name, int dx, int dy) {
    try {
      auto o0 = prefabs::load(name);
      o0->for_each([&](float x, float y, const auto & def) {
        *m += fox::sprite {
          .pos   { dx + x, dy + y },
          .uv    = def.tile.uv,
          .size  = def.tile.size,
          .texid = static_cast<fox::texids>(def.tile.texid),
        };
      });
      o0->for_each([&](float x, float y, const auto & def) {
        if (dotz::length(def.collision.zw()) > 0) {
          auto aa = def.collision.xy() + dotz::vec2 { dx + x, dy + y };
          auto bb = aa + def.collision.zw();
          collision::bodies().add_aabb(aa, bb, 'body', 1);
        }
  
        if (*def.behaviour == "backpack") backpack::add(fox::sprite {
          .pos   { dx + x, dy + y },
          .uv    = def.entity.uv,
          .size  = def.entity.size,
          .texid = static_cast<fox::texids>(def.entity.texid),
        });
      });
    } catch (...) {
    }
  }

  export void load(fox::memiter * m) {
    load_prefab(m, "prefabs-ocean-0.lsp", -16, -16);
    load_prefab(m, "prefabs-ocean-0.lsp",   0, -16);
    load_prefab(m, "prefabs-ocean-0.lsp",  16, -16);
    load_prefab(m, "prefabs-ocean-0.lsp", -16,   0);
    load_prefab(m, "prefabs-ocean-0.lsp",  16,   0);
    load_prefab(m, "prefabs-ocean-0.lsp", -16,  16);
    load_prefab(m, "prefabs-ocean-0.lsp",   0,  16);
    load_prefab(m, "prefabs-ocean-0.lsp",  16,  16);
  
    load_prefab(m, "prefabs-island-0.lsp", 0, 0);
  }
}
