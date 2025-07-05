export module qsu:layer;
import :atlas;
import dotz;
import jute;
import pog;
import quack;
import silog;
import sprite;
import vee;
import voo;

namespace qsu {
class layer : voo::update_thread {
  static constexpr const auto sprite_sz = 16.0f;

  atlas m_atlas;
  quack::instance_batch m_spr;
  sprite::layers m_layer;
  unsigned m_count{};
  unsigned m_max_sprites{};

  void build_cmd_buf(vee::command_buffer cb) override {
    voo::cmd_buf_one_time_submit pcb{cb};
    m_spr.setup_copy(cb);
  }

public:
  layer(voo::device_and_queue &dq, quack::pipeline_stuff &ps, unsigned max_spr,
        sprite::layers l, jute::view atlas) :
    update_thread(&dq),
    m_atlas { &dq, &ps, atlas },
    m_spr { ps.create_batch(max_spr) },
    m_layer { l }
  {
    m_atlas.run_once();
  }
  ~layer() {
    silog::log(silog::info, "[qsu] %d sprites for %s", m_max_sprites,
               m_atlas.name().cstr().data());
  }

  void run(quack::pipeline_stuff &ps, vee::command_buffer cb) {
    m_spr.build_commands(cb);
    ps.cmd_bind_descriptor_set(cb, m_atlas.descriptor_set());
    ps.run(cb, m_count);
  }

  void fill(sprite::compos *ec, dotz::vec2 center) {
    auto [px, py] = center;
    pog::aabb area{{px - 32, py - 32}, {px + 32, py + 32}};

    m_spr.map_all([&](auto all) {
      auto cs = all.colours;
      auto ms = all.multipliers;
      auto ps = all.positions;
      auto us = all.uvs;

      float atlas_w = m_atlas.width();
      float atlas_h = m_atlas.height();

      m_count = 0;
      ec->areas().for_each_in(area, [&](auto id, auto aabb) {
        auto spr = ec->sprites.get(id);
        if (spr.layer != m_layer)
          return;

        *cs++ = {spr.colour.r, spr.colour.g, spr.colour.b, spr.colour.a};
        *ms++ = {1, 1, 1, 1.0f - spr.dim};

        *ps++ = quack::rect{aabb.a.x, aabb.a.y, aabb.b.x - aabb.a.x,
                            aabb.b.y - aabb.a.y};

        *us++ = quack::uv{
            spr.uv.x * sprite_sz / atlas_w,
            spr.uv.y * sprite_sz / atlas_h,
            (spr.uv.x + spr.uv.w) * sprite_sz / atlas_w,
            (spr.uv.y + spr.uv.h) * sprite_sz / atlas_h,
        };

        m_count++;
      });

      if (m_count > m_max_sprites)
        m_max_sprites = m_count;
    });

    run_once();
  }
};
} // namespace qsu
