module fod;
import backpack;
import dotz;
import fox;
import input;
import player;

static dotz::ivec2 g_cursor {};

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
    const dotz::vec2 size { 8 };
    const dotz::vec2 tl = -(size + 3) / 2.0;
    const dotz::vec2 gtl = 0.3f;
    const dotz::vec2 csz = 1.2f;

    const auto sp = [&](dotz::vec2 pos, dotz::vec2 uv, dotz::vec2 size = 1) {
      *m += { .pos = tl + pos, .uv = uv, .size = size, .texid = fox::texids::ui_paper };
    };
    const auto box = [&] {
      sp({ 0, 0 }, { 1, 1 });
      for (auto x = 0; x < size.x; x++) sp({ x + 1, 0 }, { 2, 1 });
      sp({ size.x + 1, 0.f }, { 3, 1 });

      for (auto y = 0; y < size.y; y++) {
        sp({ 0, y + 1 }, { 1, 2 });
        for (auto x = 0; x < size.x; x++) sp({ x + 1, y + 1 }, { 2, 2 });
        sp({ size.x + 1, y + 1.f }, { 3, 2 });
      }

      sp({ 0.0f, size.y + 1 }, { 1, 3 });
      for (auto x = 0; x < size.x; x++) sp({ x + 1.f, size.y + 1 }, { 2, 3 });
      sp({ size.x + 1, size.y + 1 }, { 3, 3 });
    };

    box();

    for (dotz::ivec2 p = 0; p.y < 8; p.y++) {
      for (p.x = 0; p.x < 8; p.x++) {
        sp(p * csz + gtl, { 5, 5 });
      }
    }

    sp(g_cursor * csz + gtl, { 15, 4 });
  });
  fox::g->on_frame(16, 16, player::center());
}

void fod::open_backpack() {
  input::on_key_down(input::keys::ACTION, [] {});
  input::on_key_down(input::keys::CANCEL, fod::poc);
  input::on_key_down(input::keys::REST, [] {});
  fod::on_frame = ::on_frame;
  g_cursor = {};
}
