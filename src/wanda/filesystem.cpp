#include <wanda/filesystem.hpp>

// #include <boost/iterator/filter_iterator.hpp>
#include <range/v3/all.hpp>

#include <random>

namespace wanda
{
  std::optional<path_list> scan(std::filesystem::path source, bool(filter)(std::filesystem::path const &))
  {
    if (!std::filesystem::is_directory(source))
    {
      return std::nullopt;
    }

    auto begin = std::filesystem::recursive_directory_iterator{source};
    auto end = std::filesystem::recursive_directory_iterator{};
    return ranges::make_iterator_range(begin, end) | ranges::view::filter(filter);
  }

  std::filesystem::path random_pick(path_list const & paths)
  {
    static auto generator = std::mt19937{std::random_device{}()};
    auto distribution = std::uniform_int_distribution<std::size_t>{0, paths.size() - 1};

    return paths[distribution(generator)];
  }

}  // namespace wanda