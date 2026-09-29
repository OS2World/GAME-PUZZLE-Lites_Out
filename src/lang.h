/*--------------------------------------------------------------------
   LANG.H -- Language support for LitesOut
  --------------------------------------------------------------------*/
#ifndef LANG_H
#define LANG_H

#define LANG_EN   0
#define LANG_ES   1
#define LANG_NL   2
#define LANG_DE   3
#define LANG_FR   4
#define LANG_IT   5
#define LANG_COUNT 6

enum {
    STR_MENU_GAME = 0,
    STR_MENU_NEW,
    STR_MENU_EXIT,
    STR_MENU_OPTIONS,
    STR_MENU_HARDMODE,
    STR_MENU_TOPTEN,
    STR_MENU_LANGUAGE,
    STR_MENU_FRAME,
    STR_MENU_SAVEONEXIT,
    STR_MENU_HELP,
    STR_MENU_ABOUT,
    STR_CONGRATS,
    STR_YOU_WON,
    STR_GAME_OVER_TXT,
    STR_ASK_HARD,
    STR_ASK_EASY,
    STR_MODE_LABEL,
    STR_EASY,
    STR_HARD,
    STR_ENTER_NAME,
    STR_TOPTEN_EASY,
    STR_TOPTEN_HARD,
    STR_CLOSE,
    STR_OK,
    STR_COUNT
};

extern int current_lang;
extern const char *lang_strings[LANG_COUNT][STR_COUNT];
#define tr(id) ((char*)lang_strings[current_lang][(id)])

#endif
