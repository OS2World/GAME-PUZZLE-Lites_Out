/*--------------------------------------------------------------------
   LITESOUT.C -- LitesOut for OS/2, 32-bit PM port (OpenWatcom)
   Original: (c) 1996, 1997 Herbert Bushong  All Rights Reserved
   32-bit PM port: (c) 2026 OS2World
  --------------------------------------------------------------------*/

const char bldlevel[] =
    "@#Herbert Bushong:1.1#@##1## 29 Sep 2026 00:00:00      "
    "ARCAOS:::0::::@@LitesOut puzzle game for OS/2 (32-bit PM)\r\n\x1a";

#define INCL_BITMAPFILEFORMAT

#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS

#define INCL_DEV

#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#define INCL_WININPUT
#define INCL_WINRECTANGLES
#define INCL_WINPOINTERS
#define INCL_WINMENUS
#define INCL_WINFRAMEMGR
#define INCL_WINSWITCHLIST
#define INCL_WINSYS
#define INCL_WINDIALOGS
#define INCL_WINLISTBOXES
#define INCL_WINENTRYFIELDS

#define INCL_GPIBITMAPS
#define INCL_GPICONTROL
#define INCL_GPITRANSFORMS
#define INCL_GPIPRIMITIVES

#define INCL_ERRORS

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "litesout.h"
#include "lang.h"

/*--- language table ---*/
int current_lang = LANG_EN;

const char *lang_strings[LANG_COUNT][STR_COUNT] = {
    /* EN */
    { "~Game","~New Game\tCtrl+N","E~xit\tCtrl+X",
      "~Options","~Hard Mode","~Top Ten Scores\tCtrl+T",
      "~Language","~Frame Controls\tCtrl+F","~Save settings on exit",
      "~Help","~About...",
      "Congratulations!","You Won!","Game Over",
      "Switch to Hard mode?","Switch to Easy mode?",
      "Mode","Easy","Hard",
      "Enter your name:","Top Ten Scores - Easy","Top Ten Scores - Hard",
      "Close","OK" },
    /* ES */
    { "~Juego","~Nuevo Juego\tCtrl+N","~Salir\tCtrl+X",
      "~Opciones","Modo ~Dificil","~Top 10 Puntuaciones\tCtrl+T",
      "~Idioma","~Controles Marco\tCtrl+F","~Guardar ajustes al salir",
      "~Ayuda","~Acerca de...",
      "Felicitaciones!","Ganaste!","Juego Terminado",
      "Cambiar a modo Dificil?","Cambiar a modo Facil?",
      "Modo","Facil","Dificil",
      "Ingresa tu nombre:","Top 10 - Facil","Top 10 - Dificil",
      "Cerrar","Aceptar" },
    /* NL */
    { "~Spel","~Nieuw Spel\tCtrl+N","~Afsluiten\tCtrl+X",
      "~Opties","~Moeilijke Modus","~Top Tien\tCtrl+T",
      "~Taal","~Kaderbeheer\tCtrl+F","~Instellingen opslaan",
      "~Help","~Info...",
      "Gefeliciteerd!","Je hebt gewonnen!","Spel Afgelopen",
      "Wisselen naar Moeilijk?","Wisselen naar Gemakkelijk?",
      "Modus","Gemakkelijk","Moeilijk",
      "Voer je naam in:","Top Tien - Gemakkelijk","Top Tien - Moeilijk",
      "Sluiten","OK" },
    /* DE */
    { "~Spiel","~Neues Spiel\tCtrl+N","~Beenden\tCtrl+X",
      "~Optionen","Schwerer ~Modus","~Bestenliste\tCtrl+T",
      "~Sprache","~Rahmen\tCtrl+F","~Einstellungen speichern",
      "~Hilfe","~Info...",
      "Glueckwunsch!","Du hast gewonnen!","Spiel Vorbei",
      "Zum schweren Modus wechseln?","Zum leichten Modus wechseln?",
      "Modus","Leicht","Schwer",
      "Gib deinen Namen ein:","Bestenliste - Leicht","Bestenliste - Schwer",
      "Schliessen","OK" },
    /* FR */
    { "~Jeu","~Nouveau Jeu\tCtrl+N","~Quitter\tCtrl+X",
      "~Options","Mode ~Difficile","~Meilleurs Scores\tCtrl+T",
      "~Langue","~Cadre\tCtrl+F","~Sauver config a la sortie",
      "~Aide","~A propos...",
      "Felicitations!","Vous avez gagne!","Partie Terminee",
      "Passer en mode Difficile?","Passer en mode Facile?",
      "Mode","Facile","Difficile",
      "Entrez votre nom:","Meilleurs Scores - Facile","Meilleurs Scores - Difficile",
      "Fermer","OK" },
    /* IT */
    { "~Gioco","~Nuovo Gioco\tCtrl+N","~Esci\tCtrl+X",
      "~Opzioni","Modalita ~Difficile","~Punteggi\tCtrl+T",
      "~Lingua","~Cornice\tCtrl+F","~Salva impostazioni all uscita",
      "~Aiuto","~Informazioni...",
      "Complimenti!","Hai vinto!","Gioco Finito",
      "Passare alla modalita Difficile?","Passare alla modalita Facile?",
      "Modalita","Facile","Difficile",
      "Inserisci il tuo nome:","Punteggi - Facile","Punteggi - Difficile",
      "Chiudi","OK" },
};

