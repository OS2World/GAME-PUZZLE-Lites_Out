//----------------------------------------------------------------------------
//   LitesOut for OS/2, Herbert Bushong, Copyright 1996, 1997  All Rights Reserved
//
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <owl\dialog.h>
#include <owl\inputdia.h>
#include <owl\menu.h>
#include <owl\dc.h>
#include <owl\checkbox.h>
#include <owl\listbox.h>
#include <owl\pmwindow.h>
#include <owl\window.rh>

#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "litesout.h"
#include "grid.h"

// --------------- TGameConfig ------------------------
// Stores configuration and High Scores
class TGameConfig
{
	protected:
		int mode;
		char configfile[256];
		char *HiNames[20];
		long HiScores[20];

	public:
		 TGameConfig(const char *file);
		 ~TGameConfig();
		 BOOL SaveConfig();
		 void AddScore(char *name, long value);
		 BOOL Mode(){return mode;};
		 void SetMode(int value){mode = value == MODEEASY ? value : MODEHARD;};
		 char *GetHiScoreName(int index){if (mode == MODEEASY)
																			return index < 10 ? HiNames[index] : NULL;
																		 else
																			return index < 20 ? HiNames[index + 10] : NULL;
																		 };
		 long GetHiScore(int index){if (mode == MODEEASY)
																			return index < 10 ? HiScores[index] : 9999;
																		 else
																			return index < 20 ? HiScores[index + 10] : 9999;
																		 };		 BOOL IsHigh(long value){return mode == MODEEASY ? (value < HiScores[9]) : (value < HiScores[19]);};
};

TGameConfig::TGameConfig(const char *file)
{
	FILE *cfile;

	strcpy(configfile, file);
	for (int i=0; i<20; i++)
		{
			HiNames[i] = new char[60];
			strcpy(HiNames[i],"");
			HiScores[i] = 9999L;
		};
	mode = MODEEASY;

	if	(NULL == (cfile = fopen(configfile,"rb")))
		{
			if (NULL == (cfile = fopen(configfile, "wb")))
				exit(10);
			else
				{
					fwrite(&mode, sizeof(int), 1, cfile);
					for (i=0; i<20; i++)
						{
							fwrite(HiNames[i], sizeof(char), 60, cfile);
							fwrite(&HiScores[i], sizeof(long), 1, cfile);
						}
					fflush(cfile);
				}
		 }
	else
		{
			fread(&mode, sizeof(int), 1, cfile);
			for (i=0; i<20; i++)
				{
					fread(HiNames[i], sizeof(char), 60, cfile);
					fread(&HiScores[i], sizeof(long), 1, cfile);
				}
		 }
	fclose(cfile);
}

TGameConfig::~TGameConfig()
{
	for (int i=0; i<20; i++)
			delete[] HiNames[i];
}

BOOL TGameConfig::SaveConfig()
{
	FILE *cfile;
	if (NULL == (cfile = fopen(configfile, "wb")))
				return FALSE;

	fwrite(&mode, sizeof(int), 1, cfile);
	for (int i=0; i<20; i++)
		{
			fwrite(HiNames[i], sizeof(char), 60, cfile);
			fwrite(&HiScores[i], sizeof(long), 1, cfile);
		}
	fflush(cfile);
	fclose(cfile);
	return TRUE;
}

void TGameConfig::AddScore(char *name, long value)
{
	BOOL added = FALSE;
	if (mode == MODEEASY)
	{
		for (int i=9; i > 0; i--)
		{
			if (HiScores[i-1] >= value)
				{
					strcpy(HiNames[i],HiNames[i-1]);
					HiScores[i] = HiScores[i-1];
				}
			else
				{
					strcpy(HiNames[i], name);
					HiScores[i] = value;
					added = TRUE;
					break;
				}
		};
		if (!added)
		{
			strcpy(HiNames[0], name);
			HiScores[0] = value;
		};
	}
	else
	{
		for (int i=19; i > 10; i--)
		{
			if (HiScores[i-1] >= value)
				{
					strcpy(HiNames[i],HiNames[i-1]);
					HiScores[i] = HiScores[i-1];
				}
			else
				{
					strcpy(HiNames[i], name);
					HiScores[i] = value;
					added = TRUE;
					break;
				}
		};
		if (!added)
		{
			strcpy(HiNames[10], name);
			HiScores[10] = value;
		};
	}
}

