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
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto",
    }
    generators = ("CMakeDeps",)
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    settings = (
        "os",
        "arch",
        "compiler",
        "build_type",
    )
    exports_sources = ("source/*",)
    requires = (
        "asio/[~1.28]",
        "boost/[~1.83]",
        "libjpeg-turbo/[~3.0]",
        "libpng/[~1.6]",
        "lyra/[~1.6]",
        "spdlog/[~1.12]",
    )
    test_requires = ("catch2/[>=3.4]",)
    tool_requires = ("cmake/[>=3.27]",)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test(env="CTEST_OUTPUT_ON_FAILURE=1")

    def configure(self):
        self.options["boost"].header_only = True
        self.options["fmt"].header_only = True
        self.options["spdlog"].header_only = True

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
        self.runenv_info.prepend_path("PATH", os.path.join(self.package_folder, "bin"))

        self.cpp_info.components["control"].libs = ["wanda-control"]
        self.cpp_info.components["control"].requires = [
            "meta",
            "proto",
            "std_ext",
            "system",
            # requires
            "asio::asio",
            "spdlog::spdlog",
        ]

        self.cpp_info.components["meta"].libs = []

        self.cpp_info.components["proto"].libs = ["wanda-proto"]
        self.cpp_info.components["proto"].requires = ["fmt::fmt"]

        self.cpp_info.components["std_ext"].libs = []

        self.cpp_info.components["system"].libs = ["wanda-system"]
        self.cpp_info.components["system"].requires = [
            "meta",
            "std_ext",
            # requires
            "boost::boost",
            "fmt::fmt",
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
            "asio::asio",
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
            "asio::asio",
            "lyra::lyra",
            "spdlog::spdlog",
        ]
        self.cpp_info.components["wandad"].bindirs = [
            os.path.join(self.package_folder, "bin")
        ]

    def validate(self):
        check_min_cppstd(self, "20")
