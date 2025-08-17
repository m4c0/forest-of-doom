module fod;
import input;
import jute;

void fod::exit(jute::view file, jute::view entry) {
  input::reset();
  fod::on_frame = [](auto) {};
}
