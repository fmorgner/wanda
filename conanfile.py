import os

from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class Wanda(ConanFile):
    name = "wanda"
    version = "1.0.0"
    url = "https://github.com/fmorgner/wanda"
    license = "BSD 3-clause"
    description = "A wallpaper changer for the GNOME"
    package_type = "library"
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto",
    }
    generators = ("CMakeDeps",)
    options = {
        "nolibs": [True, False],
        "shared": [True, False],
    }
    default_options = {
        "nolibs": False,
        "shared": False,
    }
    settings = (
        "os",
        "arch",
        "compiler",
        "build_type",
    )
    exports_sources = ("source/*",)
    test_requires = ("catch2/[>=3.4]",)
    tool_requires = ("cmake/[>=3.27]",)

    def requirements(self):
        self.requires("boost/[~1.83]", transitive_headers=not self.options.nolibs, options={
            "asio_no_deprecated": True,
            "system_no_deprecated": True,
            "header_only": True,
        })
        self.requires("libjpeg-turbo/[~3.0]")
        self.requires("libpng/[~1.6]")
        self.requires("lyra/[~1.6]")
        self.requires("spdlog/[~1.12]", transitive_headers=not self.options.nolibs, options={
            "header_only": True,
        })

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test(env="CTEST_OUTPUT_ON_FAILURE=1")

    def configure(self):
        if self.options.nolibs:
            self.package_type = "application"

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = True
        toolchain.variables["WANDA_APPLICATIONS_ONLY"] = self.options.nolibs
        toolchain.generate()

    def layout(self):
        cmake_layout(self, src_folder="source")

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.runenv_info.prepend_path("PATH", os.path.join(self.package_folder, "bin"))

        if self.options.nolibs:
            return

        self.cpp_info.components["control"].libs = ["wanda-control"]
        self.cpp_info.components["control"].requires = [
            "meta",
            "proto",
            "std_ext",
            "system",
            # requires
            "boost::headers",
            "spdlog::spdlog",
        ]

        self.cpp_info.components["meta"].libs = []

        self.cpp_info.components["proto"].libs = ["wanda-proto"]
        self.cpp_info.components["proto"].requires = ["spdlog::spdlog",]

        self.cpp_info.components["std_ext"].libs = []

        self.cpp_info.components["system"].libs = ["wanda-system"]
        self.cpp_info.components["system"].requires = [
            "meta",
            "std_ext",
            # requires
            "boost::headers",
            "libjpeg-turbo::jpeg",
            "libpng::libpng",
            "spdlog::spdlog",
        ]

        self.cpp_info.components["wandac"].libs = []
        self.cpp_info.components["wandac"].requires = [
            "control",
            "proto",
            "system",
            # requires
            "lyra::lyra",
            "spdlog::spdlog",
        ]
        self.cpp_info.components["wandac"].bindirs = [
            os.path.join(self.package_folder, "bin")
        ]

        self.cpp_info.components["wandad"].libs = []
        self.cpp_info.components["wandad"].requires = [
            "control",
            "proto",
            "std_ext",
            "system",
            # requires
            "lyra::lyra",
            "spdlog::spdlog",
        ]
        self.cpp_info.components["wandad"].bindirs = [
            os.path.join(self.package_folder, "bin")
        ]

    def validate(self):
        check_min_cppstd(self, "20")
