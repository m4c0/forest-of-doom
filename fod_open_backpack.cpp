module fod;
import backpack;
import fox;
import input;
import player;

static void on_frame(float ms) {
  fox::g->load_ui([](auto * m) {
    *m += {
      .pos { 0, 0 },
      .uv { 3, 8 },
      .size = 1,
      .texid = fox::texids::ui_style,
    };
    *m += {
      .pos { 1, 0 },
      .uv { 4, 8 },
      .size = 1,
      .texid = fox::texids::ui_style,
    };
    *m += {
      .pos { 2, 0 },
      .uv { 5, 8 },
      .size = 1,
      .texid = fox::texids::ui_style,
    };

    *m += {
      .pos { 0, 1 },
      .uv { 0, 11 },
      .size = 1,
      .texid = fox::texids::ui_style,
    };
    *m += {
      .pos { 1, 1 },
      .uv { 1, 11 },
      .size = 1,
      .texid = fox::texids::ui_style,
    };
    *m += {
      .pos { 2, 1 },
      .uv { 2, 11 },
      .size = 1,
      .texid = fox::texids::ui_style,
    };

    *m += {
      .pos { 0, 2 },
      .uv { 3, 9 },
      .size = 1,
      .texid = fox::texids::ui_style,
    };
    *m += {
      .pos { 1, 2 },
      .uv { 4, 9 },
      .size = 1,
      .texid = fox::texids::ui_style,
    };
    *m += {
      .pos { 2, 2 },
      .uv { 5, 9 },
      .size = 1,
      .texid = fox::texids::ui_style,
    };
  });
  fox::g->on_frame(16, 16, player::center());
}

void fod::open_backpack() {
  input::on_button_down(input::buttons::ACTION, [] {});
  input::on_button_down(input::buttons::REST, [] {});
  fod::on_frame = ::on_frame;
}
