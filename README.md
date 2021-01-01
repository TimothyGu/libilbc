libilbc
=======

![Build Status](https://github.com/TimothyGu/libilbc/workflows/CMake/badge.svg)

This is a packaging-friendly copy of the iLBC codec from the WebRTC project. It
provides a base for distribution packages and can be used as drop-in
replacement for the code from RFC 3591.

Compiling
---------

libilbc requires the following to compile:

- A C compiler
- A C++ compiler supporting C++14 or later
- [CMake](https://cmake.org/)
- A [CMake-compatible build system][cmake-generators];
  some options are:
  - [Ninja](https://ninja-build.org/) (recommended)
  - [GNU Make](https://www.gnu.org/software/make/) for Unix-like systems
  - [Visual Studio](https://visualstudio.microsoft.com/) for Windows

[Abseil Common C++ Libraries](https://github.com/abseil/abseil-cpp) is also a
prerequisite, but it comes bundled with release tarballs (or provided as a
submodule for Git users) so there's no need to install it separately.

0. If you are not using a release tarball, you can clone this repo with:
   ```sh
   git clone --depth=1 https://github.com/TimothyGu/libilbc.git
   git submodule update --init
   ```

1. **[Generate build files][cmake-generate]:** `cmake .`

   If you instead want a static library, instead run `cmake
   -DBUILD_SHARED_LIBS=OFF .` per [CMake docs][BUILD_SHARED_LIBS].

2. **[Build it][cmake-build]:** `cmake --build .`

   You should now get a library file (.so, .dylib, .dll, .a, or .lib depending
   on your platform) as well as a ilbc\_test program.

3. (optional) **Try it out.** This repo comes a sample.pcm audio file that is
   in a form ilbc\_test accepts (raw signed 16-bit PCM, mono, sampled at 8000
   Hz). The following command encodes sample.pcm to encoded.ilbc, and then
   decode it again to decoded.pcm.
   ```sh
   ./ilbc_test 20 sample.pcm encoded.ilbc decoded.pcm
   ```
   You can try to play the before/after audio with [mpv][]:
   ```sh
   mpv --demuxer=rawaudio --demuxer-rawaudio-rate=8000 --demuxer-rawaudio-channels=1 sample.pcm
   mpv --demuxer=rawaudio --demuxer-rawaudio-rate=8000 --demuxer-rawaudio-channels=1 decoded.pcm
   ```

   You can create your own sample file too. The command I used was:
   ```sh
   ffmpeg -f pulse -i default -f s16le -filter:a "pan=1|c0=c0+c1,aresample=8000" sample.pcm
   ```
   which gets the audio input from Linux's PulseAudio, and then remixes and
   resamples it.

4. (optional; for Unix-like systems) **Installing.** If you want to install the
   library and the ilbc\_test utility to system locations, run either `ninja
   install` or `make install` depending on which build system you chose (or
   [`cmake --install .`][cmake-install] on CMake 3.15 or later). By default,
   the library gets installed to /usr/local; to tweak the install prefix, set
   `-DCMAKE_INSTALL_PREFIX=<path>` when running `cmake`; see
   [docs][CMAKE_INSTALL_PREFIX].

Supported platforms
-------------------

The following platforms are regularly run on CI and are thus supported:

* Linux
  * ARMv5–7 (32-bit; soft and hard float)
  * ARMv8 (64-bit)
  * PowerPC (little-endian) (64-bit)
  * MIPS (little-endian) (64-bit)
  * x86-64
* macOS on x86-64
* Windows
  * x86-64 with Visual C++ 2019
  * x86-64 with [clang-cl](https://clang.llvm.org/docs/UsersManual.html#clang-cl)

The following architectures get cross-compiled on CI and thus probably work.
But we don't know for sure if it actually runs:

* Linux
  * MIPS (little-endian) (32-bit)
  * PowerPC (big-endian) (32/64-bit)
  * RISC-V (64-bit)
  * SPARC (64-bit)
* Windows
  * ARM64 with Visual C++ 2019

These platforms are known to _not_ work:

* Linux
  * MIPS (big-endian) (32-bit)
* Windows
  * ARM with Visual C++ 2019 (32-bit)

All other platforms _may_ work out of the box. If they don't, a simple change
to rtc\_base/system/arch.h will most likely fix it up.

For packagers
-------------

There are some additional CMake options available to the fine folks packaging
this library. For the most part, we use the
[GNUInstallDirs](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html)
CMake module to determine where things are installed, which should generally do
the right thing. If it doesn't, try:

- `-DCMAKE_INSTALL_PREFIX=<dir>` for the prefix (usually defaults to
  /usr/local)
- `-DCMAKE_INSTALL_BINDIR=<dir>` for executables and Windows DLLs
- `-DCMAKE_INSTALL_DOCDIR=<dir>` for various Markdown documentation files
- `-DCMAKE_INSTALL_INCLUDEDIR=<dir>` for C headers
- `-DCMAKE_INSTALL_LIBDIR=<dir>` for static libraries (`.a`), Windows DLL
  import libraries (`.lib`), and non-Windows dynamic libraries (`.so` and
  `.dylib`)

[BUILD_SHARED_LIBS]: https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html
[cmake-build]: https://cmake.org/cmake/help/latest/manual/cmake.1.html#build-a-project
[cmake-install]: https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project
[cmake-generate]: https://cmake.org/cmake/help/latest/manual/cmake.1.html#generate-a-project-buildsystem
[cmake-generators]: https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html
[CMAKE_INSTALL_PREFIX]: https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html
[GNUInstallDirs]: https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html
[mpv]: https://mpv.io/
