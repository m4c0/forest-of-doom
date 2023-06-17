export module main;

import casein;
import quack;
import sires;
import silog;
import stubby;

class sprite_layout : public quack::instance_layout<void, 1> {
  void resize(unsigned w, unsigned h) override { batch()->resize(7, 7, w, h); }

public:
  using instance_layout::instance_layout;
};

class qsu {
  quack::renderer m_r{1};
  sprite_layout m_spr{&m_r};

  void setup() {
    stbi::load_from_resource("11_Camping_16x16.png")
        .map([this](const auto &img) {
          m_r.load_atlas(img.width, img.height, [&img](auto *p) {
            const auto pixies = img.width * img.height;
            const auto data = reinterpret_cast<const decltype(p)>(*img.data);
            for (auto i = 0; i < pixies; i++) {
              p[i] = data[i];
            }
          });
          silog::log(silog::info, "%dx%d atlas loaded", img.width, img.height);
        })
        .take([](auto err) {
          silog::log(silog::error, "Error loading atlas: %s", err);
        });
  }

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    m_spr.process_event(e);

    if (e.type() == casein::CREATE_WINDOW)
      setup();
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static qsu q{};
  // static game gg{&r};

  static constexpr const auto k_map = [] {
    casein::key_map res{};
    // res[casein::K_DOWN] = [](auto) { gg.down(); };
    // res[casein::K_LEFT] = [](auto) { gg.left(); };
    // res[casein::K_RIGHT] = [](auto) { gg.right(); };
    // res[casein::K_UP] = [](auto) { gg.up(); };
    return res;
  }();
  static constexpr const auto map = [] {
    casein::event_map res{};
    // res[casein::CREATE_WINDOW] = [](auto) { gg.setup(); };
    res[casein::KEY_DOWN] = [](auto e) { k_map.handle(e); };
    return res;
  }();

  q.process_event(e);
  // gg.process_event(e);
  map.handle(e);
}
