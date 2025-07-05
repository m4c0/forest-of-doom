export module qsu:atlas;
import jute;
import quack;
import vee;
import voo;

namespace qsu {
class atlas : public voo::update_thread {
  vee::sampler m_smp = vee::create_sampler(vee::nearest_sampler);
  voo::h2l_image m_img;
  vee::descriptor_set m_ds;
  jute::view m_name;

  void build_cmd_buf(vee::command_buffer cb) override {
    voo::cmd_buf_one_time_submit pcb{cb};
    m_img.setup_copy(cb);
  }

  static auto load_image(voo::device_and_queue *dq, jute::view name) {
    if (name == "") {
      return voo::h2l_image { dq->physical_device(), 16, 16, VK_FORMAT_R8G8B8A8_SRGB };
    } else {
      return voo::load_sires_image(name, dq->physical_device());
    }
  }

public:
  atlas(voo::device_and_queue *dq, quack::pipeline_stuff *ps, jute::view name)
      : update_thread{dq->queue()}, m_img{load_image(dq, name)},
        m_ds{ps->allocate_descriptor_set(m_img.iv(), *m_smp)}, m_name{name} {}

  [[nodiscard]] constexpr auto descriptor_set() const noexcept { return m_ds; }
  [[nodiscard]] constexpr auto name() const noexcept { return m_name; }
  [[nodiscard]] constexpr auto width() const noexcept { return m_img.width(); }
  [[nodiscard]] constexpr auto height() const noexcept {
    return m_img.height();
  }

  using update_thread::run_once;
};
} // namespace qsu