/*--- bitmap info ---*/
typedef struct { HBITMAP hbm; LONG w, h; } BMPINFO;

static BMPINFO bmpOff, bmpFlashOff;
static BMPINFO bmpOn1, bmpFlashOn1;
static BMPINFO bmpOn2, bmpFlashOn2;
static BMPINFO bmpHits, bmpMode;
static HPOINTER curMalet, curMaletDwn;
static HPOINTER curScr1, curScr2, curScr3, curScr4;

/*--- window handles ---*/
static HAB  hab;
static HMQ  hmq;
static HWND hwndFrame, hwndClient;
static HWND hwndTitleBar, hwndSysMenu, hwndMinButton, hwndMenuBar;
static HWND hwndObject;
static BOOL bFrameHidden = FALSE;

/*--- client size (updated in WM_SIZE) ---*/
static LONG clientW = CLIENT_W;
static LONG clientH = CLIENT_H;

/*--- game state ---*/
static int  blk_state[BLOCKSACROSS][BLOCKSDOWN];
static int  blk_dir  [BLOCKSACROSS][BLOCKSDOWN];
static int  maxstate     = MODEEASY;
static int  counter      = 0;
static int  blocks_lit   = 0;
static int  flash_x      = -1;
static int  flash_y      = -1;
static BOOL game_over    = FALSE;
static BOOL need_scramble = TRUE;

/*--- hi-scores ---*/
typedef struct { char name[60]; long score; } HISENTRY;
static HISENTRY hiscores[20];

/*--- settings ---*/
static int save_on_exit = 1;

#define CONFIG_FILE "litesout.cfg"

/*--- forward declarations ---*/
static void DrawBlock(HPS hps, int gx, int gy, BOOL flash);
static void DrawScoreboard(HPS hps);
static void WriteScore(HPS hps);
static void StartNewGame(HWND hwnd);
static void CheckGameOver(HWND hwnd, HPS hps);
static void set_language(int lang);
MRESULT EXPENTRY ClientWndProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AboutDlgProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY TopTenDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY EnterNameDlgProc(HWND, ULONG, MPARAM, MPARAM);

/*======================================================================
   CONFIG
  ======================================================================*/
static void load_settings(void)
{
    FILE *fp;
    int i;
    save_on_exit = 1;
    maxstate     = MODEEASY;
    current_lang = LANG_EN;
    for (i = 0; i < 20; i++) {
        hiscores[i].name[0] = '\0';
        hiscores[i].score   = 9999L;
    }
    fp = fopen(CONFIG_FILE, "rb");
    if (!fp) return;
    fread(&save_on_exit, sizeof(int), 1, fp);
    fread(&maxstate,     sizeof(int), 1, fp);
    fread(&current_lang, sizeof(int), 1, fp);
    fread(hiscores, sizeof(hiscores), 1, fp);
    fclose(fp);
    if (save_on_exit < 0 || save_on_exit > 1)         save_on_exit = 1;
    if (maxstate != MODEEASY && maxstate != MODEHARD) maxstate = MODEEASY;
    if (current_lang < 0 || current_lang >= LANG_COUNT) current_lang = LANG_EN;
}

static void save_settings(void)
{
    FILE *fp = fopen(CONFIG_FILE, "wb");
    if (!fp) return;
    fwrite(&save_on_exit, sizeof(int), 1, fp);
    fwrite(&maxstate,     sizeof(int), 1, fp);
    fwrite(&current_lang, sizeof(int), 1, fp);
    fwrite(hiscores, sizeof(hiscores), 1, fp);
    fclose(fp);
}

/*======================================================================
   HI-SCORE
  ======================================================================*/
static BOOL IsHigh(long value)
{
    int last = (maxstate == MODEEASY) ? 9 : 19;
    return (BOOL)(value < hiscores[last].score);
}

static void AddScore(const char *name, long value)
{
    int start = (maxstate == MODEEASY) ? 0 : 10;
    int end   = start + 10;
    int i;
    BOOL added = FALSE;
    for (i = end - 1; i > start; i--) {
        if (hiscores[i-1].score >= value) {
            strcpy(hiscores[i].name, hiscores[i-1].name);
            hiscores[i].score = hiscores[i-1].score;
        } else {
            strncpy(hiscores[i].name, name, 59);
            hiscores[i].name[59] = '\0';
            hiscores[i].score = value;
            added = TRUE;
            break;
        }
    }
    if (!added) {
        strncpy(hiscores[start].name, name, 59);
        hiscores[start].name[59] = '\0';
        hiscores[start].score = value;
    }
}

