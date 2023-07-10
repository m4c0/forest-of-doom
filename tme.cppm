export module main;

import casein;
import chunk;
import collision;
import cursor;
import jute;
import pog;
import prefabs;
import qsu;
import silog;
import sprite;
import tile;
import tiles;
import tilemap;
import yoyo_libc;

template <unsigned N> class palette {
  tile::camping::c m_tiles[N];
  unsigned m_index{};

public:
  constexpr palette(auto... n) : m_tiles{n...} {}

  [[nodiscard]] constexpr auto get() const noexcept { return m_tiles[m_index]; }

  void operator++() noexcept { m_index = (m_index + 1) % N; }
  void operator--() noexcept { m_index = (m_index + N - 1) % N; }
};

static void fail(const char *msg) {
  silog::log(silog::error, "Error: %s", msg);
  throw 0;
}

class ec : public cursor::compos, public tilemap::compos {
  pog::entity_list m_e{};

  chunk::compo m_chunks{};
  collision::compo m_bodies{};
  cursor::compo m_cursor{};
  sprite::compo m_sprites{};
  tile::camping::compo m_tiles{};

public:
  pog::entity_list &e() noexcept override { return m_e; }

  chunk::compo &chunks() noexcept override { return m_chunks; }
  collision::compo &bodies() noexcept override { return m_bodies; }
  cursor::compo &cursor() noexcept override { return m_cursor; }
  sprite::compo &sprites() noexcept override { return m_sprites; }
};

class game {
  static constexpr const auto prefab = prefabs::ocean_0;
  static constexpr const auto fname = "prefabs-ocean_0.cppm";
  static constexpr const auto mname = "ocean_0";

  ec m_ec{};
  qsu::main *m_q{};
  tilemap::map m_map = prefab;
  tilemap::map m_undo_map = prefab;
  tile::camping::c m_brush{};

  palette<8> m_island_pal{tile::camping::island_tl, tile::camping::island_t,
                          tile::camping::island_tr, tile::camping::island_r,
                          tile::camping::island_br, tile::camping::island_b,
                          tile::camping::island_bl, tile::camping::island_l};
  palette<8> m_lake_pal{tile::camping::lake_tl, tile::camping::island_b,
                        tile::camping::lake_tr, tile::camping::island_l,
                        tile::camping::lake_br, tile::camping::island_t,
                        tile::camping::lake_bl, tile::camping::island_r};
  palette<3> m_land_pal{tile::camping::grass_0, tile::camping::grass_1,
                        tile::camping::water};

  void update_sprites() {
    m_ec.chunks().remove_if([this](auto cid, auto eid) {
      if (cid != 1)
        return false;

      tiles::remove_tile(eid, &m_ec);
      return true;
    });
    m_map.add_entities(&m_ec, 1, 0, 0);

    sort_sprites(m_ec.sprites());
    m_q->fill_sprites(m_ec.sprites());
  }

  void flood_fill(auto x, auto y, tile::camping::c old) {
    auto _ = m_map.get(x, y).map([this, x, y, old](auto t) {
      if (t != old)
        return;

      m_map.set(x, y, m_brush);
      flood_fill(x + 1, y, old);
      flood_fill(x - 1, y, old);
      flood_fill(x, y + 1, old);
      flood_fill(x, y - 1, old);
    });
  }

public:
  void setup(qsu::main *q) {
    m_q = q;

    q->set_grid(tilemap::width, tilemap::height);
    q->center_at(tilemap::width / 2, tilemap::height / 2);

    cursor::add_entity(&m_ec);
    set_brush(tile::camping::grass_0);
    update_sprites();
  }

  void set_brush(tile::camping::c t) {
    m_brush = t;
    tiles::update_tile(m_ec.cursor().get_id(), &m_ec, t);
    m_q->fill_sprites(m_ec.sprites());
  }

