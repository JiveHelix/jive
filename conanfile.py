from conans import ConanFile, CMake
# from conan.tools.cmake import CMakeToolchain
# from conan.tools.layout import cmake_layout


class JiveConan(ConanFile):
    name = "jive"
    version = "1.0.0"

    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto"}

    # Optional metadata
    license = "MIT"
    author = "Jive Helix (jivehelix@gmail.com)"
    url = "https://github.com/JiveHelix/jive"
    description = "Miscellaneous tools for C++ development."
    topics = ("utilities", "C++")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    generators = "cmake"

    # Default behavior is to copy all sources to the build folder.
    # Our CMake configuration requires out-of-source builds.
    no_copy_source = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.includes = ["include"]
        self.cpp_info.libs = ["jive"]

    def build_requirements(self):
        self.test_requires("catch2/2.13.8")