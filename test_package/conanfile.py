import os

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run


class WandaTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        if self._apps_only:
            return

        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if not can_run(self):
            return

        if self._apps_only:
            self.run("wandad -h", env="conanrun")
            self.run("wandac -h", env="conanrun")
        else:
            cmd = os.path.join(self.cpp.build.bindir, "test")
            self.run(cmd, env="conanrun")

    @property
    def _apps_only(self):
        return self.dependencies[self.tested_reference_str].options.nolibs
