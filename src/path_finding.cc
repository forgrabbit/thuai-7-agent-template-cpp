#include "path_finding.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <queue>
#include <ranges>
#include <unordered_set>
#include <vector>

#include "agent/position.h"

static auto CalculateManhattanDistance(thuai7_agent::Position<int> const& start,
                                       thuai7_agent::Position<int> const& end)
    -> int;
static auto GetNeighbors(thuai7_agent::Map const& map,
                         thuai7_agent::Position<int> const& position)
    -> std::vector<thuai7_agent::Position<int>>;
static auto IsValidPosition(thuai7_agent::Map const& map,
                            thuai7_agent::Position<int> const& position)
    -> bool;

auto FindPathBeFS(thuai7_agent::Map const& map,
                  thuai7_agent::Position<int> const& start,
                  thuai7_agent::Position<int> const& end)
    -> std::vector<thuai7_agent::Position<int>> {
  auto priority_queue_comparator =
      [&end](thuai7_agent::Position<int> const& lhs,
             thuai7_agent::Position<int> const& rhs) -> bool {
    return CalculateManhattanDistance(lhs, end) >
           CalculateManhattanDistance(rhs, end);
  };
  auto unordered_set_hasher =
      [](thuai7_agent::Position<int> const& position) -> std::size_t {
    return std::hash<int>{}(position.x) ^ std::hash<int>{}(position.y);
  };

  std::priority_queue<thuai7_agent::Position<int>,
                      std::vector<thuai7_agent::Position<int>>,
                      decltype(priority_queue_comparator)>
      queue(priority_queue_comparator);

  std::unordered_set<thuai7_agent::Position<int>,
                     decltype(unordered_set_hasher)>
      visited(0, unordered_set_hasher);

  std::unordered_map<thuai7_agent::Position<int>, thuai7_agent::Position<int>,
                     decltype(unordered_set_hasher)>
      parents(0, unordered_set_hasher);

  queue.push(start);
  visited.insert(start);

  bool is_found = false;

  while (!queue.empty() && !is_found) {
    auto current = queue.top();
    queue.pop();

    if (current == end) {
      is_found = true;
      break;
    }

    for (auto const& neighbor : GetNeighbors(map, current)) {
      if (visited.find(neighbor) != visited.end()) {
        continue;
      }

      queue.push(neighbor);
      visited.insert(neighbor);
      parents[neighbor] = current;
    }
  }

  if (!is_found) {
    return {};
  }

  std::vector<thuai7_agent::Position<int>> path;
  for (auto current = end; current != start; current = parents[current]) {
    path.push_back(current);
  }
  path.push_back(start);

  std::reverse(path.begin(), path.end());

  return path;
}

static auto CalculateManhattanDistance(thuai7_agent::Position<int> const& start,
                                       thuai7_agent::Position<int> const& end)
    -> int {
  return std::abs(start.x - end.x) + std::abs(start.y - end.y);
}

static auto GetNeighbors(thuai7_agent::Map const& map,
                         thuai7_agent::Position<int> const& position)
    -> std::vector<thuai7_agent::Position<int>> {
  std::vector<thuai7_agent::Position<int>> neighbors{
      {position.x - 1, position.y},
      {position.x + 1, position.y},
      {position.x, position.y - 1},
      {position.x, position.y + 1}};

  auto view =
      neighbors | std::views::filter([&map](auto const& neighbor) -> bool {
        return IsValidPosition(map, neighbor);
      });
  return {view.begin(), view.end()};
}

static auto IsValidPosition(thuai7_agent::Map const& map,
                            thuai7_agent::Position<int> const& position)
    -> bool {
  return std::ranges::all_of(map.obstacles,
                             [&position](auto const& obstacle) -> bool {
                               return obstacle != position;
                             });
}
