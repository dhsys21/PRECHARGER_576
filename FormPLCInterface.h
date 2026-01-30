//---------------------------------------------------------------------------

#ifndef FormPLCInterfaceH
#define FormPLCInterfaceH
//---------------------------------------------------------------------------
#include "DEFINE.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "AdvSmoothButton.hpp"
#include "AdvSmoothPanel.hpp"
//---------------------------------------------------------------------------
class TForm_PLCInterface : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox2;
	TListView *ListView_PC;
	TListView *ListView_PLC;
	TTimer *Timer_Update;
	TAdvSmoothPanel *Panel1;
	TLabel *lblConfiguration;
	TAdvSmoothButton *btnSaveConfig;
	TPanel *pnlCurrMin;
	TEdit *editCurrMin;
	TGroupBox *GroupBox4;
	TLabel *Label2;
	TLabel *Label3;
	TComboBox *cbAddress;
	TEdit *editValue;
	TButton *btnWriteValue;
	TGroupBox *GroupBox3;
	TEdit *editIrOcvNg;
	TButton *btnWriteNgValue;
	TGroupBox *GroupBox1;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *editVoltage;
	TEdit *editCurrent;
	TButton *btnWriteIrOcvValue;
	TCheckBox *chkAllData;
	void __fastcall Timer_UpdateTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall GroupBox2Click(TObject *Sender);
	void __fastcall btnWriteValueClick(TObject *Sender);
	void __fastcall btnWriteNgValueClick(TObject *Sender);
	void __fastcall btnWriteIrOcvValueClick(TObject *Sender);
	void __fastcall chkAllDataClick(TObject *Sender);
private:	// User declarations
	void __fastcall SetListViewPLC(int nTag);
	void __fastcall SetListViewPC(int nTag);
	void __fastcall SetListViewPLC2();
	void __fastcall SetListViewPC2();

	void __fastcall AddListView(TListView *list, AnsiString address, AnsiString name);
	double auto_manual_plc[1], auto_manual_plc_old[1];

    TListItem *tempListItem;
public:		// User declarations
	__fastcall TForm_PLCInterface(TComponent* Owner);
	TListView *nListView_PLC[1];
	TListView *nListView_PC[1];
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_PLCInterface *Form_PLCInterface;
//---------------------------------------------------------------------------
#endif
