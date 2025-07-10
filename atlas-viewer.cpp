#pragma leco add_resource_dir res
#pragma leco app

import casein;
import fox;
import voo;
import v;

fox::main * g_fox;

const int i = [] {
  v::on_start = [] {
    g_fox = new fox::main {};
    g_fox->load([](auto * m) {
      *m += {
        .pos { 0, 0 },
        .uv { 0, 0 },
        .size { 32, 32 },
      };
    });
  };
  v::on_frame = [] {
    g_fox->on_frame(32, { 16, 16 });
  };
  v::on_resize = [] {};
  v::on_stop = [] {
    delete g_fox;
  };

  return 0;
}();
