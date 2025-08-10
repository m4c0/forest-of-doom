module v;
import casein;
import voo;
import vinyl;

static struct app_globals {
  voo::device_and_queue dq { "fod", casein::native_ptr };
  vee::render_pass rp = voo::single_att_render_pass(dq);
} * g_ag;

static struct extent_globals {
  voo::swapchain_and_stuff sw { g_ag->dq, *g_ag->rp };
} * g_eg;

voo::device_and_queue * v::dq() { return &g_ag->dq; }
vee::render_pass::type v::rp() { return *g_ag->rp; }
voo::swapchain_and_stuff * v::sw() { return &g_eg->sw; }

static void on_start() {
  g_ag = new app_globals {};
  v::on_start();
}
static void on_resize() {
  delete g_eg;
  g_eg = new extent_globals {};
}
static void on_frame() {
  v::sw()->acquire_next_image();
  v::sw()->queue_one_time_submit(v::dq()->queue(), [] {
    auto rp = v::sw()->cmd_render_pass();
    auto cb = v::sw()->command_buffer();
    vee::cmd_set_viewport(cb, v::sw()->extent());
    vee::cmd_set_scissor(cb, v::sw()->extent());
    v::on_frame();
  });
  v::sw()->queue_present(v::dq()->queue());
}
static void on_stop() {
  v::on_stop();
  delete g_eg;
  delete g_ag;
}

const int v_init = [] {
  using namespace vinyl;
  on(START,  on_start);
  on(RESIZE, on_resize);
  on(FRAME,  on_frame);
  on(STOP,   on_stop);
  return 0;
}();
