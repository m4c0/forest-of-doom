export module prefabs;
export import :island_0;
export import :island_1;
export import :ocean_0;
export import :wgen_0;
export import :wgen_1;
import dotz;
import jojo;
import jute;
import hai;
import traits;

using namespace jute::literals;

namespace prefabs {
  export struct error {
    hai::cstr msg;
    unsigned line_number;

    error(jute::heap h) : msg { (*h).cstr() } {}
  };

  struct tiledef {
    jute::heap id;
    unsigned   tile;
    dotz::vec4 collision;
  };

  static dotz::vec4 parse_vec4(jute::view v) {
    throw error { "parse vec4 TBD"_hs };
  } 

  class tiledefs {
    static constexpr const auto max = 128;

    hai::varray<tiledef> m_defs { max };

    [[nodiscard]] constexpr auto & current() { return m_defs[m_defs.size() - 1]; }

    void copy(tiledef o) {
      auto & c = current();
      if (o.tile) c.tile = o.tile;
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

           if (cmd == "tile")      current().tile      = jute::to_u32(args);
      else if (cmd == "collision") current().collision = parse_vec4(args);
      else throw error { "unknown command: "_hs + cmd };
    }

    void validate_last() {
      auto & c = current();
      const auto err = [&](jute::heap msg) {
        throw error { msg + " for tiledef [" + c.id + "]" };
      };

      if (c.tile != 28) err("TBD: tiles other than 28"_hs);
    }
  };

  class tilemap {
  };

  class parser {
    void (parser::*m_liner)(jute::view) = &parser::take_command;
    unsigned m_map_row = 1;
   
    tiledefs m_tdefs {};
    //tilemap m_map {};

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
        x++;
        //if (c != ' ') m_map(x, y) = m_tdefs[c];
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
        m_map_row = 1;
        return;
      }
    
      throw error { "unknown command: "_hs + cmd };
    }

  public:
    void parse(jute::view line) { (this->*m_liner)(line); }

    //[[nodiscard]] constexpr auto take() { return traits::move(m_map); }
  };

  export [[nodiscard]] auto load(jute::view filename) {
    unsigned line_number = 1;
    try {
      parser p {};
      jojo::readlines(filename, [&](auto line) {
        p.parse(line);
        line_number++;
      });
      //return p.take();
    } catch (error & e) {
      e.line_number = line_number;
      throw traits::move(e);
    }
  }
}
