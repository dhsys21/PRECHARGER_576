//---------------------------------------------------------------------------

#include <vcl.h>
#include <DateUtils.hpp>
#pragma hdrstop

#include "RVMO_main.h"
#include "FormMeasureInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvSmoothButton"
#pragma link "AdvSmoothPanel"
#pragma resource "*.dfm"
TBaseForm *BaseForm;
//---------------------------------------------------------------------------
__fastcall TBaseForm::TBaseForm(TComponent* Owner)
	: TForm(Owner)
{
	FormCnt = CONTCOUNT;
	DeleteDay = 90;
	DeleteIndex = 0;
	this->Left = 0;
	this->Top = 0;
	this->Width = 1900;

	statusImage[0] = Image1;
	statusImage[1] = Image2;
	statusImage[2] = Image3;
	statusImage[3] = Image4;
	statusImage[4] = Image5;
	statusImage[5] = Image6;
	statusImage[6] = Image7;
	statusImage[7] = Image8;
	statusImage[8] = Image9;
	statusImage[9] = Image10;
	statusImage[10] = Image11;
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::FormCreate(TObject *Sender)
{
	if(!DirectoryExists((AnsiString)APP_PATH))MkDir((AnsiString)APP_PATH);
	if(!DirectoryExists((AnsiString)BIN_PATH))MkDir((AnsiString)BIN_PATH);
	if(!DirectoryExists((AnsiString)DATA_PATH))MkDir((AnsiString)DATA_PATH);
	if(!DirectoryExists((AnsiString)LOG_PATH))MkDir((AnsiString)LOG_PATH);
	if(!DirectoryExists((AnsiString)TRAY_PATH))MkDir((AnsiString)TRAY_PATH);

	ReadSystemInfo();
}
//---------------------------------------------------------------------------

void __fastcall TBaseForm::FormShow(TObject *Sender)
{
	this->Left = 0;
	MakePanel();
	this->HorzScrollBar->Position = 0;

	Form_Error->Parent = this;
	Form_Error->Position = poDesigned;

    Form_Language->LanguageChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TBaseForm::MakePanel()
{
	int nx, ny;
	nx = 3;
	ny = 85;

	for(int i = 0; i < FormCnt;){
		nForm[i] = new TTotalForm(Owner);
		nForm[i]->Parent = this;
		nForm[i]->Left = nx;
		nForm[i]->Top = ny;
		nForm[i]->Position = poDesigned;
		nForm[i]->Tag = i;
		nForm[i]->Visible = true; 		// WM_SHOW 이벤트 발생

		i += 1;
//		nx += (nForm[i-1]->Width + 6);
		nx += 466;
	}
}

void __fastcall TBaseForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	for(int i=0; i<FormCnt ;++i){
		nForm[i]->Close();
	}
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (MessageDlg("Close the form?", mtConfirmation, TMsgDlgButtons() << mbOK << mbCancel,1) == mrOk){
		pPassword->Visible = !pPassword->Visible;
        pPassword->BringToFront();
	}
	CanClose = false;
}

//---------------------------------------------------------------------------
//--------------------- 버튼 색상표시 (연결정보)  ---------------------------
//---------------------------------------------------------------------------
void __fastcall TBaseForm::ClockTimerTimer(TObject *Sender)
{
	if(Mod_PLC->ClientSocket_PC->Active && Mod_PLC->ClientSocket_PLC->Active)
	{
		AdvSmoothPanel_PLC->Fill->Color = pon->Color;
		AdvSmoothPanel_PLC->Fill->ColorMirror = pon->Color;
		AdvSmoothPanel_PLC->Fill->ColorMirrorTo = pon->Color;
		AdvSmoothPanel_PLC->Fill->ColorTo = pon->Color;
	}else{
		AdvSmoothPanel_PLC->Fill->Color = poff->Color;
		AdvSmoothPanel_PLC->Fill->ColorMirror = poff->Color;
		AdvSmoothPanel_PLC->Fill->ColorMirrorTo = poff->Color;
		AdvSmoothPanel_PLC->Fill->ColorTo = poff->Color;
	}


	if(Now().FormatString("hhnn") == "0700") {
		DeleteDay = 90;	// 12시가 되면 DeleteDay를 90으로 초기화
	}
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::SetColorPanel(TAdvSmoothPanel* pnl, bool bOn)
{
    if(bOn == true)
	{
		pnl->Fill->Color = pon->Color;
		pnl->Fill->ColorMirror = pon->Color;
		pnl->Fill->ColorMirrorTo = pon->Color;
		pnl->Fill->ColorTo = pon->Color;
	}else{
		pnl->Fill->Color = poff->Color;
		pnl->Fill->ColorMirror = poff->Color;
		pnl->Fill->ColorMirrorTo = poff->Color;
		pnl->Fill->ColorTo = poff->Color;
	}
}
void __fastcall TBaseForm::IncErrorCount()
{
	ErrorCount += 1;
	if(ErrorCount == 1){
		AnsiString path;
		path = AnsiString(BIN_PATH)+"Alarm.wav";
		PlaySound((TCHAR *)path.c_str(), NULL, SND_ASYNC|SND_LOOP);
	}
}
void __fastcall TBaseForm::DecErrorCount()
{
	ErrorCount = 0;
	PlaySound(NULL, NULL, 0);
}


//---------------------------------------------------------------------------
//--------------------- 파일 삭제  ------------------------------------------
//---------------------------------------------------------------------------
bool __fastcall TBaseForm::DeleteLogFile(AnsiString FileName)
{
	if(FileExists(FileName))
	{
		DeleteFile(FileName);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TBaseForm::DeleteLogFolder(UnicodeString FolderName)
{
	if(DirectoryExists(FolderName)) {
		SHFILEOPSTRUCT FileOp;

		FileOp.hwnd = this->WindowHandle;
		FileOp.wFunc = FO_DELETE;
		FileOp.pFrom = (TCHAR *)FolderName.c_str();
		FileOp.pTo = NULL;
		FileOp.fFlags = FOF_NOCONFIRMATION |FOF_NOERRORUI;
		FileOp.fAnyOperationsAborted = FALSE;
		SHFileOperationW(&FileOp);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::FileDeleteTimerTimer(TObject *Sender)
{
	TDateTime  dt;

	// 1시간에 파일 하나씩 삭제
	dt = Now() - DeleteDay;
	switch(DeleteIndex){
	case 0:
		DeleteLogFolder((AnsiString)LOG_PATH + dt.FormatString("yyyymmdd"));
		DeleteDay += 1;
		break;
	}
}
//---------------------------------------------------------------------------
//--------------------- 파일 삭제  ------------------------------------------
//---------------------------------------------------------------------------


void __fastcall TBaseForm::advGraphShowClick(TObject *Sender)
{
	Form_PLCInterface->Visible = false;

	Form_Graph->Position = poDesigned;
	Form_Graph->Left = 5;
	Form_Graph->Top = 85;
    Form_Graph->BringToFront();
	Form_Graph->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TBaseForm::ptimeClick(TObject *Sender)
{
	advPLCInterfaceShow->Visible = !advPLCInterfaceShow->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::Label1DblClick(TObject *Sender)
{
    Memo1->Visible = !Memo1->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::Memo1Change(TObject *Sender)
{
	if(Memo1->Lines->Count > 500) Memo1->Clear();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//--------------------- PLC  ------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TBaseForm::advPLCInterfaceShowClick(TObject *Sender)
{
	Form_Graph->Visible = false;

	Form_PLCInterface->Position = poDesigned;
	Form_PLCInterface->Left = 5;
	Form_PLCInterface->Top = 85;
	Form_PLCInterface->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::AdvSmoothPanel_PLCClick(TObject *Sender)
{
	gbPLCConnection->Visible = !gbPLCConnection->Visible;
	gbPLCConnection->BringToFront();
    Memo1->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TBaseForm::btnConnectPLCClick(TObject *Sender)
{
	WriteSystemInfo();
    ReadSystemInfo();
	Mod_PLC->Connect(PLC_IPADDRESS, PLC_PLCPORT, PLC_PCPORT);

}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::btnDisConnectPLCClick(TObject *Sender)
{
    Mod_PLC->DisConnect();
}



//---------------------------------------------------------------------------
//--------------------- 함수 정리  ------------------------------------------
//---------------------------------------------------------------------------
//double __fastcall TBaseForm::HexToDouble(UnicodeString str, int def)
//{
//    double dVal;
//
//}bool TBaseForm::WaitForMilliSeconds(int milliseconds)
{
    TDateTime startTime = Now();

    while (MilliSecondsBetween(Now(), startTime) < milliseconds)
    {
        Application->ProcessMessages();  // UI가 멈추지 않도록 함
    }
    return true;
}
int __fastcall TBaseForm::StringToInt(UnicodeString str, int def)
{
	int iVal;
	if(TryStrToInt(str, iVal) == true) return iVal;
	else return def;
}
//---------------------------------------------------------------------------
double __fastcall TBaseForm::StringToDouble(UnicodeString str, double def)
{
	double dVal;
	if(TryStrToFloat(str, dVal) == true) return dVal;
	else return def;
}
//---------------------------------------------------------------------------
vector<int> __fastcall TBaseForm::StringToVector(UnicodeString str)
{
	vector<int> numbers;

	int start = 1, end = 0;
	while((end = str.Pos(",")) > 0)
	{
		UnicodeString temp = str.SubString(start, end - start);
		numbers.push_back(temp.ToIntDef(0));
		str.Delete(start, end);
	}
	numbers.push_back(str.SubString(start, str.Length() - start + 1).ToIntDef(0));

    return numbers;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TBaseForm::HexToAscii(AnsiString hex)
{
	AnsiString result = "";
    if(hex.Length() % 2 == 1) hex += "00";

    for(int i = 1; i < hex.Length(); i += 2){
        String hexByte = hex.SubString(i, 2);
        char asciiChar = (char)StrToInt("$" + hexByte);
        result += asciiChar;
    }
    return result;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TBaseForm::HexToAscii(AnsiString hex, int length)
{
	AnsiString result = "";

    for(int i = 1; i < length; i += 2){
        String hexByte = hex.SubString(i, 2);
        char asciiChar = (char)StrToInt("$" + hexByte);
        result += asciiChar;
    }
    return result;
}
//--------------------------------------------------------------------------- '
//---------------------------------------------------------------------------
vector<AnsiString> __fastcall TBaseForm::SplitString(AnsiString str, AnsiString delimiter)
{
	vector<AnsiString> result;

    int pos = 0;
    while((pos = str.Pos(delimiter)) > 0){
        result.push_back(str.SubString(1, pos - 1));
        str = str.SubString(pos + 1, str.Length() - pos);
    }
    result.push_back(str);

    return result;
}
//---------------------------------------------------------------------------
bool __fastcall TBaseForm::ReadSystemInfo()
{
	TIniFile *ini;

	AnsiString file;
	file = (AnsiString)BIN_PATH + "config.inf";

	ini = new TIniFile(file);

//	editPLCIPAddress->Text = ini->ReadString("PRE_PLC_CONNECTION", "IP", "17.91.80.220");
//	editPLCPortPC->Text = ini->ReadString("PRE_PLC_CONNECTION", "PORT1", "5005");
//	editPLCPortPLC->Text = ini->ReadString("PRE_PLC_CONNECTION", "PORT2", "5006");

//	PLC_IPADDRESS = editPLCIPAddress->Text;
//	PLC_PCPORT = editPLCPortPC->Text.ToIntDef(5005);
//	PLC_PLCPORT = editPLCPortPLC->Text.ToIntDef(5006);

	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::WriteSystemInfo()
{
	TIniFile *ini;

	AnsiString file;
	file = (AnsiString)BIN_PATH + "config.inf";
	ini = new TIniFile(file);

	ini->WriteString("PRE_PLC_CONNECTION", "IP", editPLCIPAddress->Text);
	ini->WriteString("PRE_PLC_CONNECTION", "PORT1", editPLCPortPC->Text);
	ini->WriteString("PRE_PLC_CONNECTION", "PORT2", editPLCPortPLC->Text);

	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::OpenFolder(UnicodeString path)
{
	 ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------
//--------------------- 함수 정리  ------------------------------------------
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//--------------------- 프로그램 종료시 패스워드 입력 ------------------------
//---------------------------------------------------------------------------
void __fastcall TBaseForm::PasswordBtnClick(TObject *Sender)
{
	if(PassEdit->Text == "0000"){
		Application->Terminate();
	}
	else{
		MessageBox(Handle, L"Are you sure you’re spelling your password correctly?", L"ERROR", MB_OK|MB_ICONERROR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::cancelBtn2Click(TObject *Sender)
{
    pPassword->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TBaseForm::PassEditKeyPress(TObject *Sender, System::WideChar &Key)
{
    if(Key == VK_RETURN){
		PasswordBtnClick(Sender);
	}
}
//---------------------------------------------------------------------------
//--------------------- 프로그램 종료시 패스워드 입력 ------------------------
//---------------------------------------------------------------------------
void __fastcall TBaseForm::btnViewLogClick(TObject *Sender)
{
    OpenFolder(LOG_PATH);
}
//---------------------------------------------------------------------------

void __fastcall TBaseForm::btnViewDataClick(TObject *Sender)
{
    OpenFolder(DATA_PATH);
}
//---------------------------------------------------------------------------

void __fastcall TBaseForm::rbKoreanClick(TObject *Sender)
{
    TRadioButton *rbt = (TRadioButton *)Sender;
	if(rbt->Checked)
	{
		Form_Language->LanguageChange(rbt->Tag);
	}
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::btnNgCountErrClick(TObject *Sender)
{
    AnsiString stagename = "STAGE " + IntToStr(this->Tag + 1);
    WideString message = Form_Language->msgTooManyNG;
    Form_NgCountError->DisplayErrorMessage(stagename + " NG ERROR",
                                        message.c_bstr(),
                                        "Click [OK] button.", this->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TBaseForm::lblTitleClick(TObject *Sender)
{
    GroupBox2->Visible = !GroupBox2->Visible;
}
//---------------------------------------------------------------------------

