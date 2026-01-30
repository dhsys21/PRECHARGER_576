//---------------------------------------------------------------------------

#ifndef FormRemeasureH
#define FormRemeasureH
//---------------------------------------------------------------------------
#include "DEFINE.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <jpeg.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include "AdvSmoothButton.hpp"
//---------------------------------------------------------------------------


class TRemeasureForm : public TForm
{
__published:	// IDE-managed Components
	TSaveDialog *SaveDialog;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TGroupBox *GroupBox2;
	TPanel *pstage;
	TPanel *pcolor1;
	TPanel *pcolor2;
	TPanel *pcolor4;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TPanel *Panel5;
	TPanel *Panel6;
	TPanel *Panel7;
	TPanel *Panel8;
	TPanel *Panel9;
	TPanel *Panel10;
	TPanel *Panel11;
	TPanel *Panel12;
	TPanel *Panel13;
	TPanel *Panel14;
	TPanel *Panel15;
	TPanel *Panel16;
	TPanel *Panel17;
	TPanel *Panel18;
	TPanel *cl_line;
	TPanel *Panel19;
	TPanel *Panel21;
	TPanel *Panel22;
	TPanel *Panel23;
	TPanel *Panel24;
	TPanel *Panel25;
	TPanel *Panel26;
	TPanel *Panel27;
	TPanel *Panel28;
	TPanel *Panel29;
	TPanel *Panel30;
	TPanel *Panel31;
	TPanel *Panel32;
	TPanel *Panel33;
	TPanel *Panel34;
	TPanel *Panel35;
	TPanel *pnormal2;
	TPanel *pnormal1;
	TPanel *Panel1;
	TAdvSmoothButton *AccInitBtn;
	TLabel *Label4;
	TPanel *pnlTitleCh;
	TPanel *pnlCh;
	TPanel *pnlTitlePos;
	TPanel *pnlPos;
	TPanel *Panel39;
	TPanel *pAccCnt;
	TPanel *Panel40;
	TPanel *pAccDate;
	TPanel *pnlTitleTotalUse;
	TPanel *pnlTotalUse;
	TPanel *pnlTitleConsNg;
	TPanel *pnlConsNg;
	TPanel *pnlTitleTotalNg;
	TPanel *pnlTotalNg;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall chInitdblClick(TObject *Sender);
	void __fastcall AccInitBtnClick(TObject *Sender);
    void __fastcall ChInfoMouseEnter(TObject *Sender);
	void __fastcall ChInfoMouseLeave(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	void __fastcall MakePanel(AnsiString type);
	void __fastcall MakeUIPanel(AnsiString type);
	void __fastcall SetOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index);
	void __fastcall SetUIOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index);
	void __fastcall RefreshForm();

	TColor clLine, clLast, clSelect;
	int mode;
    int pnl_nw;
    int pnl_nh;

public:		// User declarations
	int *acc_remeasure;
    int *acc_totaluse;
    int *acc_consng;
	AnsiString *acc_init;
	int *acc_cnt;

	TPanel *pre[MAXCHANNEL];
	TPanel *pch[MAXCHANNEL];
    TPanel *pUIx[LINECOUNT];
	TPanel *pUIy[LINECOUNT];
	int stage;

	__fastcall TRemeasureForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRemeasureForm *RemeasureForm;
//---------------------------------------------------------------------------
#endif
