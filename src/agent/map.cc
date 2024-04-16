#include "map.h"

#include <fmt/ranges.h>

#include <string>

namespace thuai7_agent {

auto format_as(Map const& object) -> std::string {
  return fmt::format("Map{{width: {}, height: {}, obstacles: {}}}",
                     object.width, object.height, object.obstacles);
}

}  // namespace thuai7_agent
