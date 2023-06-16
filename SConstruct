from init_env import init_environment

env = init_environment("qt5 mlibrary")

# latest clang does not expose c++11+ by default anymore?
# need to add a standard manually
env.Append(CXXFLAGS=['-std=c++14'])
env.Append(LIBPATH = 'lib')

# manually appending this library so the example can be compiled
# the environment will not load the library if it's not there
env.Append(LIBS = 'htmlImageBrowserCreator')

# building the library
lib = env.Library(source = "htmlImageBrowserCreator.cc", target = "lib/htmlImageBrowserCreator")


example = env.Program(source = "pageCreator.cc", target = "pageCreator")

Depends(example,  lib)
