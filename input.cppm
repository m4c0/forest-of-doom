export module input;
import casein;
import hai;

namespace input {
  export enum class axis { X, Y };
  export enum class keys : unsigned {
    ACTION = 0,
    CANCEL,
    REST,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_UP,
    MAX_KEYS,
  };

  export float state(axis a);

  export void on_key_down(keys b, hai::fn<void> fn);
  export void reset();

  export void setup();
}

module :private;

static constexpr auto ik(input::keys k) { return static_cast<unsigned>(k); }

static hai::fn<void> g_btn_down_cb[ik(input::keys::MAX_KEYS)] {};
static bool g_state[ik(input::keys::MAX_KEYS)] {};
static void setup_btn(casein::keys k, input::keys i) {
  using namespace casein;
  handle(KEY_DOWN, k, [i=ik(i)] {
    auto old = g_state[i];
    g_state[i] = true;
    if (!old && g_btn_down_cb[i]) g_btn_down_cb[i]();
  });
  handle(KEY_UP,   k, [i=ik(i)] { g_state[i] = false; });
}

static int axis_state(input::keys n, input::keys p) {
  if (g_state[ik(n)] && !g_state[ik(p)]) return -1;
  if (!g_state[ik(n)] && g_state[ik(p)]) return 1;
  return 0;
}

float input::state(axis a) {
  switch (a) {
    case axis::X: return axis_state(keys::MOVE_LEFT, keys::MOVE_RIGHT);
    case axis::Y: return axis_state(keys::MOVE_UP,   keys::MOVE_DOWN);
  }
}

void input::on_key_down(keys k, hai::fn<void> fn) { g_btn_down_cb[ik(k)] = fn; }

void input::reset() {
  for (auto i = 0; i < ik(keys::MAX_KEYS); i++) {
    g_state[i] = false;
    g_btn_down_cb[i] = {};
  }
}

void input::setup() {
  using namespace casein;
  setup_btn(K_LEFT,  keys::MOVE_LEFT);
  setup_btn(K_RIGHT, keys::MOVE_RIGHT);
  setup_btn(K_UP,    keys::MOVE_UP);
  setup_btn(K_DOWN,  keys::MOVE_DOWN);

  setup_btn(K_ENTER,  keys::ACTION);
  setup_btn(K_ESCAPE, keys::CANCEL);
  setup_btn(K_SPACE,  keys::REST);
}
