from conans import ConanFile, CMake


class JiveConan(ConanFile):
    name = "jive"
    version = "1.0.16"

    scm = {
        "type": "git",
        "url": "https://github.com/JiveHelix/jive.git",
        "revision": "auto",
        "submodule": "recursive"}

    license = "MIT"
    author = "Jive Helix (jivehelix@gmail.com)"
    url = "https://github.com/JiveHelix/jive"
    description = "Miscellaneous tools for C++ development."
    topics = ("utilities", "C++")

    settings = "os", "compiler", "build_type", "arch"

    generators = "cmake"

    options = {
        "CMAKE_TRY_COMPILE_TARGET_TYPE":
            ["EXECUTABLE", "STATIC_LIBRARY", None],
        "fPIC": [True, False]}

    default_options = {
        "CMAKE_TRY_COMPILE_TARGET_TYPE": None,
        "fPIC": False}

    # Default behavior is to copy all sources to the build folder.
    # Our CMake configuration requires out-of-source builds.
    no_copy_source = True

    def build(self):
        cmake = CMake(self)

        if (self.options.CMAKE_TRY_COMPILE_TARGET_TYPE):
            cmake.definitions["CMAKE_TRY_COMPILE_TARGET_TYPE"] = \
                self.options.CMAKE_TRY_COMPILE_TARGET_TYPE

        if (self.options.fPIC):
            cmake.definitions["fPIC"] = self.options.fPIC

        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.includes = ["include"]
        self.cpp_info.libs = ["jive"]

    def build_requirements(self):
        self.test_requires("catch2/2.13.8")
