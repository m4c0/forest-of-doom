module fod;
import input;

static void on_frame(float ms) {
}

void fod::open_backpack() {
  input::on_button_down(input::buttons::ACTION, [] {});
  input::on_button_down(input::buttons::REST, [] {});
  fod::on_frame = ::on_frame;
}
