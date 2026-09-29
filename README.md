# GAME-PUZZLE-Lites_Out

LitesOut is a clone of the Electronic game "Lights Out", ported to OS/2
32-bit Presentation Manager.  The goal is to turn all the lights OFF by
clicking blocks — each click toggles the block and its four neighbors.
An optional Hard mode adds a third state, making it more challenging.

Version 1.1 is a complete rewrite from the original Borland OWL source
to plain C with OpenWatcom 2.0.

## Features

- 6 x 8 grid of toggleable blocks
- Easy mode (on/off) and Hard mode (3-state cycling)
- Top Ten Scores table, separately for Easy and Hard
- 6-language runtime switching (EN, ES, NL, DE, FR, IT)
- Frame Controls toggle: hide/show title bar and menu for full puzzle view
- Persistent settings (difficulty, language, high scores) via litesout.cfg
- Keyboard shortcuts: Ctrl+N, Ctrl+T, Ctrl+F, Ctrl+X
- BLDLEVEL string in executable

## Controls

| Key / Action           | Function                       |
|------------------------|--------------------------------|
| Left-click on a block  | Toggle block and neighbors     |
| Ctrl+N                 | New Game                       |
| Ctrl+T                 | Top Ten Scores                 |
| Ctrl+F                 | Toggle Frame Controls          |
| Ctrl+X                 | Exit                           |

## Build

Requires OpenWatcom 2.0 and OS/2 Toolkit 4.5.  Run on ArcaOS / Warp 4.52:

```
compile-wat.cmd
```

Output: `bin\litesout.exe`

## Project Layout

```
src/            C source, headers, resource script, binary assets
legacy/         Original Borland OWL source (reference only)
doc/            Readme, Changelog, License
bin/            Build output (gitignored)
makefile.wat    OpenWatcom wmake file
compile-wat.cmd Build script for ArcaOS
```

## License

- Original application: (c) 1996, 1997 Herbert Bushong — All Rights Reserved
- 32-bit OS/2 port: (c) 2026 OS2World — BSD 3-Clause License

See [doc/LICENSE.txt](doc/LICENSE.txt) for the full text.

## Authors

- Herbert Bushong (original Borland OWL version)
- OS2World (32-bit PM port)

## Links
- https://github.com/OS2World/GAME-PUZZLE-Lites_Out
- https://www.os2world.com/games/index.php/native-games/puzzle/272-lites-out
