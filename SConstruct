from init_env import init_environment

env = init_environment("mlibrary")
# manually appending this library so the example can be compiled
# the environment will not load the library if it's not there
env.Append(LIBS = 'textProgressBar')

# calling SConscript here to build just the library as in the hierarchy build
lib = SConscript('SConscript')

example = env.Program(source = "example.cc", target = "example")

Depends(example,  lib);
