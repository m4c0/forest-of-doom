module prefabs;
import hashley;
import jojo;
import sires;
import traits;

// TODO: random tiles (example: automatically apply different grass tiles)
// TODO: layers?

namespace prefabs {
  constexpr static dotz::vec4 parse_vec4(jute::view v) {
    // TODO: validate if all values are defined
    auto [x, a] = v.split(' ');
    auto [y, b] = a.split(' ');
    auto [z, w] = b.split(' ');
    return { jute::to_f(x), jute::to_f(y), jute::to_f(z), jute::to_f(w) };
  } 
  static_assert(parse_vec4("0 1 2 3").x == 0);
  static_assert(parse_vec4("0 1 2 3").y == 1);
  static_assert(parse_vec4("0 1 2 3").z == 2);
  static_assert(parse_vec4("0 1 2 3").w == 3);
  static_assert(dotz::sq_length(parse_vec4("")) == 0);

  class tiledefs {
    static constexpr const auto max = 128;

    hai::varray<tiledef> m_defs { max };

    [[nodiscard]] constexpr auto & current() { return m_defs[m_defs.size() - 1]; }

    void copy(tiledef o) {
      auto & c = current();
      if (dotz::length(o.tile) > 0) c.tile = o.tile;
      if (dotz::length(o.collision) > 0) c.collision = o.collision;
    }

    [[nodiscard]] constexpr auto & operator[](jute::view id) const {
      for (auto & d: m_defs) if (d.id == id) return d;
      throw error("undefined tiledef: "_hs + id);
    }

  public:
    [[nodiscard]] constexpr auto & operator[](char id) const {
      jute::view v { &id, 1 };
      return (*this)[v];
    };

    void add(jute::view arg) { m_defs.push_back(tiledef { arg.trim() }); }
    void parse(jute::view line) {
      auto [cmd, args] = line.split(' ');
      args = args.trim();

           if (cmd == "tile")      current().tile      = parse_vec4(args);
      else if (cmd == "entity")    current().entity    = parse_vec4(args);
      else if (cmd == "collision") current().collision = parse_vec4(args);
      else if (cmd == "copy")      copy((*this)[args]);
      else throw error { "unknown command: "_hs + cmd };
    }

    void validate_last() {
      auto & c = current();
      const auto err = [&](jute::heap msg) {
        throw error { msg + " for tiledef [" + c.id + "]" };
      };

      if (dotz::length(c.tile.zw()) == 0) err("missing size of tile"_hs);
      if (dotz::length(c.collision.xy()) != 0 && dotz::length(c.collision.zw()) == 0) err("missing size of collisio"_hs);
    }
  };

  class parser {
    void (parser::*m_liner)(jute::view) = &parser::take_command;
    unsigned m_map_row = 0;
   
    tiledefs m_tdefs {};
    tilemap m_map {};

    void cmd_version(jute::view arg) {
      arg = arg.trim();
      if (arg != "0") throw error { "invalid version: "_hs + arg };
    }

    void cmd_define(jute::view arg) {
      m_tdefs.add(arg);
      m_liner = &parser::read_define;
    }

    void read_define(jute::view line) {
      line = line.trim();
      if (line == ".") {
        m_tdefs.validate_last();
        m_liner = &parser::take_command;
        return;
      }
      m_tdefs.parse(line);
    }
    
    void read_map(jute::view line) {
      if (line == ".") {
        m_liner = &parser::take_command;
        return;
      }

      unsigned x = 0;
      unsigned y = m_map_row++;
      for (auto c : line) {
        if (c != ' ') m_map(x, y) = m_tdefs[c];
        x++;
      }
    }

    void take_command(jute::view line) {
      line = line.trim();
      if (line == "") return;
      if (line.subview(2).before == "//") return;
      
      auto [cmd, args] = line.split(' ');
    
      if (cmd == "version") return cmd_version(args);
      if (cmd == "define")  return cmd_define(args);

      if (cmd == "map") {
        m_liner = &parser::read_map;
        m_map_row = 0;
        return;
      }
    
      throw error { "unknown command: "_hs + cmd };
    }

  public:
    void parse(jute::view line) { (this->*m_liner)(line); }

    [[nodiscard]] constexpr auto take() { return traits::move(m_map); }
  };

  // TODO: eviction rules
  hashley::fin<tilemap> g_cache { 127 };

  const tilemap * load(jute::view filename) {
    if (g_cache.has(filename)) return &g_cache[filename];

    unsigned line_number = 1;
    try {
      parser p {};
      jojo::readlines(sires::real_path_name(filename), [&](auto line) {
        p.parse(line);
        line_number++;
      });
      g_cache[filename] = p.take();
      return &g_cache[filename];
    } catch (error & e) {
      e.line_number = line_number;
      throw traits::move(e);
    }
  }
}
