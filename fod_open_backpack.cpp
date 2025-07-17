module fod;
import backpack;
import fox;
import input;
import player;

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
  });
  fox::g->on_frame(16, 16, player::center());
}

void fod::open_backpack() {
  input::on_button_down(input::buttons::ACTION, [] {});
  input::on_button_down(input::buttons::REST, [] {});
  fod::on_frame = ::on_frame;
}
