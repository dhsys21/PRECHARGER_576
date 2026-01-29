//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_ErrorReset *Form_ErrorReset;
//---------------------------------------------------------------------------
__fastcall TForm_ErrorReset::TForm_ErrorReset(TComponent* Owner)
	: TForm(Owner)
{
    stage = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorReset::Timer_BringToFrontTimer(TObject *Sender)
{
    this->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorReset::DisplayErrorMessage(int nStage)
{
    stage = nStage;
	if(!this->Visible)
	{
		Timer_BringToFront->Enabled = true;

		this->Position = poMainFormCenter;
		this->BringToFront();
		this->Show();
	}
	else this->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorReset::btnResetClick(TObject *Sender)
{
    Timer_BringToFront->Enabled = false;

    BaseForm->nForm[stage]->CmdReset();
    Sleep(1000);
    BaseForm->nForm[stage]->CmdResetTimer();
    this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorReset::btnCancelClick(TObject *Sender)
{
    Timer_BringToFront->Enabled = false;

    this->Close();
}
//---------------------------------------------------------------------------
