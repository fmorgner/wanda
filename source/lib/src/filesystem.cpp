#include "wanda/filesystem.hpp"

#include <random>
#include <ranges>

namespace wanda
{
  std::optional<path_list> scan(std::filesystem::path source, bool(filter)(std::filesystem::path const &))
  {
    if (!std::filesystem::is_directory(source))
    {
      return std::nullopt;
    }
    auto entries = std::filesystem::recursive_directory_iterator{source};
    auto result = path_list{};
    for (auto & entry : entries | std::views::filter(filter))
    {
      result.push_back(entry.path());
    }
    return result;
  }

  std::filesystem::path random_pick(path_list const & paths)
  {
    static auto generator = std::mt19937{std::random_device{}()};
    auto distribution = std::uniform_int_distribution<std::size_t>{0, paths.size() - 1};

    return paths[distribution(generator)];
  }

}  // namespace wanda