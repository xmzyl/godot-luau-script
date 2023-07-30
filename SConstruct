#!/usr/bin/env python

import os

env = Environment(tools=["default", "compilation_db"], PLATFORM="")
cdb = env.CompilationDatabase()
Alias("cdb", cdb)

# clang terminal colors
if "TERM" in os.environ:
    env["ENV"]["TERM"] = os.environ["TERM"]

Export("env")
SConscript("extern/godot-cpp/SConstruct")

# We do not want to export any symbols we don't need to.
# Strictly speaking, only the init function must be exported.
if not env.get("is_msvc", False):
    env.Append(CXXFLAGS=["-fvisibility=hidden"])

if env["platform"] == "macos":
    env.Append(RANLIBFLGS="-no_warning_for_no_symbols")

# Using this option makes a warning. Too bad!
opts = Variables([], ARGUMENTS)

opts.Add(BoolVariable("generate_luau_bindings",
         "Force generation of Luau bindings", False))
opts.Add(BoolVariable("tests", "Build tests", False))
opts.Add(BoolVariable("iwyu", "Run include-what-you-use on main source", False))

opts.Update(env)

env_base = env.Clone()
env_base["CPPDEFINES"] = []  # irrelevant to externs

Export("env_base")
SConscript("extern/SCSub_Luau.py")
SConscript("SCSub_bindgen.py")

env_main = env.Clone()

if env["iwyu"]:
    env_main["CC"] = "include-what-you-use"
    env_main["CXX"] = "include-what-you-use"

sources = Glob("src/*.cpp")

env_main.Append(CPPPATH=["src/"])

# Catch2
if env["tests"]:
    if env["target"] != "editor":
        raise ValueError("Tests can only be enabled on the editor target")

    env_main.Append(CPPDEFINES="TESTS_ENABLED", CPPPATH=["extern/Catch2/extras/"])
    sources.append(File("extern/Catch2/extras/catch_amalgamated.cpp"))

    sources += Glob("tests/*.cpp")
    env_main.Append(CPPPATH=["tests/"])

if env["platform"] == "macos":
    library = env_main.SharedLibrary(
        "bin/luau-script/libluau-script.{}.{}.framework/libluau-script.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env_main.SharedLibrary(
        "bin/luau-script/libluau-script{}{}".format(
            env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
