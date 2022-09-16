import os

from conans import ConanFile
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
        "asio/[~=1.24.0]",
        "boost/[~=1.80.0]",
        "libjpeg/9d",
        "libpng/[~=1.6.0]",
        "lyra/[~=1.6.0]",
        "spdlog/[~=1.10.0]",
    )
    tool_requires = (
        "cmake/[~=3.24]",
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
        self.env_info.PATH.append(os.path.join(self.package_folder, "bin"))
