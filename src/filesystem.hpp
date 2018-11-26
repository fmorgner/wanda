#ifndef WANDA_FILESYSTEM_HPP
#define WANDA_FILESYSTEM_HPP

#include <filesystem>
#include <optional>
#include <vector>

namespace wanda
{

/**
 * @brief Covenience alias for path lists
 */
using path_list = std::vector<std::filesystem::path>;

/**
 * @brief The default scan filter, allowing only regular files to pass
 */
constexpr inline auto default_filter = [](std::filesystem::path const &path) {
    return is_regular_file(path);
};

/**
 * @brief Scan the given folder for files
 */
std::optional<path_list> scan(std::filesystem::path folder, bool(filter)(std::filesystem::path const &) = default_filter);

/**
 * @brief Pick a random path from the given list
 */
std::filesystem::path random_pick(path_list const &paths);

} // namespace wanda

#endif