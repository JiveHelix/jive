from conan import ConanFile


class JiveConan(ConanFile):
    python_requires = "boiler/0.1"
    python_requires_extend = "boiler.LibraryConanFile"

    name = "jive"
    version = "1.3.0"
    license = "MIT"
    author = "Jive Helix (jivehelix@gmail.com)"
    url = "https://github.com/JiveHelix/jive"
    description = "Miscellaneous tools for C++ development."
    topics = ("utilities", "C++")

    def requirements(self):
        self.requires("fmt/[~10]", transitive_headers=True)

    def build_requirements(self):
        self.test_requires("catch2/2.13.9")
