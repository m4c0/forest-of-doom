export module wgen;
import bitmask;
import casein;
import eigen;
import hai;
import prefabs;
import rng;
import qsu;
import silog;
import sitime;
import tile;
import tilemap;
import traits;

static_assert(static_cast<unsigned>(tile::terrain::last) < 64);

struct ec : tile::terrain::compos {};

using namespace eigen;

static const consts cs = eigen::create_consts(prefabs::wgen_0);
class ieigen : public eigen::state {
public:
  ieigen() : state{cs.mask} {}
};

static constexpr const auto width = 32;
static constexpr const auto height = 32;
class map {
  static constexpr const auto sim_m = 1;
  static constexpr const auto margin = sim_m;

  ieigen m_states[height][width]{};

  [[nodiscard]] auto find_lowest_entropy() {
    struct {
      unsigned min_x{};
      unsigned min_y{};
      unsigned min_e = max_entropy;
    } me;

    for (auto y = margin; y < height - margin * 2; y++) {
      for (auto x = margin; x < width - margin * 2; x++) {
        if (m_states[y][x].value())
          continue;

        auto e = m_states[y][x].entropy();
        if (e == 0)
          continue;
        if (me.min_e < e)
          continue;

        if (me.min_e == e && rng::randf() > 0.5)
          continue;

        me.min_e = e;
        me.min_x = x;
        me.min_y = y;
      }
    }
    return me;
  }

  void check_integrity(unsigned min_x, unsigned min_y) {
    for (auto dx = -sim_m; dx <= sim_m; dx++) {
      for (auto dy = -sim_m; dy <= sim_m; dy++) {
        auto &s = m_states[min_y + dy][min_x + dx];
        if (s.entropy() == 0)
          throw world_decayed{};
      }
    }
  }
  void apply_staging(unsigned min_x, unsigned min_y) {
    for (auto dx = -sim_m; dx <= sim_m; dx++) {
      for (auto dy = -sim_m; dy <= sim_m; dy++) {
        auto &s = m_states[min_y + dy][min_x + dx];
        s.apply_stage();
      }
    }
  }

public:
  void observe_minimal_entropy() {
    auto m = find_lowest_entropy();
    auto min_e = m.min_e;
    auto min_x = m.min_x;
    auto min_y = m.min_y;

    if (min_e == max_entropy)
      throw eigen::world_decayed{};

    auto r = rng::rand(min_e);
    auto n = m_states[min_y][min_x].observe(r);

    cs.pat.for_each([&](int x, int y, unsigned t) {
      if (n != t)
        return;

      for (auto dx = -sim_m; dx <= sim_m; dx++) {
        for (auto dy = -sim_m; dy <= sim_m; dy++) {
          auto px = ((x + dx) + tilemap::width) % tilemap::width;
          auto py = ((y + dy) + tilemap::height) % tilemap::height;
          if (!m_states[min_y + dy][min_x + dx][cs.pat.get(px, py)])
            return;
        }
      }
      for (auto dx = -sim_m; dx <= sim_m; dx++) {
        for (auto dy = -sim_m; dy <= sim_m; dy++) {
          auto px = ((x + dx) + tilemap::width) % tilemap::width;
          auto py = ((y + dy) + tilemap::height) % tilemap::height;
          m_states[min_y + dy][min_x + dx].set_stage(cs.pat.get(px, py));
        }
      }
    });
    check_integrity(min_x, min_y);
    apply_staging(min_x, min_y);
  }

  void print(tile::terrain::compos *ec) const {
    for (auto y = 0; y < height; y++) {
      for (auto x = 0; x < width; x++) {
        auto &st = m_states[y][x];
        if (st.entropy() < 4)
          silog::log(silog::debug, "%dx%d e=%d b=0x%08llx", x, y, st.entropy(),
                     st.bits());

        auto t = static_cast<tile::terrain::c>(st.value());

        if (t != tile::terrain::blank)
          tile::terrain::add_tile(ec, t, x, y);

        sprite::c s{
            .layer = sprite::layers::debug,
            .colour = {0, 0, 0, 0.5},
            .dim = 1,
        };
        rect r{static_cast<float>(x), static_cast<float>(y), 1, 1};

        auto e = m_states[y][x].entropy();
        if (e < 4) {
          s.colour.r = e / 4.f;
        } else if (e < 16) {
          s.colour.g = e / 16.f;
        } else {
          s.colour.b = e / 64.f;
        }

        sprite::add(ec, s, r);
      }
    }
  }
};

class app {
  qsu::main m_q{};
  ec m_ec{};
  hai::uptr<map> m_map;
  bool m_frozen = false;

  void setup() {
    m_q.set_grid(32, 32);
    m_q.center_at(16, 16);

    rng::seed(69);

    m_map = hai::uptr<map>::make();

    for (auto i = 0; i < width * height; i++) {
      try {
        m_map->observe_minimal_entropy();
      } catch (eigen::world_decayed) {
        silog::log(silog::info, "reality decayed after %d cycles", i);
        m_frozen = true;
        break;
      }
    }

    m_map->print(&m_ec);
    m_q.fill(&m_ec);
  }

  void step() {
    if (m_frozen)
      return;

    try {
      m_map->observe_minimal_entropy();
    } catch (eigen::world_decayed) {
      m_frozen = true;
    }

    m_ec = {};
    m_map->print(&m_ec);
    m_q.fill(&m_ec);
  }

  void move_map(int dx, int dy) {
    auto [cx, cy] = m_q.center();
    m_q.center_at(cx + dx, cy + dy);
  }

public:
  void process_event(const casein::event &e) {
    m_q.process_event(e);

    switch (e.type()) {
    case casein::CREATE_WINDOW:
      setup();
      break;
    case casein::KEY_DOWN: {
      switch (*e.as<casein::events::key_down>()) {
      case casein::K_LEFT:
        move_map(-1, 0);
        break;
      case casein::K_RIGHT:
        move_map(1, 0);
        break;
      case casein::K_UP:
        move_map(0, 1);
        break;
      case casein::K_DOWN:
        move_map(0, -1);
        break;
      default:
        step();
        break;
      }
      break;
    }
    default:
      break;
    }
  }
};

extern "C" void casein_handle(const casein::event &e) {
  static app gg{};
  gg.process_event(e);
}
