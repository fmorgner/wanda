# pylint:  disable=missing-docstring

from conans import ConanFile, CMake

class Wanda(ConanFile):
    name = "wanda"
    version = "0.1"
    license = "BSD 3-clause"
    description = "A wallpaper changer for the GNOME"
    generators = "cmake"
    export_sources = (
        "CMakeLists.txt",
        "src/*",
    )
    requires = (
        "boost_program_options/1.67.0@bincrafters/stable",
    )

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

    def package(self):
        pass
