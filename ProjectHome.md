The FCS2.0 (and the 1.0/3.0 and XML serializations) suffer from a small, closed and proprietary community. Most tools are years (if not decades) out of date, and many have not been updated within recent memory.

This project provides a small ANSI/ISO C++ (2003) compliant FCS2.0 library. The library reads liberally, that is, it can read many of the FCS2.0 variants in the wild; it writes as strictly as possible.

If you have a bug, please include the example file that the FCS- reader or writer dies on.

The library is released under the modified BSD license, but the "tools" (executables) are GPLed.

**Current News**

  * First release ([FirstRelease](FirstRelease.md)) of the toolset
  * Jim has provided an example FCS3.0 and some documentation for reading and writing 3.0; I have included those bits which are pertinent
  * FCS 2.0 documentation? if anyone has unencumbered documentation of the keyword sets I'd like a copy!
  * FCS 1.0 documentation? I know it's been >20 years, but it _has_ to exist somewhere!
  * Bug fix for FCS IO Reader (stack-smash)