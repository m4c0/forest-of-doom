export module cursor;
import pog;
import sprite;

export namespace cursor {
struct c {};
using compo = pog::singleton<c>;

void add_entity(pog::entity_list *e, compo *cur, sprite::compo *sprites) {}
} // namespace cursor
