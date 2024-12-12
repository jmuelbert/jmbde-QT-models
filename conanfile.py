from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy, get, rmdir
from conan.tools.scm import Git
from conan.tools.build import check_min_cppstd
import os


class JmbdeModelsConan(ConanFile):
    """
    Conan package recipe for jmbdemodels library.
    This library provides the data models for the jmbde application.
    """

    name = "jmbdemodels"
    version = "0.7"

    # Metadata
    license = "GPL-V3-or-later"
    author = "Jürgen Mülbert <juergen.muelbert@gmail.com>"
    url = "https://github.com/jmuelbert/jmbde-QT-models"
    homepage = "https://github.com/jmuelbert/jmbde-QT-models"
    description = "The models lib for the jmbde datamodel"
    topics = ("jmbde", "models", "datamodel", "qt", "qt6")

    # Package configuration
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    options = {
        "shared": [True, False],
        "build_static": [True, False],
        "build_examples": [True, False],
        "build_tests": [True, False],
        "build_docs": [True, False],
        "build_api_docs": [True, False],
        "fPIC": [True, False],
        "with_coverage": [True, False],
    }

    default_options = {
        "shared": False,
        "build_static": False,
        "build_examples": False,
        "build_tests": False,
        "build_docs": True,
        "build_api_docs": True,
        "fPIC": True,
        "with_coverage": False,
        "qt/*:shared": True,
    }

    # Source files to include
    exports_sources = (
        "CMakeLists.txt",
        "CMakeLists_files.cmake",
        "cmake/*",
        "src/*",
        "include/*",
        "test_package/*"
    )

    def requirements(self):
        """Define runtime requirements."""
        self.requires("qt/[>=6.5.0 <7.0.0]")
        self.requires("extra-cmake-modules/[>=5.112.0]")

        # Documentation generation requirements
        if self.options.build_docs or self.options.build_api_docs:
            self.requires("doxygen/1.9.4", build=True)
            # self.requires("sphinx/5.3.0", build=True)
            # self.requires("breathe/4.34.0", build=True)

    def build_requirements(self):
        """Define build-time requirements."""
        self.test_requires("gtest/1.13.0")
        if self.options.with_coverage:
            self.tool_requires("lcov/1.16")

    def validate(self):
        """Verify that package requirements are met."""
        if self.settings.compiler.cppstd:
            check_min_cppstd(self, "17")

        if self.info.settings.compiler == "gcc" and self.info.settings.compiler.version < "7":
            raise ConanInvalidConfiguration("GCC >= 7 is required")

    def configure(self):
        """Configure build options."""
        if self.options.shared:
            self.options.rm_safe("fPIC")

        # Configure Qt components
        self.options["qt"].qtsvg = True
        self.options["qt"].qttools = True
        self.options["qt"].shared = True


    def layout(self):
        """Define the layout of build files."""
        cmake_layout(self)

    def generate(self):
        """Generate build system files."""
        tc = CMakeToolchain(self)

        # Set CMake variables
        tc.variables["BUILD_STATIC"] = self.options.build_static
        tc.variables["BUILD_EXAMPLES"] = self.options.build_examples
        tc.variables["BUILD_TESTS"] = self.options.build_tests
        tc.variables["BUILD_DOCS"] = self.options.build_docs
        tc.variables["BUILD_API_DOCS"] = self.options.build_api_docs
        tc.variables["WITH_COVERAGE"] = self.options.with_coverage

        # Generate toolchain file
        tc.generate()

        # Generate dependency files
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        """Build the package."""
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        # Run tests if enabled
        if self.options.build_tests:
            cmake.test(output_on_failure=True)

        # Build documentation if enabled
        if self.options.build_docs:
            cmake.build(target="docs")
        if self.options.build_api_docs:
            cmake.build(target="api_docs")

    def package(self):
        """Package artifacts."""
        cmake = CMake(self)
        cmake.install()

        # Copy license and documentation
        copy(self, "LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        if self.options.build_docs:
            copy(self, "*", src=os.path.join(self.build_folder, "docs/html"),
                 dst=os.path.join(self.package_folder, "docs"))
        if self.options.build_api_docs:
            copy(self, "*", src=os.path.join(self.build_folder, "docs/api"),
                 dst=os.path.join(self.package_folder, "docs/api"))

        # Remove unnecessary directories
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))

    def package_info(self):
        """Define package information."""
        self.cpp_info.set_property("cmake_file_name", "JmbdeModels")
        self.cpp_info.set_property("cmake_target_name", "JmbdeModels::JmbdeModels")

        # Component definition
        lib_name = "jmbdemodelsd" if self.settings.build_type == "Debug" else "jmbdemodels"
        self.cpp_info.components["libjmbdemodels"].libs = [lib_name]
        self.cpp_info.components["libjmbdemodels"].requires = [
            "qt::qtCore",
            "qt::qtSvg",
            "qt::qtTools"
        ]

        # System libraries
        if self.settings.os == "Windows":
            self.cpp_info.components["libjmbdemodels"].system_libs.extend([
                "ws2_32",
                "kernel32",
                "user32",
                "gdi32"
            ])

        # Pkg-config
        self.cpp_info.set_property("pkg_config_name", "jmbdemodels")

        # Set version in package metadata
        self.cpp_info.version = self.version