// --------------- TScoresDialog ------------------------
// Displays top ten scores

class TScoresDialog : public TDialog
{
	protected:
		TListBox *list;
		char scores[10][80];
	public:
		TScoresDialog(TWindow *parent, TResId ResId, TModule *mod, TGameConfig *cfg);
		~TScoresDialog(){ delete list;};
		void SetupWindow();
		void CmOk();

	DECLARE_RESPONSE_TABLE(TScoresDialog);
};

DEFINE_RESPONSE_TABLE1(TScoresDialog, TDialog)
EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE;

TScoresDialog::TScoresDialog(TWindow *parent, TResId ResId, TModule *mod, TGameConfig *cfg)
							:TDialog(parent, ResId, mod)
{
		for (int i=0; i < 10; i++)
			sprintf(scores[i], "%-35.35s %-15d", strlen(cfg->GetHiScoreName(i)) == 0 ? "No One" : cfg->GetHiScoreName(i), cfg->GetHiScore(i));
}

void TScoresDialog::SetupWindow()
{
	TDialog::SetupWindow();
	list = new TListBox(this, LB_TOPTEN);
	list->Create();
	for (int i=0; i < 10; i++)
		list->InsertString(scores[i], i);
}

void TScoresDialog::CmOk()
{
	TDialog::CmOk();
}

// --------------- TGameWindow ------------------------
// Main Window

class TGameWindow : public TWindow {
	public:
		TGameWindow();
		void DrawBlock(TDC& DC, int x, int y, BOOL flash);
		void DrawGameOver(TDC& DC);
		void WriteScore(TDC& dc);
		void DrawScoreboard(TDC& dc);
		void CheckGameOver();

	protected:
		void SetupWindow();
		void CleanupWindow();
		void ScrambleBlocks();
		//
		// message response functions
		//
		void CmAbout();      	// IDM_ABOUT
		void CmMode();    		// IDM_MODE
		void CmResetGame();  	// IDM_RESET
		void CmHelp();				// IDM_HELP
		void CmTopTen();      // IDM_TOPTEN
		BOOL EvEraseBkgnd(HDC hDC);
		void EvDestroy();
		void EvLButtonDown(UINT, TPoint&);
		void EvLButtonUp(UINT, TPoint&);
		void Paint(TDC&, BOOL, TRect&);

	private:
		TBitmap* 		BlockOff;
		TBitmap* 		BlockOffFlash;
		TBitmap* 		BlockOn1;
		TBitmap* 		BlockOn1Flash;
		TBitmap* 		BlockOn2;
		TBitmap* 		BlockOn2Flash;
		TBitmap*    Hits;
		TBitmap*    Mode;
		TCursor*    CursorDown;
		TCursor*    CursorUp;
		TCursor*    Cur1;
		TCursor*    Cur2;
		TCursor*    Cur3;
		TCursor*    Cur4;

		int         Counter, BlocksLit;
		int         useHardMode, flashX, flashY;
		BOOL        IsGameOver, NeedScrambled;
		GameGrid*		Grid;
		TGameConfig* config;

	DECLARE_RESPONSE_TABLE(TGameWindow);
};

DEFINE_RESPONSE_TABLE1(TGameWindow, TWindow)
	EV_COMMAND(IDM_MODE, CmMode),
	EV_COMMAND(IDM_RESET, CmResetGame),
	EV_COMMAND(IDM_ABOUT, CmAbout),
	EV_COMMAND(IDM_HELP, CmHelp),
	EV_COMMAND(IDM_TOPTEN, CmTopTen),
	EV_WM_ERASEBKGND,
	EV_WM_DESTROY,
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONUP,
	EV_WM_PAINT,
END_RESPONSE_TABLE;


// --------------- TGameWindow -----------------

