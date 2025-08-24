module fod;
import backpack;
import collision;
import dotz;
import exit;
import fox;
import input;
import jute;
import player;
import prefabs;
import silog;

static void load_prefab(jute::view file, jute::view entry) {
  auto o0 = prefabs::load(file);

  fox::g->load(fox::layers::floor, [=](auto * m) {
    // Add the field with a margin (otherwise we only limit the player from
    // fully leaving the field)
    player::playfield().add_aabb({1}, o0->size() - 1, 'fild', 1);

    o0->for_each([&](float x, float y, const auto & def) {
      static constexpr const auto player_h = 1.f;

      if (*def.entry == entry) player::teleport({ x, y - player_h });

      if (def.exit.file.size()) exits::add({
        .pos   { x, y },
        .file  = def.exit.file,
        .entry = def.exit.entry,
      });
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
  });
  fox::g->load(fox::layers::over, [=](auto * m) {
    o0->for_each([&](float x, float y, const auto & def) {
      if (def.over.size.x) {
        *m += {
          .pos   { x, y },
          .uv    = def.over.uv,
          .size  = def.over.size,
          .texid = static_cast<fox::texids>(def.over.texid),
        };
      }
    });
  });
}

void fod::exit(jute::view file, jute::view entry) {
  silog::log(silog::info, "Loading %s entering from %s", file.begin(), entry.begin());

  input::reset();
  fod::on_frame = [](auto) {};

  backpack::purge();
  collision::purge_all();
  exits::purge();
  player::purge();

  try {
    load_prefab(file, entry);
    fod::poc();
  } catch (...) {
  }
}
