#ifndef WANDA_MAGIC_HPP
#define WANDA_MAGIC_HPP

#include <magic.h>

#include <filesystem>
#include <memory>
#include <string>
#include <type_traits>

namespace wanda
{
  struct magic
  {
    struct closer
    {
      auto operator()(magic_t handle) const noexcept -> void
      {
        magic_close(handle);
      }
    };

    enum struct mime_type
    {
      unknown,
      image_jpeg,
      image_png,
    };

    magic()
        : m_handle{magic_open(MAGIC_MIME_TYPE)}
    {
      magic_load(m_handle.get(), nullptr);
    }

    auto type(std::filesystem::path path) -> mime_type
    {
      auto magic_type = std::string{magic_file(m_handle.get(), path.native().c_str())};

      if (magic_type == "image/jpeg")
      {
        return mime_type::image_jpeg;
      }
      else if (magic_type == "image/png")
      {
        return mime_type::image_png;
      }

      return mime_type::unknown;
    }

  private:
    std::unique_ptr<std::remove_pointer_t<magic_t>, closer> m_handle;
  };

}  // namespace wanda

#endif