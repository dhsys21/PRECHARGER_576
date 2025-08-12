//---------------------------------------------------------------------------

#ifndef RVMO_mainH
#define RVMO_mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "DEFINE.h"
#include "FormTotal.h"
#include "FormPLCInterface.h"
#include "FormLanguage.h"
#include "FormNgCountError.h"
#include "FormGraph.h"
#include "Util.h"
#include <jpeg.hpp>
#include <Graphics.hpp>
#include "mmsystem.h"
#include "AdvSmoothButton.hpp"
#include "AdvSmoothPanel.hpp"
#include "Modplc.h"
#include "ModVisa.h"
#include "ModVisaArray.h"
#include "ModVisaThread.h"
#include "FormError.h"
#include <Vcl.Imaging.pngimage.hpp>
#pragma link "wininet.lib"
#include "Mdfunc.h"

//---------------------------------------------------------------------------

class TBaseForm : public TForm
{
__published:	// IDE-managed Components
	TTimer *ClockTimer;
	TTimer *FileDeleteTimer;
	TPanel *pon;
	TPanel *poff;
	TAdvSmoothButton *advPLCInterfaceShow;
	TLabel *Label1;
	TAdvSmoothButton *AdvSmoothButton_Configuration;
	TAdvSmoothPanel *AdvSmoothPanel_PLC;
	TAdvSmoothButton *advGraphShow;
	TGroupBox *gbPLCConnection;
	TEdit *editPLCIPAddress;
	TPanel *Panel25;
	TPanel *Panel26;
	TEdit *editPLCPortPC;
	TAdvSmoothButton *btnConnectPLC;
	TAdvSmoothButton *btnDisConnectPLC;
	TPanel *pnlportplc;
	TEdit *editPLCPortPLC;
	TImage *Image12;
	TMemo *Memo1;
	TLabel *lblTitle;
	TPanel *pPassword;
	TPanel *Panel61;
	TEdit *PassEdit;
	TAdvSmoothButton *cancelBtn2;
	TAdvSmoothButton *PasswordBtn;
	TLabel *lblLineNo;
	TPanel *Panel1;
	TImage *Image1;
	TImage *Image2;
	TImage *Image3;
	TImage *Image4;
	TImage *Image5;
	TImage *Image6;
	TImage *Image7;
	TImage *Image8;
	TImage *Image9;
	TImage *Image10;
	TImage *Image11;
	TAdvSmoothPanel *pnlBT1;
	TAdvSmoothPanel *pnlBT2;
	TAdvSmoothButton *btnViewLog;
	TAdvSmoothButton *btnViewData;
	TPanel *pstepInfo1;
	TRadioButton *rbKorean;
	TRadioButton *rbEnglish;
	TGroupBox *GroupBox2;
	TButton *Button1;
	TButton *Button5;
	TButton *btnNgCountErr;
	TCheckBox *chkTest;
	TRadioButton *rbChinese;
	TAdvSmoothButton *btnInit;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ClockTimerTimer(TObject *Sender);
	void __fastcall FileDeleteTimerTimer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall advPLCInterfaceShowClick(TObject *Sender);
	void __fastcall ptimeClick(TObject *Sender);
	void __fastcall Label1DblClick(TObject *Sender);
	void __fastcall Memo1Change(TObject *Sender);
	void __fastcall advGraphShowClick(TObject *Sender);
	void __fastcall AdvSmoothPanel_PLCClick(TObject *Sender);
	void __fastcall btnConnectPLCClick(TObject *Sender);
	void __fastcall btnDisConnectPLCClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall PasswordBtnClick(TObject *Sender);
	void __fastcall cancelBtn2Click(TObject *Sender);
	void __fastcall PassEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall btnViewLogClick(TObject *Sender);
	void __fastcall btnViewDataClick(TObject *Sender);
	void __fastcall rbKoreanClick(TObject *Sender);
	void __fastcall btnNgCountErrClick(TObject *Sender);
	void __fastcall lblTitleClick(TObject *Sender);
	void __fastcall btnInitClick(TObject *Sender);
	//void __fastcall ptimeDblClick(TObject *Sender);
	//void __fastcall ptimeClick(TObject *Sender);

private:	// User declarations

	int __fastcall GetStageIndex(AnsiString stage);


	void __fastcall MakePanel();

	int DeleteDay;
	int DeleteIndex;
	bool __fastcall DeleteLogFile(AnsiString FileName);
	bool __fastcall DeleteLogFolder(UnicodeString FolderName);

	bool __fastcall ReadSystemInfo();
    	void __fastcall WriteSystemInfo();


	AnsiString m_sTrayID, m_sCellModel, m_sLOTNumber, m_sCell_Serial[MAXCHANNEL + 1];

	int nSection, nStep;

public:		// User declarations
    __fastcall TBaseForm(TComponent* Owner);
    TTotalForm *nForm[CONTCOUNT];

    int FormCnt;
	int ErrorCount;
	void __fastcall IncErrorCount();
	void __fastcall DecErrorCount();
    void __fastcall SetColorPanel(TAdvSmoothPanel* pnl, bool bOn);

    bool __fastcall WaitForMilliSeconds(int milliseconds);
	//int __fastcall StringToInt(UnicodeString str, int def);
	//double __fastcall StringToDouble(UnicodeString str, double def);
    vector<int> __fastcall StringToVector(UnicodeString str);
    AnsiString __fastcall HexToAscii(AnsiString hex);
    AnsiString __fastcall HexToAscii(AnsiString hex, int length);
    vector<AnsiString> __fastcall  SplitString(AnsiString str, AnsiString delimiter);
	TImage *statusImage[13];

//---------------------------------------------------------------------------
// PLC
	AnsiString PLC_IPADDRESS;
	int PLC_PCPORT;
	int PLC_PLCPORT;
//---------------------------------------------------------------------------

};
//---------------------------------------------------------------------------
extern PACKAGE TBaseForm *BaseForm;
//---------------------------------------------------------------------------
#endif