/*======================================================================
   GAME LOGIC
  ======================================================================*/
static int ToggleState(int bx, int by, int value)
{
    int *s = &blk_state[bx][by];
    int *d = &blk_dir  [bx][by];
    if (*s < value && *d == DIRECTIONDOWN) *d = DIRECTIONUP;
    if (*s > value && *d == DIRECTIONUP)   *d = DIRECTIONDOWN;
    if (*s == maxstate) *d = DIRECTIONDOWN;
    if (*s == 0)        *d = DIRECTIONUP;
    *s += *d;
    return *s;
}

static int CountLit(void)
{
    int count = 0, i, j;
    for (i = 0; i < BLOCKSACROSS; i++)
        for (j = 0; j < BLOCKSDOWN; j++)
            if (blk_state[i][j] > 0) count++;
    return count;
}

static int ToggleBlock(int x, int y)
{
    int temp;
    temp = ToggleState(x, y, blk_state[x][y]);
    if (x > 0)              ToggleState(x-1, y,   temp);
    if (x+1 < BLOCKSACROSS) ToggleState(x+1, y,   temp);
    if (y > 0)              ToggleState(x,   y-1, temp);
    if (y+1 < BLOCKSDOWN)   ToggleState(x,   y+1, temp);
    return CountLit();
}

static void ResetGrid(void)
{
    int i, j;
    for (i = 0; i < BLOCKSACROSS; i++)
        for (j = 0; j < BLOCKSDOWN; j++) {
            blk_state[i][j] = 0;
            blk_dir  [i][j] = DIRECTIONUP;
        }
}

/*======================================================================
   BITMAP HELPERS
  ======================================================================*/
static HBITMAP LoadBmp(ULONG id)
{
    HPS hps = WinGetScreenPS(HWND_DESKTOP);
    HBITMAP hbm = GpiLoadBitmap(hps, NULLHANDLE, id, 0L, 0L);
    WinReleasePS(hps);
    return hbm;
}

static void GetBmpSize(HBITMAP hbm, LONG *pw, LONG *ph)
{
    BITMAPINFOHEADER bih;
    bih.cbFix = sizeof(bih);
    GpiQueryBitmapParameters(hbm, &bih);
    *pw = (LONG)bih.cx;
    *ph = (LONG)bih.cy;
}

static void BlitBmp(HPS hps, HBITMAP hbm,
                    LONG srcW, LONG srcH, LONG dstW, LONG dstH,
                    LONG dstX, LONG dstY)
{
    HDC    hdc;
    HPS    hmps;
    SIZEL  sz = {0, 0};
    POINTL apt[4];

    hdc  = DevOpenDC(hab, OD_MEMORY, "*", 0L, NULL, NULLHANDLE);
    hmps = GpiCreatePS(hab, hdc, &sz,
                       PU_PELS|GPIF_DEFAULT|GPIT_MICRO|GPIA_ASSOC);
    GpiSetBitmap(hmps, hbm);

    /* dstX/dstY are Windows-style (y=0 at top); flip for PM */
    apt[0].x = dstX;        apt[0].y = clientH - dstY - dstH;
    apt[1].x = dstX + dstW; apt[1].y = clientH - dstY;
    apt[2].x = 0;            apt[2].y = 0;
    apt[3].x = srcW;         apt[3].y = srcH;

    GpiBitBlt(hps, hmps, 4L, apt, ROP_SRCCOPY, BBO_AND);

    GpiSetBitmap(hmps, NULLHANDLE);
    GpiDestroyPS(hmps);
    DevCloseDC(hdc);
}

/*======================================================================
   DRAWING
  ======================================================================*/
static void DrawBlock(HPS hps, int gx, int gy, BOOL flash)
{
    HBITMAP hbm;
    int st = blk_state[gx][gy];
    if (st <= 0)
        hbm = flash ? bmpFlashOff.hbm : bmpOff.hbm;
    else if (st == 1)
        hbm = flash ? bmpFlashOn1.hbm : bmpOn1.hbm;
    else
        hbm = flash ? bmpFlashOn2.hbm : bmpOn2.hbm;

    BlitBmp(hps, hbm, 40L, 40L, (LONG)BLOCKWIDTH, (LONG)BLOCKHEIGHT,
            (LONG)(gx * BLOCKWIDTH  + GRIDXOFFSET),
            (LONG)(gy * BLOCKHEIGHT + GRIDYOFFSET));
}

