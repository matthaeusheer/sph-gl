target("techcamp")
  set_kind("binary")
  -- set_toolset("cxx", "clang")
  -- set_toolset("ld", "clang++")

  set_languages("cxx11", "c99")

  -- main files
  add_files("main.cpp")
  add_files("src/utils/*.cpp")
  add_files("src/graphics/buffers/*.cpp")
  add_files("src/graphics/*.cpp")
  add_includedirs(".")
  add_includedirs("src/graphics")
  add_includedirs("src/utils")

  -- dependencies
  add_files("Dependencies/GLAD/glad.c")
  add_includedirs("Dependencies/KHR/include/")
  add_includedirs("Dependencies/GLFW/include/")
  add_includedirs("Dependencies/GLAD/include/")
  add_includedirs("Dependencies/GLEW/include/")
  add_includedirs("Dependencies/StdImage/include/")
  add_includedirs("Dependencies/glm/include/")
