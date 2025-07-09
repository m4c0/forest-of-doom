module v;
import casein;
import voo;
import vinyl;

static struct app_globals {
  voo::device_and_queue dq { "fod", casein::native_ptr };
} * g_ag;

static struct extent_globals {
  voo::swapchain_and_stuff sw { g_ag->dq };
} * g_eg;

voo::device_and_queue * v::dq() { return &g_ag->dq; }
voo::swapchain_and_stuff * v::sw() { return &g_eg->sw; }

static void on_start() {
  g_ag = new app_globals {};
  v::on_start();
}
static void on_resize() {
  delete g_eg;
  g_eg = new extent_globals {};
  v::on_resize();
}
static void on_frame() {
  v::sw()->acquire_next_image();
  v::sw()->queue_one_time_submit(v::dq()->queue(), [](auto pcb) {
    auto rp = v::sw()->cmd_render_pass();
    v::on_frame();
  });
  v::sw()->queue_present(v::dq()->queue());
}
static void on_stop() {
  v::on_stop();
  delete g_eg;
  delete g_ag;
}

struct app_init {
  app_init() {
    using namespace vinyl;
    on(START,  on_start);
    on(RESIZE, on_resize);
    on(FRAME,  on_frame);
    on(STOP,   on_stop);
  }
} i;
