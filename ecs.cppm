export module ecs;
import animation;
import pog;
import player;
import tilemap;

namespace ecs {
export class ec : public player::compos, public tilemap::compos {};
} // namespace ecs
