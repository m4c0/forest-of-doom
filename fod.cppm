#pragma leco app
export module fod;

import backpack;
import dotz;
import example_map;
import fox;
import hud;
import input;
import player;
import silog;
import sitime;
import v;

fox::main * g_fox {};
sitime::stopwatch g_timer {};

static void on_start() {
  g_timer = {};
  g_fox = new fox::main {};
  
  g_fox->load(fox::layers::floor, [](auto * m) {
    example_map::load(m);
  });
}

static void on_frame() {
  float ms = g_timer.millis();
  dotz::vec2 in {
    input::state(input::axis::X),
    input::state(input::axis::Y),
  };
  player::tick(in, ms);

  g_fox->load(fox::layers::entities, [](auto * m) {
    auto p = player::center();
    backpack::load(p, m);
  });
  g_fox->load(fox::layers::player, [](auto * m) {
    player::load(m);
  });
  g_fox->load_ui([](auto * m) {
    hud::load_ui(m, g_fox->aspect() * 8);
  });
  g_fox->on_frame(16, 16, player::center());

  g_timer = {};
}

static void on_resize() {
}

static void on_stop() {
  delete g_fox;
}

static void on_action() {
  if (backpack::open(player::center())) {
    // TODO: open UI, reset input, etc
    silog::log(silog::info, "open sesame");
  }
}
static void on_rest() {
  player::rest();
}

const int i = [] {
  v::on_start  = on_start;
  v::on_resize = on_resize;
  v::on_frame  = on_frame;
  v::on_stop   = on_stop;

  input::setup();
  input::on_button_down(input::buttons::ACTION, on_action);
  input::on_button_down(input::buttons::REST, on_rest);
  return 0;
}();
