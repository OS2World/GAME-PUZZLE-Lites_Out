//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1994 by Borland International
//----------------------------------------------------------------------------

#ifndef LITESOUT__H_
#define LITESOUT__H_
//Program Defines
#define BLOCKSACROSS 6
#define BLOCKSDOWN   8
#define BLOCKWIDTH   40
#define BLOCKHEIGHT  40
#define GRIDSTATEOFF 0
#define GRIDSTATEON1 1
#define GRIDSTATEON2 2
#define DIRECTIONDOWN -1
#define DIRECTIONUP   1
#define MODEEASY			1
#define MODEHARD      2
#define GRIDXOFFSET 	0
#define GRIDYOFFSET 	0

//RESOURCE ID's
//button return values
#define IDOK                1
#define IDCANCEL            2
#define IDABORT             3
#define IDRETRY             4
#define IDIGNORE            5
#define IDYES               6
#define IDNO                7

//STRINGTABLE DEFINES
#define IDS_RESET     50
#define IDS_LOAD      51
#define IDS_MODE		  52
#define IDS_HELPWINDOWTITLE 53
#define IDS_HELPLIBRARYNAME 54

//Menu choice defines
#define IDM_RESET	200
#define IDM_MODE  201
#define IDM_ABOUT	202
#define IDM_HELP  203
#define IDM_TOPTEN 204
//Menu and dialog identifiers
#define LitesMenu 200
#define About     210
#define TOPTENDLG 220

#define LB_TOPTEN 221
#define LB_TOPTENSCORES 222

//bitmap, cursor, and icon  resource ids
#define BMP_OFF			 300
#define BMP_ON1			 301
#define BMP_ON2			 302
#define BMP_FLASHOFF			 303
#define BMP_FLASHON1			 304
#define BMP_FLASHON2			 305
#define CUR_MALET    306
#define CUR_MALETDWN 307
#define ICO_LITESOUT 308

#define BMP_HITS  309
#define BMP_MODE  310
#define CUR_SCR1	315
#define CUR_SCR2  316
#define CUR_SCR3  317
#define CUR_SCR4  318


// Help Panel Define
#define LITESOUT_HELP_TABLE 1000
#define SUBTABLE_MAIN 1001
#define PANEL_MAIN 100
#define PANEL_MENU 110
#define PANEL_WINDOW 120

#endif
