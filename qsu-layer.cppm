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
static constexpr const unsigned instance_size = sizeof(quack::instance);

class layer : voo::update_thread {
  static constexpr const auto sprite_sz = 16.0f;

  atlas m_atlas;
  voo::h2l_buffer m_spr;
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
    update_thread { dq.queue() },
    m_atlas { &dq, &ps, atlas },
    m_spr { dq.physical_device(), max_spr * instance_size },
    m_layer { l }
  {
    m_atlas.run_once();
  }
  ~layer() {
    silog::log(silog::info, "[qsu] %d sprites for %s", m_max_sprites,
               m_atlas.name().cstr().data());
  }

  void draw(quack::pipeline_stuff & ps, quack::params p) {
    p.inst_buffer = m_spr.local_buffer();
    p.atlas_dset  = m_atlas.descriptor_set();
    p.count       = m_count;
    quack::run(&ps, p);
  }

  void fill(sprite::compos *ec, dotz::vec2 center) {
    auto [px, py] = center;
    pog::aabb area{{px - 32, py - 32}, {px + 32, py + 32}};

    {
      voo::memiter<quack::instance> m { m_spr.host_memory(), &m_count };

      dotz::vec2 atlas_sz { m_atlas.width(), m_atlas.height() };

      ec->areas().for_each_in(area, [&](auto id, auto aabb) {
        auto spr = ec->sprites.get(id);
        if (spr.layer != m_layer) return;

        dotz::vec2 aa { aabb.a.x, aabb.a.x };
        dotz::vec2 bb { aabb.b.x, aabb.b.x };

        dotz::vec2 uv    { spr.uv.x, spr.uv.y };
        dotz::vec2 uv_sz { spr.uv.w, spr.uv.h };

        dotz::vec4 c { spr.colour.r, spr.colour.g, spr.colour.b, spr.colour.a };

        m += quack::instance {
          .position = aa,
          .size     = bb - aa,
          .uv0      = uv * sprite_sz / atlas_sz,
          .uv1      = (uv + uv_sz) * sprite_sz / atlas_sz,
          .colour   = c,
          .multiplier { 1.0f, 1.0f, 1.0f, 1.0f - spr.dim },
        };
      });
    }

    if (m_count > m_max_sprites) m_max_sprites = m_count;

    run_once();
  }
};
} // namespace qsu
