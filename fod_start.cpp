module fod;
import fox;
import input;
import sitime;
import v;

sitime::stopwatch g_timer {};

const int i = [] {
  v::on_start  = [] {
    fox::g = new fox::main {};
    fod::exit("prefabs-camping-0", "start");
  };
  v::on_frame  = [] {
    static sitime::stopwatch timer {};
    fod::on_frame(timer.millis());
    timer = {};
  };
  v::on_stop   = [] {
    delete fox::g;
  };

  input::setup();
  return 0;
}();
