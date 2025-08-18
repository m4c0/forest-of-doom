module fod;
import input;
import jute;
import player;

void fod::exit(jute::view file, jute::view entry) {
  player::reset();

  input::reset();
  fod::on_frame = [](auto) {};
}
