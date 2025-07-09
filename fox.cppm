#pragma leco add_shader fox.vert
#pragma leco add_shader fox.frag
export module fox;
import dotz;
import traits;
import v;
import voo;

namespace fox {
  static constexpr const auto max_sprites = 256 * 9 * 4;

  export struct sprite {
    dotz::vec2 pos;
  };

  export using memiter = voo::memiter<sprite>;

  export class main {
    voo::bound_buffer m_buf = voo::bound_buffer::create_from_host(
        v::dq()->physical_device(),
        max_sprites * sizeof(sprite),
        vee::buffer_usage::vertex_buffer);
    unsigned m_count {};

    voo::one_quad m_quad { v::dq()->physical_device() };

    vee::pipeline_layout m_pl = vee::create_pipeline_layout();
    vee::gr_pipeline m_ppl = vee::create_graphics_pipeline({
      .pipeline_layout = *m_pl,
      .render_pass = v::dq()->render_pass(),
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
      },
    });

  public:
    void load(auto && fn) {
      voo::memiter<sprite> m { *m_buf.memory, &m_count };
      fn(&m);
    }

    void render(vee::command_buffer cb) {
      vee::cmd_bind_gr_pipeline(cb, *m_ppl);
      vee::cmd_bind_vertex_buffers(cb, 1, *m_buf.buffer);
      m_quad.run(cb, 0, m_count);
    }
  };
}
