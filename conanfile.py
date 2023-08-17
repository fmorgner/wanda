import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class Wanda(ConanFile):
    name = "wanda"
    version = "1.0.0"
    url = "https://github.com/fmorgner/wanda"
    license = "BSD 3-clause"
    description = "A wallpaper changer for the GNOME"
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto",
    }
    generators = (
        "CMakeDeps",
    )
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    settings = (
        "os",
        "arch",
        "compiler",
        "build_type",
    )
    exports_sources = (
        "source/*",
    )
    requires = (
        "asio/[~1.28]",
        "boost/[~1.82]",
        "libjpeg/9e",
        "libpng/[~1.6]",
        "lyra/[~1.6]",
        "spdlog/[~1.12]",
    )
    tool_requires = (
        "cmake/[>=3.27]",
    )

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = True
        toolchain.generate()

    def layout(self):
        cmake_layout(self, src_folder="source")

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["wanda"]
