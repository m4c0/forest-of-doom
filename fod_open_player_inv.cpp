module fod;
import fox;
import fui;
import input;
import player;

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
    fui::inv { &player::inv::inv(), {}, { -1 } }.load(m, {});
  });
  fox::g->on_frame(16, 16, player::center());
}

void fod::open_player_inv() {
  fod::on_frame = ::on_frame;

  using namespace input;
  reset();
  on_key_down(keys::CANCEL, fod::poc); // TODO: discard selection or quit
}
