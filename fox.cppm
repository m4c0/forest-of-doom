#pragma leco add_shader fox.vert
#pragma leco add_shader fox.frag
#pragma leco add_impl fox_uber_dset.cpp
export module fox;
import dotz;
import hai;
import sires;
import traits;
import v;
import voo;
import wagen;

namespace fox {
  constexpr const auto max_sprites = 256 * 9 * 4;
  constexpr const auto uber_dset_smps = 2;

  export struct sprite {
    dotz::vec2 pos;
    dotz::vec2 uv;
    dotz::vec2 size;
    float alpha = 1;
    unsigned texid = 0;
  };

  export using memiter = voo::memiter<sprite>;

  struct upc {
    dotz::vec2 grid_pos;
    dotz::vec2 grid_size;
  };

  class uber_dset {
    vee::sampler m_smp;
    voo::single_dset m_dset;
    voo::bound_image m_img[uber_dset_smps];

  public:
    uber_dset();

    constexpr const auto descriptor_set() const { return m_dset.descriptor_set(); }
    constexpr const auto descriptor_set_layout() const { return m_dset.descriptor_set_layout(); }
  };

  class sprite_buffer {
    voo::bound_buffer m_buf = voo::bound_buffer::create_from_host(
        v::dq()->physical_device(),
        max_sprites * sizeof(sprite),
        vee::buffer_usage::vertex_buffer);
    unsigned m_count {};

  public:
    void load(auto && fn) {
      voo::memiter<sprite> m { *m_buf.memory, &m_count };
      fn(&m);
    }
    auto bind(vee::command_buffer cb) {
      vee::cmd_bind_vertex_buffers(cb, 1, *m_buf.buffer);
      return m_count;
    }
  };

  export class main {
    sprite_buffer m_buf {};

    voo::one_quad m_quad { v::dq()->physical_device() };

    uber_dset m_dset {};
    vee::pipeline_layout m_pl = vee::create_pipeline_layout(
        m_dset.descriptor_set_layout(),
        vee::vertex_push_constant_range<upc>());
    vee::gr_pipeline m_ppl = vee::create_graphics_pipeline({
      .pipeline_layout = *m_pl,
      .render_pass = v::rp(),
      .shaders {
        voo::shader("fox.vert.spv").pipeline_vert_stage(),
        voo::shader("fox.frag.spv").pipeline_frag_stage(),
      },
      .bindings {
        voo::one_quad::vertex_input_bind(),
        vee::vertex_input_bind_per_instance(sizeof(sprite)),
      },
      .attributes {
        voo::one_quad::vertex_attribute(0),
        vee::vertex_attribute_vec2(1, traits::offset_of(&sprite::pos)),
        vee::vertex_attribute_vec2(1, traits::offset_of(&sprite::uv)),
        vee::vertex_attribute_vec2(1, traits::offset_of(&sprite::size)),
        vee::vertex_attribute_float(1, traits::offset_of(&sprite::alpha)),
        vee::vertex_attribute_uint(1, traits::offset_of(&sprite::texid)),
      },
    });

  public:
    void load(auto && fn) { m_buf.load(fn); }

    void on_frame(float grid_size, dotz::vec2 center) {
      upc pc {
        .grid_pos = center,
        .grid_size = dotz::vec2 { v::sw()->aspect(), 1.0f } * grid_size / 2,
      };

      auto cb = v::sw()->command_buffer();
      vee::cmd_bind_gr_pipeline(cb, *m_ppl);
      vee::cmd_push_vertex_constants(cb, *m_pl, &pc);
      vee::cmd_bind_descriptor_set(cb, *m_pl, 0, m_dset.descriptor_set());
      m_quad.run(cb, 0, m_buf.bind(cb));
    }
  };
}
