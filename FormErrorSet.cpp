//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_ErrorSet *Form_ErrorSet;
//---------------------------------------------------------------------------
__fastcall TForm_ErrorSet::TForm_ErrorSet(TComponent* Owner)
	: TForm(Owner)
{
    stage = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorSet::ChangeMessage(UnicodeString msg1)
{
    msg1 = StringReplace(msg1, L"\\r\\n", L"\r\n", TReplaceFlags() << rfReplaceAll);
	Label_Msg1->Caption = msg1;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorSet::DisplayErrorMessage(int nStage)
{
    stage = nStage;
	if(!this->Visible)
	{
		Timer_BringToFront->Enabled = true;
        Mod_PLC->SetValue(PC_D_PRE_ERROR, 1);

        Label_Msg1->Caption = L"The current settings are invalid. \r\n\r\nPlease reset and try again.";
		SaveErrorLog(Label_Msg1->Caption);

		this->Position = poMainFormCenter;
		this->BringToFront();
		this->Show();
		this->Left = this->Left - BaseForm->Left;
	}
	else this->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorSet::SaveErrorLog(AnsiString msg1)
{
    AnsiString str, dir;
	int file_handle;

	dir = (AnsiString)LOG_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	str = dir + "ERROR_" + Now().FormatString("yymmdd-hh") + ".log";

	if(FileExists(str))
		file_handle = FileOpen(str, fmOpenWrite);
	else{
		file_handle = FileCreate(str);
	}

	FileSeek(file_handle, 0, 2);

	str = Now().FormatString("yyyy-mm-dd hh:nn:ss> ") + msg1 + "\r\n";
	FileWrite(file_handle, str.c_str(), str.Length());
	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorSet::Timer_BringToFrontTimer(TObject *Sender)
{
    this->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorSet::timerErrorOffTimer(TObject *Sender)
{
    Mod_PLC->SetValue(PC_D_PRE_ERROR, 0);
    Timer_BringToFront->Enabled = false;
    timerErrorOff->Enabled = false;
    this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ErrorSet::btnOKClick(TObject *Sender)
{
    timerErrorOff->Enabled = true;
}
//---------------------------------------------------------------------------
