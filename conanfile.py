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
    build_policy = "missing"
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
        "asio/1.18.1",
        "lyra/1.5.1",
        "CUTE/2.2.6@fmorgner/stable",
        "spdlog/1.4.2",
        "boost/1.75.0",
        "libpng/1.6.37",
        "libjpeg/9d",
    )

    def configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["wanda"]
