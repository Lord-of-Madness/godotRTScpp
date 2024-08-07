#!/usr/bin/env python
import os
import sys

#path to godot-cpp SConstruct file
env = SConscript("godot-cpp/SConstruct")


# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["Project1/"])
sources = Glob("Project1/*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "../../godot-rts/DLLs/lib.{}.{}.framework/lib.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "../../godot-rts/DLLs/lib{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
