//---------------------------------------------------------------------------

#ifndef FormMeasureInfoH
#define FormMeasureInfoH
//---------------------------------------------------------------------------
#include "DEFINE.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include "AdvSmoothButton.hpp"
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <vector>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <jpeg.hpp>
#include "AdvSmoothButton.hpp"
#include "FormLanguage.h"
//---------------------------------------------------------------------------
using namespace std;

class TMeasureInfoForm : public TForm
{
__published:	// IDE-managed Components
	TSaveDialog *SaveDialog;
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
	TPanel *clir;
	TPanel *clocv;
	TGroupBox *gbChInfo;
	TPanel *pnlChannel;
	TPanel *pch;
	TPanel *pnlPos;
	TPanel *ppos;
	TPanel *pLocal;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox1;
	TPanel *pstage;
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
	TAdvSmoothButton *btnProbeOpen;
	TAdvSmoothButton *btnProbeClose;
	TAdvSmoothButton *btnAuto;
	TAdvSmoothButton *btnStop;
	TAdvSmoothButton *btnSave;
	TTimer *probeTimer;
	TAdvSmoothButton *btnInit;
	TCheckBox *chkGraph;
	TChart *chartVoltage;
	TLineSeries *Series1;
	TLineSeries *Series2;
	TLineSeries *Series3;
	TChart *chartCurrent;
	TLineSeries *LineSeries1;
	TLineSeries *LineSeries2;
	TLineSeries *LineSeries3;
	TAdvSmoothButton *btnSetPrecharger;
	TAdvSmoothButton *btnReset;
	TAdvSmoothButton *btnRBT;
	TAdvSmoothButton *btnReport;
	TAdvSmoothButton *btnCAL;
	TAdvSmoothButton *btnSTC;
	TAdvSmoothButton *btnENA;
	TAdvSmoothButton *btnDST;
	TAdvSmoothButton *btnDisChargeSet;
	TAdvSmoothButton *btnDisCharge;
	TTimer *Timer_SetStep;
	TTimer *Timer_SetStep2;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnAutoClick(TObject *Sender);
	void __fastcall btnProbeClick(TObject *Sender);
	void __fastcall ChInfoMouseEnter(TObject *Sender);
	void __fastcall ChInfoMouseLeave(TObject *Sender);
	void __fastcall Panel19Click(TObject *Sender);
	void __fastcall Panel35Click(TObject *Sender);
	void __fastcall btnProbeOpenClick(TObject *Sender);
	void __fastcall probeTimerTimer(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);
	void __fastcall btnInitClick(TObject *Sender);
	void __fastcall chkGraphClick(TObject *Sender);
	void __fastcall btnStopClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnResetClick(TObject *Sender);
	void __fastcall btnSetPrechargerClick(TObject *Sender);
	void __fastcall GroupBox3Click(TObject *Sender);
	void __fastcall btnRBTClick(TObject *Sender);
	void __fastcall btnReportClick(TObject *Sender);
	void __fastcall btnCALClick(TObject *Sender);
	void __fastcall btnSTCClick(TObject *Sender);
	void __fastcall btnENAClick(TObject *Sender);
	void __fastcall btnDSTClick(TObject *Sender);
	void __fastcall btnDisChargeSetClick(TObject *Sender);
	void __fastcall Timer_SetStepTimer(TObject *Sender);
	void __fastcall Timer_SetStep2Timer(TObject *Sender);

private:	// User declarations
	void __fastcall MakePanel(AnsiString type);
	void __fastcall MakeUIPanel(AnsiString type);
	void __fastcall SetOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index);
	void __fastcall SetUIOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index);
    int pnl_nw;
    int pnl_nh;
public:		// User declarations
	TPanel *pvolt[MAXCHANNEL];
	TPanel *pcurr[MAXCHANNEL];
	TPanel *pUIx[LINECOUNT];
	TPanel *pUIy[LINECOUNT];

	int stage;
    int nStep;
    int nSetCount;
    void __fastcall SetStep();

    bool n_bManualStart;

	__fastcall TMeasureInfoForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMeasureInfoForm *MeasureInfoForm;
//---------------------------------------------------------------------------
#endif
