from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain

class SimpleGeometry(ConanFile):
    name = "SimpleGeo"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"
        self.folders.generators = "build"

    def requirements(self):
        self.requires("fmt/11.1.4")
        self.requires("glfw/3.4")
        self.requires("glad/0.1.36")
        self.requires("opengl/system")
        self.requires("imgui/1.91.8")

    def configure(self):
        self.output.info("No specific configuration")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.output.info("No packaging")

    def package_layout(self):
        self.output.info("No packaging")