TGameWindow::TGameWindow()
	: TWindow(0, 0, 0)
{
	Attr.X = 100;
	Attr.Y = 40;
	Attr.W = 400;
	Attr.H = 400;
	Attr.AccelTable = LitesMenu;
	randomize();

	IsGameOver = FALSE;
	Counter = 0;
	BlocksLit = 0;
	flashX = flashY = -1;
	SetCursor(GetModule(), CUR_MALET);
	Grid = new GameGrid;
	NeedScrambled = TRUE;
	config = new TGameConfig("litesout.ini");
	useHardMode = config->Mode();  //MODEEASY;
	Grid->SetMode(useHardMode);
		for (int i = 0; i < BLOCKSACROSS; i++)
			for (int j = 0; j < BLOCKSDOWN; j++)
				{
					Grid->Grid[i][j]->SetMaxState(useHardMode);
					Grid->Grid[i][j]->SetState(0);
				 };
}

void TGameWindow::ScrambleBlocks()
{
	 int xVal, yVal;
		TClientDC dc(*this);
		TModule* inst;
		inst = GetModule();

	 for (int i = 0; i < 100; i++)
		{
			switch (rand() % 4)
			{
				case 0:
					SetCursor(inst, CUR_SCR1);
					break;
				case 1:
					SetCursor(inst, CUR_SCR2);
					break;
				case 2:
					SetCursor(inst, CUR_SCR3);
					break;
				case 3:
					SetCursor(inst, CUR_SCR4);
					break;
				default:
					SetCursor(inst, CUR_SCR2);
			};

			xVal = rand() % BLOCKSACROSS;
			yVal = rand() % BLOCKSDOWN;
			Grid->ToggleBlock(xVal, yVal);

			DrawBlock(dc, xVal, yVal, TRUE);
			if (xVal > 0) DrawBlock(dc, xVal- 1, yVal, FALSE);
			if (xVal + 1 < Grid->Across) DrawBlock(dc, xVal + 1, yVal, FALSE);
			if (yVal > 0) DrawBlock(dc, xVal, yVal - 1, FALSE);
			if (yVal + 1 < Grid->Down) DrawBlock(dc, xVal, yVal + 1, FALSE);
			DrawBlock(dc, xVal, yVal, FALSE);
		};
	 SetCursor(inst, CUR_MALET);
	 inst = NULL;
}


void TGameWindow::SetupWindow()
{
	TWindow::SetupWindow();
	HINSTANCE inst = *GetModule();
	CursorDown = new TCursor(inst, CUR_MALETDWN);
	CursorUp = new TCursor(inst, CUR_MALET);
	Cur1 = new TCursor(inst, CUR_SCR1);
	Cur2 = new TCursor(inst, CUR_SCR2);
	Cur3 = new TCursor(inst, CUR_SCR3);
	Cur4 = new TCursor(inst, CUR_SCR4);
	BlockOff = new TBitmap(inst, BMP_OFF);
	BlockOffFlash = new TBitmap(inst, BMP_FLASHOFF);
	BlockOn1= new TBitmap(inst, BMP_ON1);
	BlockOn1Flash= new TBitmap(inst, BMP_FLASHON1);
	BlockOn2= new TBitmap(inst, BMP_ON2);
	BlockOn2Flash= new TBitmap(inst, BMP_FLASHON2);
	Hits = new TBitmap(inst, BMP_HITS);
	Mode = new TBitmap(inst, BMP_MODE);
	LoadAcceleratorTable();

}

void TGameWindow::CleanupWindow()
{
	config->SaveConfig();
	delete config;
	delete CursorDown;
	delete CursorUp;
	delete Cur1;
	delete Cur2;
	delete Cur3;
	delete Cur4;
	delete BlockOff;
	delete BlockOffFlash;
	delete BlockOn1;
	delete BlockOn1Flash;
	delete BlockOn2;
	delete BlockOn2Flash;
	delete Hits;
	delete Mode;
	delete Grid;
	TWindow::CleanupWindow();
}


static void DrawBMP(TDC& dc, POINT pt, TBitmap& bitmap)
{
	TMemoryDC memDC(dc);
	memDC.SelectObject(bitmap);

	dc.BitBlt(pt.x, pt.y, bitmap.Width(), bitmap.Height(), memDC, 0, 0, SRCCOPY);
}

