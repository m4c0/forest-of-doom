#pragma leco add_shader fox.vert
#pragma leco add_shader fox.frag
export module fox;
import dotz;
import sires;
import traits;
import v;
import voo;

namespace fox {
  static constexpr const auto max_sprites = 256 * 9 * 4;

  export struct sprite {
    dotz::vec2 pos;
    dotz::vec2 uv;
    dotz::vec2 size;
  };

  export using memiter = voo::memiter<sprite>;

  struct upc {
    dotz::vec2 grid_pos;
    dotz::vec2 grid_size;
  };

  static auto create_sampler() {
    auto info = vee::sampler_create_info {};
    info.address_mode(VK_SAMPLER_ADDRESS_MODE_REPEAT);
    info.nearest();
    info.unnormalizedCoordinates = 1;
    return vee::create_sampler(info);
  }

  export class main {
    vee::sampler m_smp = create_sampler();
    voo::bound_image m_img {};
    voo::bound_buffer m_buf = voo::bound_buffer::create_from_host(
        v::dq()->physical_device(),
        max_sprites * sizeof(sprite),
        vee::buffer_usage::vertex_buffer);
    unsigned m_count {};

    voo::one_quad m_quad { v::dq()->physical_device() };

    voo::single_frag_dset m_dset { 1 };
    vee::pipeline_layout m_pl = vee::create_pipeline_layout(
        m_dset.descriptor_set_layout(),
        vee::vertex_push_constant_range<upc>());
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
        vee::vertex_attribute_vec2(1, traits::offset_of(&sprite::uv)),
        vee::vertex_attribute_vec2(1, traits::offset_of(&sprite::size)),
      },
    });

  public:
    main() {
      auto img = sires::real_path_name("1_Terrains_and_Fences_16x16.png");
      auto pd = v::dq()->physical_device();
      auto q = v::dq()->queue();
      voo::load_image(img, pd, q, &m_img, [this] {
        vee::update_descriptor_set(m_dset.descriptor_set(), 0, *m_img.iv, *m_smp);
      });
    }
    void load(auto && fn) {
      voo::memiter<sprite> m { *m_buf.memory, &m_count };
      fn(&m);
    }

    void on_frame(dotz::vec2 center) {
      upc pc {
        .grid_pos = center,
        .grid_size = dotz::vec2 { v::sw()->aspect(), 1.0f } * 16,
      };

      auto cb = v::sw()->command_buffer();
      vee::cmd_bind_gr_pipeline(cb, *m_ppl);
      vee::cmd_bind_vertex_buffers(cb, 1, *m_buf.buffer);
      vee::cmd_push_vertex_constants(cb, *m_pl, &pc);
      vee::cmd_bind_descriptor_set(cb, *m_pl, 0, m_dset.descriptor_set());
      m_quad.run(cb, 0, m_count);
    }
  };
}
