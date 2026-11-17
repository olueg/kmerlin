# KMerlin 1.4.2 — Museum piece

**KMerlin** is a MSN Messenger client for KDE 3 / Qt 3, written in C++.
It was developed around 2002–2004 by Olaf Lueg and the KMerlin developer team,
and released under the GNU General Public License v2.

This repository preserves the original 1.4.2 source tree as a historical artifact.
It is **not maintained** and will not build on modern systems without a
KDE 3 / Qt 3 toolchain (and the `libXss` / XScreenSaver development headers).

## Features (at the time)

- MSN Messenger protocol support (contacts, groups, presence/status)
- Chat windows with emoticons, custom fonts/colors, text replacement
- System tray icon with animated status
- Idle/away detection (XScreenSaver)
- Multi-profile support, contact import/export
- KDE integration: DCOP, session management, KNotify, KConfig
- Translations: ar, ca, de, es, fr, is, nb, nl, zh-CN, zh-TW

## Building (historical)

```sh
make -f Makefile.cvs   # generates configure
./configure --prefix=/usr
make
make install
```

Requires KDE 3.x, Qt 3.x, and `libXss` (XScreenSaver). See `configure.in.in`.

## License

GPL v2 — see `COPYING`.

## Author

Olaf Lueg <olueg@olsd.de> (and contributors listed in `src/main.cpp`).
EOF