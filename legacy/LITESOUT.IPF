.*==============================================================*\
.*                                                              *
.* litesout.ipf - Information Tag Language file LitesOut for OS2*
.*                                                              *
.*==============================================================*/
:userdoc.
:title.Lites Out Help
.*--------------------------------------------------------------*\
.*  Main window extended help panel                             *
.*      res = PANEL_MAIN                                        *
.*--------------------------------------------------------------*/
:h1 res=100 name=PANEL_MAIN.Introduction
:i1 id=Introduction.Introduction
:p.Lites Out for OS/2 is similar to the popular electronic game
but with an interesting twist. The game can be played in two modes.
:p.EASY MODE - In this mode, the game plays like the original
electronic game. The object of the game is to get all the blocks
unlit. Every time you switch one block, the four blocks surrounding
(above, below, left, and right) also switch states.
:p.HARD MODE - In this mode, the object is the same, but each block
can have 3 states; OFF, ON1 (yellow), or ON2 (blue). The direction
the blocks change depend on the direction it was going and the
state of the main block that is being switched.


:h1 res=110 name=PANEL_MENU.Menu Options
:i1 id=Menu.Menu Options
:p.The Menu options allow you to switch modes, reset the board, 
see the Top Ten Scores for each mode, show the product information, 
and display help.

:h2 res=111.Mode
:i2 refid=Menu.Mode
:p.The MODE menu choice pops a dialog box up asking you if you wish to change
modes. If you answer YES, the mode is changed and the board is reset.

:h2 res=112.Reset
:i2 refid=Menu.Reset
:p.The RESET menu choice forces the game to reset the board using the current
mode of difficulty.

:h2 res=113.Top Ten
:i2 refid=Menu.Top Ten
:p.The TOP TEN menu choice displays a dialog box with the 10 best scores for
the current mode of difficulty. There are two different lists, one for
each mode.

:h2 res=114.About
:i2 refid=Menu.About
:p.The ABOUT menu choice pops a dialog box displaying the current product
information and version.

:h2 res=115.Help
:i2 refid=Menu.Help
:p.The HELP menu choice loads this help file. Note&colon. This actually calls
VIEW.EXE to load the INF file. VIEW.EXE should be in you path.

:h1 res=120 name=PANEL_WINDOW.Window Display
:i1 id=Window.Main Window
:p.The Main Window consists of the game grid, the current number of hits (times
you've switched a blocks state), and the current mode of play.
:p.To switch a block, click on it with the mouse pointer.
:p.Hits and Mode are updated automatically as you make choices.

:h1 res=500 name=PANEL_AUTHOR.Contacting the Author
:i1 id=Author.Contacting the Author
:p.This program was written as practice for OWL in Borland C++ for OS2 2.00,
and as such, I won't be doing much with it. Still, I'll gladly accept
feedback on any bugs, problems, etc... 
:p.The program is provided AS IS, with no warranties, guarantees, blah, blah, blah...
It is freeware, so enjoy it.
:p. For feedback, questions, etc... I can be reached via several methods:
:p.:lines. 
Herbert Bushong
email - 
  harchon@centuryinter.net
  hbush@ibm.net

fidonet -
  herbert bushong@1&colon.19/19
  OS2PROG, OS2 echoes

or in the Intelec Network in various conferences.
:elines.

:euserdoc.
