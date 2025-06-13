//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormConfig.h"
#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvSmoothButton"
#pragma link "AdvSmoothPanel"
#pragma resource "*.dfm"
TConfigForm *ConfigForm;
//---------------------------------------------------------------------------
__fastcall TConfigForm::TConfigForm(TComponent* Owner)
	: TForm(Owner)
{
	PLC_IPADDRESS = editPLCIPAddress->Text;
	PLC_PCPORT = editPLCPortPC->Text.ToIntDef(5005);
	PLC_PLCPORT = editPLCPortPLC->Text.ToIntDef(5006);
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::btnConnectPLCClick(TObject *Sender)
{
	Mod_PLC->Connect(PLC_IPADDRESS, PLC_PLCPORT, PLC_PCPORT);
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::btnDisConnectPLCClick(TObject *Sender)
{
    Mod_PLC->DisConnect();
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::btnCloseConnConfigClick(TObject *Sender)
{
	ConfigForm->Visible = false;
}
//---------------------------------------------------------------------------
