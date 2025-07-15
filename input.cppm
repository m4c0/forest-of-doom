export module input;
import casein;
import hai;

namespace input {
  export enum class axis { X, Y };
  export enum class buttons { ACTION, REST };
  export float state(axis a);
  export bool state(buttons b);

  export void on_button_down(buttons b, hai::fn<void> fn);

  export void setup();
}

module :private;

namespace {
  enum keys {
    ACTION,
    REST,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_UP,
    MAX_KEYS,
  };
}

static hai::fn<void> g_btn_down_cb[MAX_KEYS] {};
static bool g_state[MAX_KEYS] {};
static void setup_btn(casein::keys k, keys i) {
  using namespace casein;
  handle(KEY_DOWN, k, [i] {
    auto old = g_state[i];
    g_state[i] = true;
    if (!old && g_btn_down_cb[i]) g_btn_down_cb[i]();
  });
  handle(KEY_UP,   k, [i] { g_state[i] = false; });
}

static int axis_state(keys n, keys p) {
  if (g_state[n] && !g_state[p]) return -1;
  if (!g_state[n] && g_state[p]) return 1;
  return 0;
}

float input::state(axis a) {
  switch (a) {
    case axis::X: return axis_state(MOVE_LEFT, MOVE_RIGHT);
    case axis::Y: return axis_state(MOVE_UP, MOVE_DOWN);
  }
}
bool input::state(buttons b) {
  switch (b) {
    case buttons::ACTION: return g_state[ACTION];
    case buttons::REST: return g_state[REST];
  }
}

void input::on_button_down(buttons b, hai::fn<void> fn) {
  switch (b) {
    case buttons::ACTION: g_btn_down_cb[ACTION] = fn; break;
    case buttons::REST: g_btn_down_cb[REST] = fn; break;
  }
}

void input::setup() {
  using namespace casein;
  setup_btn(K_LEFT,  MOVE_LEFT);
  setup_btn(K_RIGHT, MOVE_RIGHT);
  setup_btn(K_UP,    MOVE_UP);
  setup_btn(K_DOWN,  MOVE_DOWN);

  setup_btn(K_ENTER, ACTION);
  setup_btn(K_SPACE, REST);
}
