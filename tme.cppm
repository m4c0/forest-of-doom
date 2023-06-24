export module main;

import cursor;
import casein;
import ecs;
import qsu;
import tile;
import tiles;
import tilemap;

template <unsigned N> class palette {
  tile m_tiles[N];
  unsigned m_index{};

public:
  constexpr palette(auto... n) : m_tiles{n...} {}

  [[nodiscard]] constexpr auto get() const noexcept { return m_tiles[m_index]; }

  void operator++() noexcept { m_index = (m_index + 1) % N; }
  void operator--() noexcept { m_index = (m_index + N - 1) % N; }
};

class game {
  static constexpr const auto step = 1.0f;
  ecs::ec m_ec{};
  qsu::main *m_q{};
  tilemap::map m_map{};
  tile m_brush{};

  palette<8> m_island_pal{island_tl, island_t, island_tr, island_r,
                          island_br, island_b, island_bl, island_l};
  palette<8> m_lake_pal{lake_tl, island_b, lake_tr, island_l,
                        lake_br, island_t, lake_bl, island_r};
  palette<3> m_land_pal{grass_0, grass_1, water};

  void update_sprites() {
    m_ec.chunks.remove_if([this](auto cid, auto eid) {
      if (cid != 1)
        return false;

      tiles::remove_tile(eid, {&m_ec.e, &m_ec.sprites});
      return true;
    });
    m_map.add_entities({&m_ec.e, &m_ec.sprites}, &m_ec.chunks, 1, 0, 0);

    sort_sprites(m_ec.sprites);
    m_q->fill_sprites(m_ec.sprites);
  }

  void flood_fill(auto x, auto y, tile old) {
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

    q->set_grid(16, 16);
    q->center_at(8, 8);

    cursor::add_entity(&m_ec.e, &m_ec.cursor, &m_ec.sprites);
    set_brush(grass_0);

    q->fill_sprites(m_ec.sprites);
  }

  void set_brush(tile t) {
    m_brush = t;
    tiles::update_tile(m_ec.cursor.get_id(), &m_ec.sprites, t);
    m_q->fill_sprites(m_ec.sprites);
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
    cursor::update(&m_ec.cursor, &m_ec.sprites, x, y);
    m_q->fill_sprites(m_ec.sprites);
  }
  void mouse_down() {
    auto [x, y] = m_q->mouse_pos();
    m_map.set(x, y, m_brush);
    cursor::update(&m_ec.cursor, &m_ec.sprites, x, y);
    update_sprites();
  }

  void flood_fill() {
    auto [x, y] = m_q->mouse_pos();
    auto _ = m_map.get(x, y).map([this, x, y](auto t) {
      flood_fill(x, y, t);
      update_sprites();
    });
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
    res[casein::K_SPACE] = [](auto) { gg.set_brush(blank); };
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
