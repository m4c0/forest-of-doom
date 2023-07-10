export module rect;
import pog;

export struct rect {
  float x;
  float y;
  float w;
  float h;
};
export rect rect_of(pog::aabb area) {
  return rect{
      .x = area.a.x,
      .y = area.a.y,
      .w = area.b.x - area.a.x,
      .h = area.b.y - area.a.y,
  };
}
