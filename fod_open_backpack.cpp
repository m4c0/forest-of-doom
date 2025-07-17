module fod;
import backpack;
import dotz;
import fox;
import input;
import player;

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
    const dotz::vec2 size { 10, 9 };
    const dotz::vec2 tl = -(size + 3) / 2.0;

    const auto sp = [&](dotz::vec2 pos, dotz::vec2 uv, dotz::vec2 size = 1) {
      *m += { .pos = tl + pos, .uv = uv, .size = size, .texid = fox::texids::ui_style };
    };
    const auto box = [&] {
      sp({ 0, 0 }, { 3, 8 });
      for (auto x = 0; x < size.x; x++) sp({ x + 1, 0 }, { 4, 8 });
      sp({ size.x + 1, 0.f }, { 5, 8 });

      for (auto y = 0; y < size.y; y++) {
        sp({ 0, y + 1 }, { 0, 11 });
        for (auto x = 0; x < size.x; x++) sp({ x + 1, y + 1 }, { 1, 11 });
        sp({ size.x + 1, y + 1.f }, { 2, 11 });
      }

      sp({ 0.0f, size.y + 1 }, { 3, 9 });
      for (auto x = 0; x < size.x; x++) sp({ x + 1.f, size.y + 1 }, { 4, 9 });
      sp({ size.x + 1, size.y + 1 }, { 5, 9 });
    };

    box();

    for (auto y = 0; y < 5; y++) {
      for (auto x = 0; x < 5; x++) {
        sp({ x * 2 + 0.5f, y * 2.0f }, { 3, 18 }, 3);
      }
    }

  });
  fox::g->on_frame(16, 16, player::center());
}

void fod::open_backpack() {
  input::on_button_down(input::buttons::ACTION, [] {});
  input::on_button_down(input::buttons::CANCEL, fod::poc);
  input::on_button_down(input::buttons::REST, [] {});
  fod::on_frame = ::on_frame;
}
