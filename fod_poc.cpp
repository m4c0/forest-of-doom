module fod;
import backpack;
import dotz;
import fox;
import hud;
import input;
import player;
import silog;

static void on_action() {
  auto i = backpack::open(player::center());
  if (i >= 0) fod::open_backpack(i);
}
static void on_rest() {
  player::rest();
}

static void on_frame(float ms) {
  dotz::vec2 in {
    input::state(input::axis::X),
    input::state(input::axis::Y),
  };
  player::tick(in, ms);

  fox::g->load(fox::layers::entities, [](auto * m) {
    auto p = player::center();
    backpack::load(p, m);
  });
  fox::g->load(fox::layers::player, [](auto * m) {
    player::load(m);
  });
  fox::g->load_ui([](auto * m) {
    hud::load_ui(m, fox::g->aspect() * 8);
  });
  fox::g->on_frame(16, 16, player::center());
}

void fod::poc() {
  input::reset();;
  input::on_key_down(input::keys::ACTION, on_action);
  input::on_key_down(input::keys::REST, on_rest);
  fod::on_frame = ::on_frame;
}
