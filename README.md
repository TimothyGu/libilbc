libilbc
=======

[![Build Status](https://travis-ci.org/TimothyGu/libilbc.svg)](https://travis-ci.org/TimothyGu/libilbc)

This is a packaging-friendly copy of the iLBC codec from the WebRTC project. It
provides a base for distribution packages and can be used as drop-in
replacement for the code from RFC 3591.

Compiling
---------

libilbc requires the following to compile:

- A C compiler
- A C++ compiler supporting C++14 or later
- [CMake](https://cmake.org/)
- A [CMake-compatible build
  system](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html);
  some options are:
  - [Ninja](https://ninja-build.org/) (recommended)
  - [GNU Make](https://www.gnu.org/software/make/) for Unix-like systems
  - [Visual Studio](https://visualstudio.microsoft.com/) for Windows

[Abseil Common C++ Libraries](https://github.com/abseil/abseil-cpp) is also a
prerequisite, but it is bundled with release tarballs (or provided as a
submodule for Git users) so there's no need to install it separately.

If you are not using a release tarball, you can clone this repo with:
```sh
git clone --depth=1 https://github.com/TimothyGu/libilbc.git
git submodule update --init
```

### Ninja

```sh
cmake -G 'Ninja' .
ninja
```

### GNU Make

```sh
cmake -G 'Unix Makefiles' .
make -j4
```

### Try it out

After you've done compiling, try out the ilbc\_test program to see if the
library works. With this repo comes a sample.pcm audio file that is in a form
ilbc\_test accepts (raw signed 16-bit PCM, mono, sampled at 8000 Hz).
```sh
./ilbc_test 20 sample.pcm encoded.ilbc decoded.pcm
```

You can try to play the before/after audio with [mpv](https://mpv.io/):
```sh
mpv --demuxer=rawaudio --demuxer-rawaudio-rate=8000 --demuxer-rawaudio-channels=1 sample.pcm
# The above should play "hello one two three".

mpv --demuxer=rawaudio --demuxer-rawaudio-rate=8000 --demuxer-rawaudio-channels=1 decoded.pcm
# The above should play "hello one two three" as well, except it has been
# encoded and decoded using the iLBC codec.
```

You can create your own sample file too. The command I used was:
```sh
ffmpeg -f pulse -i default -f s16le -filter:a 'pan=1|c0=c0+c1,aresample=8000' sample.pcm
```
which gets the audio input from Linux's PulseAudio, and then remixes and
resamples it.

### Installing (Unix-like)

If you want to install the library to system locations, run either `ninja
install` or `make install` depending on which build system you chose. By
default, the library gets installed to /usr/local; to tweak the install prefix,
set `-DCMAKE_INSTALL_PREFIX=<path>` when running `cmake`; see
[docs](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html).
(For more info, we use the
[GNUInstallDirs](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html)
CMake module to determine where things are installed.)

Contributing
------------

Only bug fixes and upstream merges are allowed. If you would like to fix
the source code that would benefit upstream as well, please consider sending
your patch to WebRTC first.

How Do I Merge Upstream Changes?
--------------------------------

Try
```sh
git remote add upstream https://chromium.googlesource.com/external/webrtc
git fetch upstream
git merge upstream/lkgr
# LKGR = latest known good revision
# https://chromium.googlesource.com/chromiumos/docs/+/master/glossary.md
```

Delete all directories other than
* common\_audio/signal\_processing/
* modules/audio\_coding/codecs/ilbc/

Cherry pick what's needed from rtc\_base/. Update the abseil-cpp submodule.

**Always check `git status` before committing the merge** to make sure
nothing unneeded is added!!!

### Changes from upstream

We try to keep the overlaid changes to a minimum. However, here are a few
changes we had to make:
* Add the `ILBC_EXPORT` annotation to various functions in headers.
* Make the following files #include their header:
  * modules/audio\_coding/codecs/ilbc/decode.c
  * modules/audio\_coding/codecs/ilbc/encode.c

### Generating ilbc.h

This packaged library exposes a single header: ilbc.h. Past versions of
upstream WebRTC have this header directly, but this is no longer the case
starting from version 3.0.0. Here are some instructions for synthesizing
ilbc.h:

1. Start with modules/audio\_coding/codecs/ilbc/ilbc.h.
2. Add in the macros and structs from
   modules/audio\_coding/codecs/ilbc/defines.h. Delete all macros and structs
   that were not already previously exposed (e.g., all macros under the "PLC"
   heading and the iLBC\_bits structure).
3. Add in the function prototypes from
   * modules/audio\_coding/codecs/ilbc/init\_decode.h
   * modules/audio\_coding/codecs/ilbc/init\_encode.h
   * modules/audio\_coding/codecs/ilbc/decode.h
   * modules/audio\_coding/codecs/ilbc/encode.h
