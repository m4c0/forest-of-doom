export module main;

import cursor;
import casein;
import ecs;
import qsu;
import tile;
import tiles;
import tilemap;

class game {
  static constexpr const auto step = 1.0f;

  ecs::ec m_ec{};
  qsu::main *m_q{};
  tilemap::map m_map{1};
  tile m_brush{};

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
  }

  void mouse_moved() {
    auto [x, y] = m_q->mouse_pos();
    cursor::update(&m_ec.cursor, &m_ec.sprites, x, y);
    m_q->fill_sprites(m_ec.sprites);
  }
  void mouse_down() {
    m_ec.chunks.remove_if([this](auto cid, auto eid) {
      if (cid != 1)
        return false;

      tiles::remove_tile(eid, {&m_ec.e, &m_ec.sprites});
      return true;
    });

    auto [x, y] = m_q->mouse_pos();
    m_map.set(x, y, m_brush);
    m_map.add_entities({&m_ec.e, &m_ec.sprites}, &m_ec.chunks, 0, 0);

    cursor::update(&m_ec.cursor, &m_ec.sprites, x, y);
    sort_sprites(m_ec.sprites);
    m_q->fill_sprites(m_ec.sprites);
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu::main q{};
  static game gg{};

  static constexpr const auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.setup(&q); };
    res[casein::KEY_DOWN] = [](auto e) { gg.set_brush(grass_1); };
    res[casein::MOUSE_MOVE] = [](auto e) { gg.mouse_moved(); };
    res[casein::MOUSE_DOWN] = [](auto e) { gg.mouse_down(); };
    return res;
  }();

  q.process_event(e);
  map.handle(e);
}
