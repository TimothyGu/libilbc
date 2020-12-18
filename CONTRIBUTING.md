Contributing
============

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
* Add the `ILBC_EXPORT` annotation to various functions in headers
* Make the following files #include their header:
  * modules/audio\_coding/codecs/ilbc/decode.c
  * modules/audio\_coding/codecs/ilbc/encode.c
* Support a few more platforms in rtc\_base/system/arch.h

### Generating ilbc.h

This packaged library exposes a single header: ilbc.h. Past versions of
upstream WebRTC have this header directly, but starting from version 3.0.0 we
maintain our own copy of it. Here are some instructions for updating it:

1. Start with modules/audio\_coding/codecs/ilbc/ilbc.h.
2. Add in the macros and structs from
   modules/audio\_coding/codecs/ilbc/defines.h. Delete all macros and structs
   that were not already previously exposed (e.g., all macros under the
   ["PLC"](https://source.chromium.org/chromium/chromium/src/+/master:third_party/webrtc/modules/audio_coding/codecs/ilbc/defines.h;l=96;drc=a12c42a6b2d979c1bca1c4263354335fa6bd759d)
   heading and the iLBC\_bits structure).
3. Add in the function prototypes from
   * modules/audio\_coding/codecs/ilbc/init\_decode.h
   * modules/audio\_coding/codecs/ilbc/init\_encode.h
   * modules/audio\_coding/codecs/ilbc/decode.h
   * modules/audio\_coding/codecs/ilbc/encode.h
4. Add a few deprecated typedefs for backwards compatibility.
