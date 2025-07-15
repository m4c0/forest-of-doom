export module hud;
import dotz;
import fox;
import player;

namespace hud {
  static constexpr unsigned step(float v) {
    if (v == 0)    return 0;
    if (v < 0.25f) return 1;
    if (v < 0.5f)  return 2;
    if (v < 0.75f) return 3;
    return 4;
  }

  export void load(fox::memiter * m, dotz::vec2 grid) {
    constexpr const dotz::vec2 levels[] {
      { 16, 8 },
      { 18, 14 },
      { 16, 12 },
      { 18, 9 },
      { 17, 7 },
    };

    dotz::vec2 p { -grid.x, grid.y - 4 };
    const auto sp = [&](dotz::vec2 pos, dotz::vec2 uv) {
      *m += { .pos = pos + p, .uv = uv, .size = 1, .texid = 3 };
    };
    const auto gauge = [&](float y, dotz::vec2 icon_uv, float level) {
      sp({ 0.0f, y }, icon_uv);
      sp({ 0.2f, y }, { 16, 7 });
      sp({ 1.2f, y }, levels[step(level)]);
      sp({ 2.2f, y }, { 18, 7 });
    };

    gauge(0, {  9, 10 }, player::status().happyness);
    gauge(1, { 11, 10 }, player::status().health);
    gauge(2, { 12, 10 }, player::status().satiation);
    gauge(3, { 10, 10 }, player::status().energy);
  }
}