static void DrawScoreboard(HPS hps)
{
    BlitBmp(hps, bmpHits.hbm, bmpHits.w, bmpHits.h, bmpHits.w, bmpHits.h, 500L, 118L);
    BlitBmp(hps, bmpMode.hbm, bmpMode.w, bmpMode.h, bmpMode.w, bmpMode.h, 500L, 318L);
}

static void WriteScore(HPS hps)
{
    char s[30];
    RECTL rcl;
    POINTL ptl;
    const char *modeStr;

    GpiSetColor(hps, CLR_BLACK);
    GpiSetBackMix(hps, BM_LEAVEALONE);

    /* Counter */
    rcl.xLeft = 600; rcl.xRight = 790;
    rcl.yBottom = clientH - 248; rcl.yTop = clientH - 196;
    WinFillRect(hps, &rcl, CLR_PALEGRAY);
    sprintf(s, "%6.6d", counter);
    ptl.x = 604; ptl.y = clientH - 232;
    GpiCharStringAt(hps, &ptl, (LONG)strlen(s), s);

    /* Mode */
    rcl.xLeft = 620; rcl.xRight = 790;
    rcl.yBottom = clientH - 448; rcl.yTop = clientH - 396;
    WinFillRect(hps, &rcl, CLR_PALEGRAY);
    modeStr = (maxstate == MODEHARD) ? tr(STR_HARD) : tr(STR_EASY);
    ptl.x = 624; ptl.y = clientH - 432;
    GpiCharStringAt(hps, &ptl, (LONG)strlen(modeStr), (char*)modeStr);

    if (game_over) {
        rcl.xLeft = 480; rcl.xRight = 840;
        rcl.yBottom = clientH - 736; rcl.yTop = clientH - 584;
        WinFillRect(hps, &rcl, CLR_PALEGRAY);
        ptl.x = 520; ptl.y = clientH - 620;
        GpiCharStringAt(hps, &ptl, (LONG)strlen(tr(STR_CONGRATS)), tr(STR_CONGRATS));
        ptl.y = clientH - 656;
        GpiCharStringAt(hps, &ptl, (LONG)strlen(tr(STR_YOU_WON)), tr(STR_YOU_WON));
        ptl.y = clientH - 716;
        GpiCharStringAt(hps, &ptl, (LONG)strlen(tr(STR_GAME_OVER_TXT)), tr(STR_GAME_OVER_TXT));
    }
}

/*======================================================================
   SCRAMBLE
  ======================================================================*/
static void ScrambleBlocks(HWND hwnd)
{
    HPS  hps;
    int  i, xVal, yVal;

    hps = WinGetPS(hwnd);
    srand((unsigned)time(NULL));
    for (i = 0; i < 100; i++) {
        switch (rand() % 4) {
            case 0:  WinSetPointer(HWND_DESKTOP, curScr1); break;
            case 1:  WinSetPointer(HWND_DESKTOP, curScr2); break;
            case 2:  WinSetPointer(HWND_DESKTOP, curScr3); break;
            default: WinSetPointer(HWND_DESKTOP, curScr4); break;
        }
        xVal = rand() % BLOCKSACROSS;
        yVal = rand() % BLOCKSDOWN;
        blocks_lit = ToggleBlock(xVal, yVal);
        DrawBlock(hps, xVal, yVal, TRUE);
        if (xVal > 0)              DrawBlock(hps, xVal-1, yVal,   FALSE);
        if (xVal+1 < BLOCKSACROSS) DrawBlock(hps, xVal+1, yVal,   FALSE);
        if (yVal > 0)              DrawBlock(hps, xVal,   yVal-1, FALSE);
        if (yVal+1 < BLOCKSDOWN)   DrawBlock(hps, xVal,   yVal+1, FALSE);
        DrawBlock(hps, xVal, yVal, FALSE);
    }
    WinSetPointer(HWND_DESKTOP, curMalet);
    WinReleasePS(hps);
}

/*======================================================================
   GAME FLOW
  ======================================================================*/
static void StartNewGame(HWND hwnd)
{
    ResetGrid();
    counter      = 0;
    flash_x      = -1;
    flash_y      = -1;
    game_over    = FALSE;
    need_scramble = TRUE;
    WinInvalidateRect(hwnd, NULL, TRUE);
}

static char enter_name[60];

static void CheckGameOver(HWND hwnd, HPS hps)
{
    if (blocks_lit != 0) return;

    if (flash_x != -1) {
        DrawBlock(hps, flash_x, flash_y, FALSE);
        flash_x = flash_y = -1;
    }
    game_over = TRUE;

    if (IsHigh(counter)) {
        enter_name[0] = '\0';
        WinDlgBox(HWND_DESKTOP, hwnd, EnterNameDlgProc,
                  NULLHANDLE, IDD_ENTERNAME, NULL);
        if (enter_name[0] == '\0') strcpy(enter_name, "Anon");
        AddScore(enter_name, (long)counter);
        WinDlgBox(HWND_DESKTOP, hwnd, TopTenDlgProc,
                  NULLHANDLE, IDD_TOPTEN, NULL);
    }
    WriteScore(hps);
}

