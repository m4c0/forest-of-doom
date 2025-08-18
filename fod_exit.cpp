module fod;
import backpack;
import collision;
import exit;
import input;
import jute;
import player;

void fod::exit(jute::view file, jute::view entry) {
  backpack::purge();
  collision::purge_all();
  exits::purge();
  player::reset();

  input::reset();
  fod::on_frame = [](auto) {};
}
