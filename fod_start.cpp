module fod;
import fox;
import input;
import rng;
import sitime;
import v;

sitime::stopwatch g_timer {};

const int i = [] {
  v::on_start  = [] {
    fox::g = new fox::main {};
    fod::exit();
  };
  v::on_frame  = [] {
    static sitime::stopwatch timer {};
    fod::on_frame(timer.millis());
    timer = {};
  };
  v::on_stop   = [] {
    delete fox::g;
  };

  rng::seed();
  input::setup();
  return 0;
}();
