LITESOUT for OS/2 -- Version 1.1
=================================

LitesOut is an OS/2 32-bit Presentation Manager puzzle game based on
the classic Electronic Arts "Lights Out" handheld game.

The goal is to turn all the lights OFF. Clicking any block toggles that
block and its four orthogonal neighbors. Two difficulty modes are
available: Easy (3-state blocks) and Hard (cycling blocks).


CONTROLS
--------
  Left-click on a block   Toggle block and neighbors
  Ctrl+N                  New Game
  Ctrl+T                  Top Ten Scores
  Ctrl+F                  Toggle Frame Controls (title bar and menu)
  Ctrl+X                  Exit


MENUS
-----
  Game > New Game         Start a new scrambled puzzle
  Game > Exit             Quit the program

  Options > Hard Mode     Toggle Easy/Hard difficulty
  Options > Top Ten Scores  View the high-score table
  Options > Language      Switch display language (EN/ES/NL/DE/FR/IT)
  Options > Frame Controls  Show or hide the title bar and menu
  Options > Save settings on exit  Persist settings to litesout.cfg

  Help > About            Show version and license information


SETTINGS FILE
-------------
Settings are saved to LITESOUT.CFG in the current working directory
when "Save settings on exit" is enabled.  The file stores difficulty
mode, current language, and the Top Ten score table (20 entries,
10 per difficulty level).


LANGUAGES
---------
  English, Espanol, Nederlands, Deutsch, Francais, Italiano


BUILD REQUIREMENTS
------------------
  OpenWatcom 2.0 (c:\watcom2 or c:\watcom)
  OS/2 Toolkit 4.5 headers  (c:\os2tk45)

  Run compile-wat.cmd on ArcaOS / Warp 4.52 to produce bin\litesout.exe


AUTHORS
-------
  Original (Borland OWL):  Herbert Bushong (c) 1996, 1997
  OS/2 32-bit PM port:     OS2World (c) 2026


LICENSE
-------
  Original application:    All Rights Reserved, Herbert Bushong
  32-bit port:             BSD 3-Clause License (see LICENSE.txt)


LINKS
-----
  https://www.os2world.com/games/index.php/native-games/puzzle/272-lites-out
