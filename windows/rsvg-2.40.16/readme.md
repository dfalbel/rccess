## Graphics stack

Builds of static C libraries for graphics using msys2. Compatible with
both the old gcc-4.6.3 as well as the newer gcc 4.9.3 R toolchain.

## Problems

Building everything with msys2. Two problems:

 - Undefined references to `__imp_g_ascii_table and `__imp_g_utf8_skip`. Solution: rebuild with `-DGLIB_STATIC_COMPILATION`.
 - Multiple `DllMain` defintions. Solution: patches to use constructors instead of DllMain.
 - Need to avoid mingw versions of `mkstemp` and `strtok` and `strtod` for gcc 4.6.3 compatibility.

## Static linking with GLIB2

glib2 and gdk-pixbuf need DllMain patches:

 - https://github.com/Alexpux/MINGW-packages/pull/1611

Alternative patches are available from Fedora:

 - https://smani.fedorapeople.org/glib-prefer-constructors-over-DllMain.patch
 - https://smani.fedorapeople.org/gdk-pixbuf_static.patch

To build with msys but make it link with the old toolchain you need to add

#define strtod __strtod

Inside glib/gstring.c after the includes. This uses the windows
implementation of strtod instead of the new one from mingw V3.
Build both with:

	CFLAGS="-D_POSIX_SOURCE -DLIBXML_STATIC -DGLIB_STATIC_COMPILATION"

And `--enable-static --disable-shared --with-threads=win32`. Note that
currently `--with-threads=posix` does not seem to work. 

Also as of 2016 mingw runtime has a new feature that redefines 
DnsRecordListFree into the new DnsFree (which is not available in
gcc 4.6.3 and gcc 4.9.3). So we need to undo that:

	#undef DnsRecordListFree
	VOID WINAPI DnsRecordListFree(PDNS_RECORD pRecordList,DNS_FREE_TYPE FreeType);

See https://sourceforge.net/p/mingw-w64/mailman/message/34821938/

## Building pango

Need to build static library with `CFLAGS="-DGLIB_STATIC_COMPILATION"`.

## Building librsvg2

Hack to use internal `strtok` implementation:

	sed -i.bak s/mkstemp/blablbla/g configure.ac
	sed -i.bak s/HAVE_STRTOK_R/DOESNOTEXIST_BLABLA/g rsvg-css.c  
	sed -i.bak s/strtok_r/strtok_rwinlib/g rsvg-css.c 

Then build in msys2 with:

	CFLAGS="-D_POSIX_SOURCE -DLIBXML_STATIC -DGLIB_STATIC_COMPILATION"

That should do it.

## Building libharfbuzz

You either need to build `--without-graphite2` or with:

	CFLAGS="-DGRAPHITE2_STATIC"
	CPPFLAGS="-DGRAPHITE2_STATIC"

Note that graphite2 C++ library is sensitive to the compiler version.

## Fontconfig

Patch source before running `autoreconf`:

	sed -i.bak s/mkstemp/blablabla/g configure.ac
	sed -i.bak s/strtod/__strtod/g src/fcxml.c
	sed -i.bak s/strtod/__strtod/g src/fcname.c

This builds using native windows `mktemp_s` and `strtod`.

## Zlib

Build with `CFLAGS="-DNO_vsnprintf" for gcc 4.6.3 compatibility.
Note that you do this for `zlib` and not `minizip`.
