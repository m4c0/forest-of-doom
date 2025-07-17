module fod;
import example_map;
import fox;
import input;
import sitime;
import v;

sitime::stopwatch g_timer {};

const int i = [] {
  v::on_start  = [] {
    fox::g = new fox::main {};
    fox::g->load(fox::layers::floor, [](auto * m) {
      example_map::load(m);
    });
  };
  v::on_resize = [] {};
  v::on_frame  = [] {
    static sitime::stopwatch timer {};
    fod::on_frame(timer.millis());
    timer = {};
  };
  v::on_stop   = [] {
    delete fox::g;
  };

  input::setup();

  fod::poc();
  return 0;
}();