/*======================================================================
   LANGUAGE
  ======================================================================*/
static void set_language(int lang)
{
    HWND hwndMenu, hwndGameSub, hwndOptSub, hwndLangSub, hwndHelpSub;
    MENUITEM mi;
    int i;

    if (lang < 0 || lang >= LANG_COUNT) return;
    current_lang = lang;

    hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
    if (!hwndMenu) return;

    WinSendMsg(hwndMenu, MM_QUERYITEM,
               MPFROM2SHORT(IDM_SUBMENU_GAME, FALSE), MPFROMP(&mi));
    hwndGameSub = mi.hwndSubMenu;
    WinSendMsg(hwndMenu, MM_QUERYITEM,
               MPFROM2SHORT(IDM_SUBMENU_OPTIONS, FALSE), MPFROMP(&mi));
    hwndOptSub = mi.hwndSubMenu;
    WinSendMsg(hwndOptSub, MM_QUERYITEM,
               MPFROM2SHORT(IDM_SUBMENU_LANG, FALSE), MPFROMP(&mi));
    hwndLangSub = mi.hwndSubMenu;
    WinSendMsg(hwndMenu, MM_QUERYITEM,
               MPFROM2SHORT(IDM_SUBMENU_HELP, FALSE), MPFROMP(&mi));
    hwndHelpSub = mi.hwndSubMenu;

    /* Top-level */
    WinSendMsg(hwndMenu, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SUBMENU_GAME),    MPFROMP(tr(STR_MENU_GAME)));
    WinSendMsg(hwndMenu, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SUBMENU_OPTIONS), MPFROMP(tr(STR_MENU_OPTIONS)));
    WinSendMsg(hwndMenu, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SUBMENU_HELP),    MPFROMP(tr(STR_MENU_HELP)));

    /* Game submenu */
    WinSendMsg(hwndGameSub, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_NEW),  MPFROMP(tr(STR_MENU_NEW)));
    WinSendMsg(hwndGameSub, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_EXIT), MPFROMP(tr(STR_MENU_EXIT)));

    /* Options submenu */
    WinSendMsg(hwndOptSub, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_HARDMODE),      MPFROMP(tr(STR_MENU_HARDMODE)));
    WinSendMsg(hwndOptSub, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_TOPTEN),        MPFROMP(tr(STR_MENU_TOPTEN)));
    WinSendMsg(hwndOptSub, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SUBMENU_LANG),  MPFROMP(tr(STR_MENU_LANGUAGE)));
    WinSendMsg(hwndOptSub, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_FRAME),         MPFROMP(tr(STR_MENU_FRAME)));
    WinSendMsg(hwndOptSub, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SAVEONEXIT),    MPFROMP(tr(STR_MENU_SAVEONEXIT)));

    /* Help submenu */
    WinSendMsg(hwndHelpSub, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_ABOUT), MPFROMP(tr(STR_MENU_ABOUT)));

    /* Language checkmarks */
    for (i = 0; i < LANG_COUNT; i++) {
        WinSendMsg(hwndLangSub, MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_LANG_EN + i, TRUE),
                   MPFROM2SHORT(MIA_CHECKED, (i == lang) ? MIA_CHECKED : 0));
    }
}

/*======================================================================
   FRAME CONTROLS TOGGLE
  ======================================================================*/
static void ToggleFrame(void)
{
    SWP swp;
    if (!bFrameHidden) {
        WinSendMsg(hwndMenuBar, MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_FRAME, TRUE),
                   MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));
        WinSetParent(hwndTitleBar,  hwndObject, FALSE);
        WinSetParent(hwndSysMenu,   hwndObject, FALSE);
        WinSetParent(hwndMenuBar,   hwndObject, FALSE);
        WinSetParent(hwndMinButton, hwndObject, FALSE);
        bFrameHidden = TRUE;
        WinSendMsg(hwndFrame, WM_UPDATEFRAME,
                   (MPARAM)(FCF_TITLEBAR|FCF_SYSMENU|FCF_MINMAX|FCF_MENU), NULL);
    } else {
        WinSetParent(hwndTitleBar,  hwndFrame, FALSE);
        WinSetParent(hwndSysMenu,   hwndFrame, FALSE);
        WinSetParent(hwndMenuBar,   hwndFrame, FALSE);
        WinSetParent(hwndMinButton, hwndFrame, FALSE);
        bFrameHidden = FALSE;
        WinQueryWindowPos(hwndFrame, &swp);
        WinSetWindowPos(hwndFrame, HWND_TOP,
                        swp.x, swp.y, swp.cx, swp.cy,
                        SWP_MOVE|SWP_SIZE);
        WinInvalidateRect(hwndFrame, NULL, TRUE);
        WinSendMsg(hwndMenuBar, MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_FRAME, TRUE),
                   MPFROM2SHORT(MIA_CHECKED, 0));
    }
}

