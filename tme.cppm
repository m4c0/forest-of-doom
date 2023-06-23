export module main;

import cursor;
import casein;
import ecs;
import qsu;
import tiles;
import tilemap;

class game {
  static constexpr const auto step = 1.0f;

  ecs::ec m_ec{};
  qsu::main *m_q{};
  tilemap::map m_map{1};

  float m_cx = tilemap::width / 2.0f;
  float m_cy = tilemap::height / 2.0f;

  void set_center() { m_q->center_at(m_cx, m_cy); }

public:
  void setup(qsu::main *q) {
    m_q = q;

    cursor::add_entity(&m_ec.e, &m_ec.cursor, &m_ec.sprites);

    m_map.add_island(0, 0);
    m_map.add_island(4, 4);
    m_map.add_entities({&m_ec.e, &m_ec.sprites}, &m_ec.chunks, 0, 0);

    q->fill_sprites(m_ec.sprites);
    q->sort_sprites(m_ec.sprites);
    q->fill_ui_sprites(m_ec.ui_sprites);
    set_center();
  }

  void down() {
    m_cy += step;
    set_center();
  }
  void up() {
    m_cy -= step;
    set_center();
  }
  void left() {
    m_cx += step;
    set_center();
  }
  void right() {
    m_cx -= step;
    set_center();
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
    m_map.add_island(x, y);
    m_map.add_entities({&m_ec.e, &m_ec.sprites}, &m_ec.chunks, 0, 0);

    cursor::update(&m_ec.cursor, &m_ec.sprites, x, y);
    m_q->fill_sprites(m_ec.sprites);
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu::main q{};
  static game gg{};

  static constexpr const auto k_map = [] {
    casein::key_map res{};
    res[casein::K_DOWN] = [](auto) { gg.down(); };
    res[casein::K_LEFT] = [](auto) { gg.left(); };
    res[casein::K_RIGHT] = [](auto) { gg.right(); };
    res[casein::K_UP] = [](auto) { gg.up(); };
    return res;
  }();
  static constexpr const auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.setup(&q); };
    res[casein::KEY_DOWN] = [](auto e) { k_map.handle(e); };
    res[casein::MOUSE_MOVE] = [](auto e) { gg.mouse_moved(); };
    res[casein::MOUSE_DOWN] = [](auto e) { gg.mouse_down(); };
    return res;
  }();

  q.process_event(e);
  map.handle(e);
}
