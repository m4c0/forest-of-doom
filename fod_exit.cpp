module fod;
import backpack;
import collision;
import drops;
import dotz;
import exit;
import fox;
import input;
import jute;
import pathing;
import player;
import prefabs;
import silog;

static pathing::t g_pathing;
static hai::sptr<pathing::from> g_current;

static void load_prefab(prefabs::tilemap o0) {
  auto entry = g_current->entry;

  fox::g->load(fox::layers::floor, [&](auto * m) {
    // Add the field with a margin (otherwise we only limit the player from
    // fully leaving the field)
    player::playfield().add_aabb({1}, o0.size() - 1, 'fild', 1);

    o0.for_each([&](float x, float y, const auto & def) {
      static constexpr const auto player_h = 1.f;

      if (*def.entry == entry) player::teleport({ x, y - player_h });

      if (def.exit.size()) exits::add({
        .pos   { x, y },
        .name  = def.exit,
      });
    });

    o0.for_each([&](float x, float y, const auto & def) {
      *m += fox::sprite {
        .pos   { x, y },
        .uv    = def.tile.uv,
        .size  = def.tile.size,
        .texid = static_cast<fox::texids>(def.tile.texid),
      };
    });
    o0.for_each([&](float x, float y, const auto & def) {
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
  fox::g->load(fox::layers::over, [&](auto * m) {
    o0.for_each([&](float x, float y, const auto & def) {
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

  fod::poc();
}

static void load() {
  input::reset();
  fod::on_frame = [](auto) {};

  backpack::purge();
  collision::purge_all();
  drops::purge();
  exits::purge();
  player::purge();

  auto uid = g_current->uniqueid;
  if (uid != "") silog::log(silog::warning, "TBD: load uniques");

  auto file = g_current->file;
  auto entry = g_current->entry;
  silog::log(silog::info, "Loading %.*s entering from %.*s",
      static_cast<int>(file.size()), file.begin(),
      static_cast<int>(entry.size()), entry.begin());

  prefabs::load(file, load_prefab);
}

void fod::exit() {
  g_pathing = pathing::load();
  g_current = g_pathing.froms[g_pathing.start];
  if (!g_current) silog::die("Missing target [%s]", g_pathing.start.cstr().begin());
  load();
}

void fod::exit(jute::view thru) {
  auto to = g_current->exits[thru];
  if (to == "") silog::die("Target %s does not exist from %s", to.cstr().begin(), g_current->file.cstr().begin());

  auto uid = g_current->uniqueid;
  if (uid != "") silog::log(silog::warning, "TBD: save uniques");

  g_current = g_pathing.froms[to];
  if (!g_current) silog::die("Missing target [%s]", to.cstr().begin());
  load();
}
