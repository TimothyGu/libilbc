libilbc
=======

[![Build Status](https://travis-ci.org/TimothyGu/libilbc.svg)](https://travis-ci.org/TimothyGu/libilbc)

This is a packaging friendly copy of the iLBC codec from the WebRTC
project. It provides a base for distribution packages and can be used
as drop-in replacement for the non-free code from RFC 3591.

Contributing
------------

Only bug fixes and upstream merges are allowed. If you would like to fix
the source code that would benefit upstream as well, please consider sending
your patch to WebRTC first.

How Do I Merge Upstream Changes?
--------------------------------

Try
```
git remote add upstream https://chromium.googlesource.com/external/webrtc
git fetch upstream
git merge upstream/master
```

Delete all directories other than
* common\_audio/signal\_processing/
* modules/audio\_coding/codecs/ilbc/

Cherry pick what's needed from api/ and rtc\_base/. Update the abseil-cpp
header.

**Always check `git status` before committing the merge** to make sure
nothing unneeded is added!!!

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