void TGameWindow::DrawScoreboard(TDC& dc)
{
	TMemoryDC memDC(dc);
	memDC.SelectObject(*Hits);
	dc.BitBlt(250, 59, Hits->Width(), Hits->Height(), memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(*Mode);
	dc.BitBlt(250, 159, Mode->Width(), Mode->Height(), memDC, 0, 0, SRCCOPY);

}


void TGameWindow::DrawGameOver(TDC& dc)
{
	TBrush brush(TColor::LtGray);
	dc.SelectObject(brush);
	dc.SelectStockObject(NULL_PEN);
	dc.SetBkMode(TRANSPARENT);

		dc.Rectangle(300, 300, 399, 370);
		dc.TextOut(TPoint(260, 300), "Congratulations!", 17);
		dc.TextOut(TPoint(260, 320), "    You Won!    ", 17);
		dc.TextOut(TPoint(260, 350), "   Game Over    ", 17);

	dc.RestoreObjects();
}

void TGameWindow::DrawBlock(TDC& dc, int x, int y, BOOL flash)
{
	TMemoryDC memDC(dc);
	POINT pt;
	TBitmap* bmp;

	switch (Grid->Grid[x][y]->state)
	{
		case 0:
			if (!flash)
				memDC.SelectObject(*BlockOff);
			else
				memDC.SelectObject(*BlockOffFlash);
			break;

		case 1:
			if (!flash)
				memDC.SelectObject(*BlockOn1);
			else
				memDC.SelectObject(*BlockOn1Flash);
			break;

		case 2:
			if (!flash)
				memDC.SelectObject(*BlockOn2);
			else
				memDC.SelectObject(*BlockOn2Flash);
			break;

		default:
			if (!flash)
				memDC.SelectObject(*BlockOff);
			else
				memDC.SelectObject(*BlockOffFlash);
	};

	pt.x = Grid->Grid[x][y]->x;
	pt.y = Grid->Grid[x][y]->y;

	dc.BitBlt(pt.x, pt.y,
							BLOCKWIDTH, BLOCKHEIGHT, memDC, 0, 0, SRCCOPY);
}


void TGameWindow::WriteScore(TDC& dc)
{
	char s[20] = "";
	TBrush brush(TColor::LtGray);
	dc.SelectObject(brush);
	dc.SelectStockObject(NULL_PEN);
	dc.SetBkMode(TRANSPARENT);

	// Hits
	dc.Rectangle(300, 100, 390, 120);
	sprintf(s, "%6.6d", Counter);
	dc.TextOut(TPoint(300, 100), s, strlen(s));

	dc.Rectangle(310, 200, 390, 220);
	if (useHardMode == MODEHARD)
		dc.TextOut(TPoint(310, 200), "Hard", 5);
	else
		dc.TextOut(TPoint(310, 200), "Easy", 5);

	dc.RestoreObjects();

	if (IsGameOver)
		DrawGameOver(dc);

}

void TGameWindow::CmAbout()
{
	TDialog(this, About).Execute();
}

void TGameWindow::CmTopTen()
{
	TScoresDialog((TWindow *)this, (TResId)TOPTENDLG, (TModule *)0, config).Execute();
}


void TGameWindow::CmHelp()
{
	 spawnlp(P_NOWAITO,"view.exe", "view.exe","litesout.inf", NULL);
}

void TGameWindow::CmMode()
{
	int rc;
	if (useHardMode == MODEEASY)
		rc = MessageBox("Switch to HARD mode?", "Mode", MB_YESNO);
	else
		rc = MessageBox("Switch to EASY mode?", "Mode", MB_YESNO);

	if (rc == IDNO) return;

	useHardMode == MODEEASY ? useHardMode = MODEHARD : useHardMode = MODEEASY;
	config->SetMode(useHardMode);
	CmResetGame();
}

void TGameWindow::CmResetGame()
{
	Parent->DrawMenuBar();
		for (int i = 0; i < BLOCKSACROSS; i++)
			for (int j = 0; j < BLOCKSDOWN; j++)
				{
					Grid->Grid[i][j]->SetMaxState(useHardMode);
					Grid->Grid[i][j]->SetState(0);
				 };
	Counter = 0;
	flashX = flashY = -1;
	IsGameOver = FALSE;
	NeedScrambled = TRUE;
	Invalidate(TRUE);
}

BOOL TGameWindow::EvEraseBkgnd(HDC hDC)
{
	TDC dc(hDC);
	dc.TextRect(GetClientRect(), TColor::LtGray);
	return TRUE;
}

void TGameWindow::EvDestroy()
{
	TWindow::EvDestroy();
}

void TGameWindow::EvLButtonDown(UINT, TPoint& clickPoint)
{
	int xVal;
	int yVal;
	SetCursor(GetModule(), CUR_MALETDWN);

	TPoint point;
	GetCursorPos(point);
	SetCursorPos(point);
	if (IsGameOver)
		return;

	xVal = (int)(clickPoint.x - GRIDXOFFSET) / BLOCKWIDTH;
	yVal = (int)(clickPoint.y - GRIDYOFFSET) / BLOCKHEIGHT;

	if ((xVal >=BLOCKSACROSS) || (yVal >= BLOCKSDOWN)) return;

	BlocksLit = Grid->ToggleBlock(xVal, yVal);

	TClientDC dc(*this);
	DrawBlock(dc, xVal, yVal, TRUE);

	if (xVal > 0) DrawBlock(dc, xVal- 1, yVal, FALSE);
	if (xVal + 1 < Grid->Across) DrawBlock(dc, xVal + 1, yVal, FALSE);
	if (yVal > 0) DrawBlock(dc, xVal, yVal - 1, FALSE);
	if (yVal + 1 < Grid->Down) DrawBlock(dc, xVal, yVal + 1, FALSE);

	flashX = xVal;
	flashY = yVal;
	Counter++;
	if (!IsGameOver) CheckGameOver();
	WriteScore(dc);
}

void TGameWindow::CheckGameOver()
{
	if (BlocksLit) return;

	// restore block
	if (flashX != -1)
		{
			TClientDC dc(*this);
			DrawBlock(dc, flashX, flashY, FALSE);
			flashX = flashY = -1;
		};

	IsGameOver = TRUE;
	if (config->IsHigh(Counter))  //is this a high score?
	{
		char name[60] = "";
		TInputDialog(this, "Top 10 Score", "Enter Your Name ", name, 60).Execute();
		config->AddScore(name, Counter);
		TScoresDialog((TWindow *)this, (TResId)TOPTENDLG, (TModule *)0, config).Execute();
	};
}

void TGameWindow::EvLButtonUp(UINT, TPoint&)
{
	SetCursor(GetModule(), CUR_MALET);

	TPoint point;
	GetCursorPos(point);
	SetCursorPos(point);

	if (flashX != -1)
		{
			TClientDC dc(*this);
			DrawBlock(dc, flashX, flashY, FALSE);
			flashX = flashY = -1;
		};

}

void TGameWindow::Paint(TDC& dc, BOOL, TRect&)
{
		DrawScoreboard(dc);
		WriteScore(dc);

		for (int i = 0; i < BLOCKSACROSS; i++)
			for (int j = 0; j < BLOCKSDOWN; j++)
				DrawBlock(dc, i, j, FALSE);

		if (NeedScrambled)
			{
				ScrambleBlocks();
				NeedScrambled = FALSE;
			};
}


// --------------- TApp ------------------------

class TApp : public TApplication {
	public:
		TApp() : TApplication() {}
		void InitMainWindow()
		{
			EnableCtl3d();
			MainWindow = new TFrameWindow(0, "Lites Out!", new TGameWindow, TRUE);
			MainWindow->AssignMenu(LitesMenu);
			MainWindow->SetIcon(this, ICO_LITESOUT);
			MainWindow->Attr.X = 100;
			MainWindow->Attr.Y = 40;
			MainWindow->Attr.W = 400;
			MainWindow->Attr.H = 400;
	}
};

int OwlMain(int /*argc*/, char* /*argv*/ [])
{
	return TApp().Run();
}