/*======================================================================
   DIALOG PROCEDURES
  ======================================================================*/
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch (msg) {
        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd) {
                case DID_OK:
                case DID_CANCEL:
                    WinDismissDlg(hwnd, TRUE);
                    return 0;
            }
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY TopTenDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch (msg) {
        case WM_INITDLG:
        {
            int start = (maxstate == MODEEASY) ? 0 : 10;
            HWND hwndLB = WinWindowFromID(hwnd, IDC_LB_TOPTEN);
            char buf[90];
            int i;
            const char *title = (maxstate == MODEEASY)
                                ? tr(STR_TOPTEN_EASY)
                                : tr(STR_TOPTEN_HARD);
            WinSetWindowText(hwnd, title);
            WinSendMsg(hwndLB, LM_DELETEALL, 0, 0);
            for (i = 0; i < 10; i++) {
                const char *nm = (hiscores[start+i].name[0] == '\0')
                                 ? "No One"
                                 : hiscores[start+i].name;
                sprintf(buf, "%-35.35s %6ld", nm, hiscores[start+i].score);
                WinSendMsg(hwndLB, LM_INSERTITEM,
                           MPFROMSHORT(LIT_END), MPFROMP(buf));
            }
            WinSetDlgItemText(hwnd, DID_OK, tr(STR_CLOSE));
            return (MRESULT)FALSE;
        }
        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd) {
                case DID_OK:
                case DID_CANCEL:
                    WinDismissDlg(hwnd, TRUE);
                    return 0;
            }
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY EnterNameDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch (msg) {
        case WM_INITDLG:
        {
            HWND hwndEF = WinWindowFromID(hwnd, IDC_ENTERNAME);
            WinSendMsg(hwndEF, EM_SETTEXTLIMIT, MPFROMSHORT(59), 0);
            WinSetWindowText(hwndEF, "");
            WinSetDlgItemText(hwnd, IDC_PROMPT_TXT, tr(STR_ENTER_NAME));
            WinSetDlgItemText(hwnd, DID_OK, tr(STR_OK));
            return (MRESULT)FALSE;
        }
        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd) {
                case DID_OK:
                    WinQueryDlgItemText(hwnd, IDC_ENTERNAME,
                                        (LONG)sizeof(enter_name), enter_name);
                    if (enter_name[0] == '\0') strcpy(enter_name, "Anon");
                    WinDismissDlg(hwnd, DID_OK);
                    return 0;
                case DID_CANCEL:
                    strcpy(enter_name, "Anon");
                    WinDismissDlg(hwnd, DID_CANCEL);
                    return 0;
            }
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/*======================================================================
   CLIENT WINDOW PROCEDURE
  ======================================================================*/
MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch (msg) {

    case WM_CREATE:
    {
        HPS hps = WinGetScreenPS(HWND_DESKTOP);
        bmpOff.hbm      = LoadBmp(BMP_OFF);
        bmpOn1.hbm      = LoadBmp(BMP_ON1);
        bmpOn2.hbm      = LoadBmp(BMP_ON2);
        bmpFlashOff.hbm = LoadBmp(BMP_FLASHOFF);
        bmpFlashOn1.hbm = LoadBmp(BMP_FLASHON1);
        bmpFlashOn2.hbm = LoadBmp(BMP_FLASHON2);
        bmpHits.hbm     = LoadBmp(BMP_HITS);
        bmpMode.hbm     = LoadBmp(BMP_MODE);
        WinReleasePS(hps);

        GetBmpSize(bmpOff.hbm,  &bmpOff.w,  &bmpOff.h);
        GetBmpSize(bmpHits.hbm, &bmpHits.w, &bmpHits.h);
        GetBmpSize(bmpMode.hbm, &bmpMode.w, &bmpMode.h);
        bmpFlashOff.w = bmpOff.w; bmpFlashOff.h = bmpOff.h;
        bmpOn1.w = bmpOn2.w = bmpFlashOn1.w = bmpFlashOn2.w = bmpOff.w;
        bmpOn1.h = bmpOn2.h = bmpFlashOn1.h = bmpFlashOn2.h = bmpOff.h;

        curMalet    = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, CUR_MALET);
        curMaletDwn = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, CUR_MALETDWN);
        curScr1     = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, CUR_SCR1);
        curScr2     = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, CUR_SCR2);
        curScr3     = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, CUR_SCR3);
        curScr4     = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, CUR_SCR4);

        WinSetPointer(HWND_DESKTOP, curMalet);
        return 0;
    }

    case WM_SIZE:
        clientW = SHORT1FROMMP(mp2);
        clientH = SHORT2FROMMP(mp2);
        return 0;

    case WM_PAINT:
    {
        HPS   hps;
        RECTL rcl;
        int   i, j;

        hps = WinBeginPaint(hwnd, NULLHANDLE, &rcl);
        WinQueryWindowRect(hwnd, &rcl);
        WinFillRect(hps, &rcl, CLR_PALEGRAY);
        DrawScoreboard(hps);
        for (i = 0; i < BLOCKSACROSS; i++)
            for (j = 0; j < BLOCKSDOWN; j++)
                DrawBlock(hps, i, j, FALSE);
        WriteScore(hps);
        WinEndPaint(hps);

        if (need_scramble) {
            need_scramble = FALSE;
            WinPostMsg(hwnd, WM_SCRAMBLE, 0, 0);
        }
        return 0;
    }

    case WM_SCRAMBLE:
        ScrambleBlocks(hwnd);
        WinInvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_BUTTON1DOWN:
    {
        SHORT mx = SHORT1FROMMP(mp1);
        SHORT my = SHORT2FROMMP(mp1);
        int xVal, yVal;
        HPS hps;

        if (game_over) return 0;
        WinSetPointer(HWND_DESKTOP, curMaletDwn);

        /* PM y=0 at bottom; convert to Windows-style y=0 at top */
        xVal = ((int)mx - GRIDXOFFSET) / BLOCKWIDTH;
        yVal = ((int)(clientH - my) - GRIDYOFFSET) / BLOCKHEIGHT;

        if (xVal < 0 || xVal >= BLOCKSACROSS ||
            yVal < 0 || yVal >= BLOCKSDOWN) return 0;

        blocks_lit = ToggleBlock(xVal, yVal);

        hps = WinGetPS(hwnd);
        DrawBlock(hps, xVal, yVal, TRUE);
        if (xVal > 0)              DrawBlock(hps, xVal-1, yVal,   FALSE);
        if (xVal+1 < BLOCKSACROSS) DrawBlock(hps, xVal+1, yVal,   FALSE);
        if (yVal > 0)              DrawBlock(hps, xVal,   yVal-1, FALSE);
        if (yVal+1 < BLOCKSDOWN)   DrawBlock(hps, xVal,   yVal+1, FALSE);
        flash_x = xVal;
        flash_y = yVal;
        counter++;
        WriteScore(hps);
        CheckGameOver(hwnd, hps);
        WinReleasePS(hps);
        return 0;
    }

    case WM_BUTTON1UP:
    {
        WinSetPointer(HWND_DESKTOP, curMalet);
        if (flash_x != -1) {
            HPS hps = WinGetPS(hwnd);
            DrawBlock(hps, flash_x, flash_y, FALSE);
            WinReleasePS(hps);
            flash_x = flash_y = -1;
        }
        return 0;
    }

    case WM_COMMAND:
        switch (SHORT1FROMMP(mp1)) {

        case IDM_NEW:
            StartNewGame(hwnd);
            break;

        case IDM_HARDMODE:
        {
            const char *qmsg = (maxstate == MODEEASY)
                               ? tr(STR_ASK_HARD)
                               : tr(STR_ASK_EASY);
            ULONG rc = WinMessageBox(HWND_DESKTOP, hwnd,
                                     qmsg, tr(STR_MODE_LABEL),
                                     0, MB_YESNO|MB_QUERY);
            if (rc != MBID_YES) break;
            maxstate = (maxstate == MODEEASY) ? MODEHARD : MODEEASY;
            WinSendMsg(WinWindowFromID(hwndFrame, FID_MENU), MM_SETITEMATTR,
                       MPFROM2SHORT(IDM_HARDMODE, TRUE),
                       MPFROM2SHORT(MIA_CHECKED, (maxstate==MODEHARD)?MIA_CHECKED:0));
            StartNewGame(hwnd);
            break;
        }

        case IDM_TOPTEN:
            WinDlgBox(HWND_DESKTOP, hwnd, TopTenDlgProc,
                      NULLHANDLE, IDD_TOPTEN, NULL);
            break;

        case IDM_FRAME:
            ToggleFrame();
            break;

        case IDM_SAVEONEXIT:
            save_on_exit ^= 1;
            WinSendMsg(WinWindowFromID(hwndFrame, FID_MENU), MM_SETITEMATTR,
                       MPFROM2SHORT(IDM_SAVEONEXIT, TRUE),
                       MPFROM2SHORT(MIA_CHECKED, save_on_exit ? MIA_CHECKED : 0));
            break;

        case IDM_LANG_EN: case IDM_LANG_ES: case IDM_LANG_NL:
        case IDM_LANG_DE: case IDM_LANG_FR: case IDM_LANG_IT:
            set_language(SHORT1FROMMP(mp1) - IDM_LANG_EN);
            break;

        case IDM_ABOUT:
            WinDlgBox(HWND_DESKTOP, hwnd, AboutDlgProc,
                      NULLHANDLE, IDD_ABOUT, NULL);
            break;

        case IDM_EXIT:
            WinPostMsg(hwnd, WM_QUIT, 0, 0);
            break;
        }
        return 0;

    case WM_DESTROY:
    {
        GpiDeleteBitmap(bmpOff.hbm);
        GpiDeleteBitmap(bmpOn1.hbm);
        GpiDeleteBitmap(bmpOn2.hbm);
        GpiDeleteBitmap(bmpFlashOff.hbm);
        GpiDeleteBitmap(bmpFlashOn1.hbm);
        GpiDeleteBitmap(bmpFlashOn2.hbm);
        GpiDeleteBitmap(bmpHits.hbm);
        GpiDeleteBitmap(bmpMode.hbm);
        WinDestroyPointer(curMalet);
        WinDestroyPointer(curMaletDwn);
        WinDestroyPointer(curScr1);
        WinDestroyPointer(curScr2);
        WinDestroyPointer(curScr3);
        WinDestroyPointer(curScr4);
        return 0;
    }

    } /* switch */
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

