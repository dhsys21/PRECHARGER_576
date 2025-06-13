//---------------------------------------------------------------------------

#ifndef FormConfigH
#define FormConfigH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvSmoothButton.hpp"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TConfigForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox5;
	TEdit *editPLCIPAddress;
	TPanel *Panel25;
	TPanel *Panel26;
	TEdit *editPLCPortPC;
	TAdvSmoothButton *btnConnectPLC;
	TAdvSmoothButton *btnDisConnectPLC;
	TPanel *pnlportplc;
	TEdit *editPLCPortPLC;
	TAdvSmoothButton *btnSaveConnConfig;
	TAdvSmoothButton *btnCloseConnConfig;
	void __fastcall btnConnectPLCClick(TObject *Sender);
	void __fastcall btnDisConnectPLCClick(TObject *Sender);
	void __fastcall btnCloseConnConfigClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TConfigForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConfigForm *ConfigForm;
//---------------------------------------------------------------------------
#endif
