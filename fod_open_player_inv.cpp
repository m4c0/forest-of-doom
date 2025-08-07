module fod;
import fox;
import player;

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
  });
  fox::g->on_frame(16, 16, player::center());
}

void fod::open_player_inv() {
  fod::on_frame = ::on_frame;
}