/*======================================================================
   MAIN
  ======================================================================*/
int main(void)
{
    static const char szClass[] = "LitesOutClient";
    ULONG flFrame = FCF_TITLEBAR|FCF_SYSMENU|FCF_MINBUTTON|
                    FCF_TASKLIST|FCF_ICON|FCF_MENU|FCF_ACCELTABLE;
    RECTL rcl;
    LONG  cxScr, cyScr, winW, winH, x, y;

    hab = WinInitialize(0);
    hmq = WinCreateMsgQueue(hab, 0);

    load_settings();

    WinRegisterClass(hab, szClass, ClientWndProc,
                     CS_SIZEREDRAW|CS_SYNCPAINT, 0);

    hwndFrame = WinCreateStdWindow(
        HWND_DESKTOP, 0L,
        &flFrame, szClass, "LitesOut!",
        0L, NULLHANDLE, ID_RESOURCE, &hwndClient);

    if (!hwndFrame) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                      "Cannot create window.", "LitesOut!", 0, MB_OK|MB_ERROR);
        WinDestroyMsgQueue(hmq);
        WinTerminate(hab);
        return 1;
    }

    /* Capture frame child handles for Frame Controls */
    hwndTitleBar = WinWindowFromID(hwndFrame, FID_TITLEBAR);
    hwndSysMenu  = WinWindowFromID(hwndFrame, FID_SYSMENU);
    hwndMinButton= WinWindowFromID(hwndFrame, FID_MINMAX);
    hwndMenuBar  = WinWindowFromID(hwndFrame, FID_MENU);

    /* Parking window for Frame Controls hide */
    hwndObject = WinCreateWindow(HWND_OBJECT, WC_FRAME, "",
                                 0L, 0,0,0,0,
                                 NULLHANDLE, HWND_TOP, 0, NULL, NULL);

    /* Restore checkable menu items */
    WinSendMsg(hwndMenuBar, MM_SETITEMATTR,
               MPFROM2SHORT(IDM_SAVEONEXIT, TRUE),
               MPFROM2SHORT(MIA_CHECKED, save_on_exit ? MIA_CHECKED : 0));
    WinSendMsg(hwndMenuBar, MM_SETITEMATTR,
               MPFROM2SHORT(IDM_HARDMODE, TRUE),
               MPFROM2SHORT(MIA_CHECKED, (maxstate == MODEHARD) ? MIA_CHECKED : 0));

    /* Apply persisted language */
    set_language(current_lang);

    /* Size: compute frame to give CLIENT_W x CLIENT_H client area */
    rcl.xLeft = 0; rcl.yBottom = 0;
    rcl.xRight = CLIENT_W; rcl.yTop = CLIENT_H;
    WinCalcFrameRect(hwndFrame, &rcl, FALSE);
    winW = rcl.xRight - rcl.xLeft;
    winH = rcl.yTop   - rcl.yBottom;

    cxScr = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
    cyScr = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
    if (winW > cxScr) winW = cxScr;
    if (winH > cyScr) winH = cyScr;
    x = (cxScr - winW) / 2;
    y = (cyScr - winH) / 2;

    WinSetWindowPos(hwndFrame, HWND_TOP, x, y, winW, winH,
                    SWP_SIZE|SWP_MOVE|SWP_ACTIVATE|SWP_SHOW);

    /* Message loop */
    {
        QMSG qmsg;
        while (WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0))
            WinDispatchMsg(hab, &qmsg);
    }

    if (save_on_exit) save_settings();

    WinDestroyWindow(hwndObject);
    WinDestroyWindow(hwndFrame);
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
    return 0;
}
