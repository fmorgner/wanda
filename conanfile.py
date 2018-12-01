# pylint:  disable=missing-docstring

from conans import ConanFile, CMake

class Wanda(ConanFile):
    name = "wanda"
    version = "1.0.0"
    url = "https://github.com/fmorgner/wanda"
    license = "BSD 3-clause"
    description = "A wallpaper changer for the GNOME"
    generators = "cmake"
    default_user = "fmorgner"
    default_channel = "stable"
    settings = (
        "os",
        "arch",
        "compiler",
        "build_type",
    )
    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
        "src/*",
    )
    requires = (
        "asio/1.12.0@bincrafters/stable",
        "boost_iterator/1.67.0@bincrafters/stable",
        "clara/1.1.5@bincrafters/stable",
        "spdlog/1.2.1@bincrafters/stable",
    )
    default_options = {
        "asio:standalone": True,
        "spdlog:fmt_external": False,
    }

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def package(self):
        self.copy("wanda", src="bin", dst="bin")