  void next_island_brush() {
    ++m_island_pal;
    set_brush(m_island_pal.get());
  }
  void prev_island_brush() {
    --m_island_pal;
    set_brush(m_island_pal.get());
  }
  void next_lake_brush() {
    ++m_lake_pal;
    set_brush(m_lake_pal.get());
  }
  void prev_lake_brush() {
    --m_lake_pal;
    set_brush(m_lake_pal.get());
  }
  void next_land_brush() {
    ++m_land_pal;
    set_brush(m_land_pal.get());
  }

  void mouse_moved() {
    auto [x, y] = m_q->mouse_pos();
    cursor::update(&m_ec, x, y);
    m_q->fill_sprites(m_ec.sprites());
  }
  void mouse_down() {
    auto [x, y] = m_q->mouse_pos();
    m_undo_map = m_map;
    m_map.set(x, y, m_brush);
    auto [tx, ty, tw, th] = tiles::tile_uv(m_brush);
    for (auto dy = 0; dy < th; dy++) {
      for (auto dx = 0; dx < tw; dx++) {
        if (dx == 0 && dy == 0)
          continue;
        m_map.set(x + dx, y + dy, tile::camping::blank);
      }
    }
    cursor::update(&m_ec, x, y);
    update_sprites();
  }

  void flood_fill() {
    auto [x, y] = m_q->mouse_pos();
    auto _ = m_map.get(x, y).map([this, x, y](auto t) {
      if (t == m_brush)
        return;
      m_undo_map = m_map;
      flood_fill(x, y, t);
      update_sprites();
    });
  }

  void undo() {
    m_map = m_undo_map;
    update_sprites();
  }

  void dump_map() {
    using namespace jute::literals;

    try {
      yoyo::file_writer out{fname};
      out.writef("export module prefabs:%s;\n", mname).take(fail);
      out.write("import tile;\n"_s).take(fail);
      out.write("import tilemap;\n"_s).take(fail);
      out.write("\n"_s).take(fail);
      out.write("namespace prefabs {\n"_s).take(fail);
      out.writef("export constexpr const tilemap::map %s = [] {\n", mname)
          .take(fail);
      out.write("  tilemap::map res{};\n"_s).take(fail);
      for (auto y = 0; y < tilemap::height; y++) {
        for (auto x = 0; x < tilemap::width; x++) {
          m_map.get(x, y)
              .fmap([&](auto t) {
                return out.writef("  res.set(%d, %d, "
                                  "static_cast<tile::camping::c>(0x%08x));\n",
                                  x, y, t);
              })
              .take(fail);
        }
      }
      out.write("  return res;\n"_s).take(fail);
      out.write("}();\n"_s).take(fail);
      out.write("}\n"_s).take(fail);
    } catch (...) {
    }
    silog::log(silog::info, "Source saved");
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu::main q{};
  static game gg{};

  static constexpr const auto kmap = [] {
    casein::key_map res{};
    res[casein::K_Q] = [](auto) { gg.prev_island_brush(); };
    res[casein::K_W] = [](auto) { gg.next_island_brush(); };
    res[casein::K_A] = [](auto) { gg.prev_lake_brush(); };
    res[casein::K_S] = [](auto) { gg.next_lake_brush(); };
    res[casein::K_E] = [](auto) { gg.next_land_brush(); };
    res[casein::K_D] = [](auto) { gg.flood_fill(); };
    res[casein::K_U] = [](auto) { gg.undo(); };
    res[casein::K_Z] = [](auto) { gg.dump_map(); };
    res[casein::K_SPACE] = [](auto) { gg.set_brush(tile::camping::blank); };
    return res;
  }();
  static constexpr const auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.setup(&q); };
    res[casein::KEY_DOWN] = [](auto e) { kmap.handle(e); };
    res[casein::MOUSE_MOVE] = [](auto e) { gg.mouse_moved(); };
    res[casein::MOUSE_DOWN] = [](auto e) { gg.mouse_down(); };
    return res;
  }();

  q.process_event(e);
  map.handle(e);
}
