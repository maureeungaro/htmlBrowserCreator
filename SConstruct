from init_env import init_environment

env = init_environment("qt5 mlibrary")
# manually appending this library so the example can be compiled
# the environment will not load the library if it's not there
env.Append(LIBS = 'htmlImageBrowserCreator')

# calling SConscript here to build just the library as in the hierarchy build
lib = SConscript('SConscript')

env.Append(LIBPATH = 'lib')

example = env.Program(source = "pageCreator.cc", target = "pageCreator")

Depends(example,  lib);
