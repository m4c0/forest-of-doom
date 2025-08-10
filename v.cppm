#pragma leco add_impl v_impl.cpp

export module v;
import hai;
import voo;

namespace v {
  export voo::device_and_queue * dq();
  export vee::command_buffer cb();
  export vee::render_pass::type rp();

  export hai::fn<void> on_start  {};
  export hai::fn<void> on_frame  {};
  export hai::fn<void> on_resize {};
  export hai::fn<void> on_stop   {};
}
