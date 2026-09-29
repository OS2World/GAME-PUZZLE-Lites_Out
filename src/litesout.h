/*--------------------------------------------------------------------
   LITESOUT.H -- LitesOut for OS/2, 32-bit PM port
  --------------------------------------------------------------------*/
#ifndef LITESOUT_H
#define LITESOUT_H

/* Grid dimensions */
#define BLOCKSACROSS  6
#define BLOCKSDOWN    8
#define BLOCKWIDTH    80
#define BLOCKHEIGHT   80
#define GRIDXOFFSET   0
#define GRIDYOFFSET   0

/* Block states */
#define GRIDSTATEOFF  0
#define GRIDSTATEON1  1
#define GRIDSTATEON2  2

/* Toggle directions */
#define DIRECTIONDOWN (-1)
#define DIRECTIONUP   1

/* Difficulty modes */
#define MODEEASY      1
#define MODEHARD      2

/* Client area (content fits at 420 wide, 380 tall) */
#define CLIENT_W      840
#define CLIENT_H      760

/* Single resource ID (menu, accel, icon) */
#define ID_RESOURCE   1

/* Game menu IDs (100-199) */
#define IDM_NEW       100
#define IDM_EXIT      104

/* Options menu IDs (200-299) */
#define IDM_HARDMODE   200
#define IDM_TOPTEN     201
#define IDM_FRAME      203
#define IDM_SAVEONEXIT 204

/* Language submenu IDs (300-399) */
#define IDM_LANG_EN   300
#define IDM_LANG_ES   301
#define IDM_LANG_NL   302
#define IDM_LANG_DE   303
#define IDM_LANG_FR   304
#define IDM_LANG_IT   305

/* Help menu IDs (900-999) */
#define IDM_ABOUT     999

/* Submenu cascade IDs (1000-1099) */
#define IDM_SUBMENU_GAME    1000
#define IDM_SUBMENU_OPTIONS 1001
#define IDM_SUBMENU_LANG    1002
#define IDM_SUBMENU_HELP    1003

/* Dialog IDs */
#define IDD_ABOUT       1
#define IDD_TOPTEN      2
#define IDD_ENTERNAME   3

/* Dialog control IDs */
#define IDC_LB_TOPTEN   50
#define IDC_ENTERNAME   51
#define IDC_PROMPT_TXT  52

/* Bitmap resource IDs */
#define BMP_OFF       300
#define BMP_ON1       301
#define BMP_ON2       302
#define BMP_FLASHOFF  303
#define BMP_FLASHON1  304
#define BMP_FLASHON2  305
#define BMP_HITS      309
#define BMP_MODE      310

/* Pointer resource IDs */
#define CUR_MALET     306
#define CUR_MALETDWN  307
#define CUR_SCR1      315
#define CUR_SCR2      316
#define CUR_SCR3      317
#define CUR_SCR4      318

/* User messages */
#define WM_SCRAMBLE   (WM_USER + 1)

#endif
