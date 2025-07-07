/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  GAME_PANEL                       1
#define  GAME_PANEL_COMMANDBUTTON_1       2       /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_2       3       /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_3       4       /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_4       5       /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_5       6       /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_6       7       /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_7       8       /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_8       9       /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_9       10      /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_10      11      /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_11      12      /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_12      13      /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_13      14      /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_14      15      /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_15      16      /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_COMMANDBUTTON_16      17      /* control type: command, callback function: Board_Callback */
#define  GAME_PANEL_GAME_TXT_MOVES        18      /* control type: textMsg, callback function: (none) */
#define  GAME_PANEL_GAME_TXT_TIME         19      /* control type: textMsg, callback function: (none) */
#define  GAME_PANEL_GAME_BTN_BCK          20      /* control type: command, callback function: BackToMenu_Callback */
#define  GAME_PANEL_GAME_TIMER            21      /* control type: timer, callback function: FlipBack_Timer_Callback */

#define  MAIN_PANEL                       2
#define  MAIN_PANEL_MAIN_BTN_NEWGAME      2       /* control type: command, callback function: MainPanel_NewGame_Callback */
#define  MAIN_PANEL_MAIN_BTN_EXIT         3       /* control type: command, callback function: MainPanel_Exit_Callback */
#define  MAIN_PANEL_MAIN_BTN_SCORES       4       /* control type: command, callback function: MainPanel_Scores_Callback */

#define  SCORE_PNL                        3
#define  SCORE_PNL_CLOSE                  2       /* control type: command, callback function: BackToMenu_Callback */
#define  SCORE_PNL_TABLE                  3       /* control type: table, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BackToMenu_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Board_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FlipBack_Timer_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanel_Exit_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanel_NewGame_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanel_Scores_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
