#include "wanda/logging.hpp"
#include "wanda/magic.hpp"
#include "wanda/optional.hpp"
#include "wanda/setting.hpp"
#include "wanda/wallpaper.hpp"

#include <boost/gil.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/numeric/resample.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>

#include <fmt/format.h>

#include <algorithm>
#include <cmath>
#include <memory>

namespace wanda
{
  namespace
  {
    auto magic_instance = magic{};

    auto load_image(std::filesystem::path wallpaper)
    {
      auto image = boost::gil::rgb8_image_t{};

      switch (magic_instance.type(wallpaper))
      {
        case magic::mime_type::image_jpeg:
          boost::gil::read_and_convert_image(wallpaper.native(), image, boost::gil::jpeg_tag{});
          break;
        case magic::mime_type::image_png:
          boost::gil::read_and_convert_image(wallpaper.native(), image, boost::gil::png_tag{});
          break;
      }

      return image;
    }

    auto average_colors(boost::gil::rgb8_image_t image)
    {
      auto accumulator = boost::gil::rgb64f_pixel_t{};
      auto view = const_view(image);

      std::ranges::for_each(view, [&](auto const & source_pixel) {
        at_c<0>(accumulator) += std::pow(boost::gil::at_c<0>(source_pixel), 2);
        at_c<1>(accumulator) += std::pow(boost::gil::at_c<1>(source_pixel), 2);
        at_c<2>(accumulator) += std::pow(boost::gil::at_c<2>(source_pixel), 2);
      });

      at_c<0>(accumulator) = std::sqrt(at_c<0>(accumulator) / view.size());
      at_c<1>(accumulator) = std::sqrt(at_c<1>(accumulator) / view.size());
      at_c<2>(accumulator) = std::sqrt(at_c<2>(accumulator) / view.size());

      return accumulator;
    }

  }  // namespace

  void set_wallpaper(std::filesystem::path wallpaper)
  {
    using namespace wanda::literals;
    using namespace wanda::std_ext;
    using namespace std::string_literals;

    auto image = load_image(wallpaper);
    auto color = average_colors(std::move(image));
    auto hexstring = fmt::format("#{:02X}{:02X}{:02X}",
                                 static_cast<std::uint8_t>(at_c<0>(color)),
                                 static_cast<std::uint8_t>(at_c<1>(color)),
                                 static_cast<std::uint8_t>(at_c<2>(color)));

    with("org.gnome.desktop.background"_setting, [&](auto & setting) {
      with(setting["primary-color"_key], [&](auto & value) {
        value = hexstring;
      });
      with(setting["picture-uri"_key], [&](auto & value) {
        value = "file://" + wallpaper.native();
      }) ||
          [&] { get_logger()->error("invalid settings key"); };
    }) ||
        [&] { get_logger()->error("invalid setting"); };
  }

}  // namespace wanda