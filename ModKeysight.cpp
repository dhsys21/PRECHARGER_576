//---------------------------------------------------------------------------


#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
// Keysight
//#pragma comment(lib, "KeysightBT2200.lib")
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"


TMod_Keysight *Mod_Keysight;
//---------------------------------------------------------------------------
__fastcall TMod_Keysight::TMod_Keysight(TComponent* Owner)
	: TDataModule(Owner)
{
	for(int i=0;i<4;i++)
		m_bConnect[i] = false;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Keysight::KeysSetVisaAddress(AnsiString ipaddress, AnsiString port)
{
	keysight_ipaddress = ipaddress;
	keysight_port = port;
}
//---------------------------------------------------------------------------
int __fastcall TMod_Keysight::KeysConnect(int type)
{
	int nResult = -1;

    UnicodeString sResult;
	keysight_cmd = "*IDN?";
	//sResult = KeysRunCommand2(keysight_cmd);
	if(sResult.Pos("Keysight") > 0) nResult = 1;

	return nResult;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TMod_Keysight::KeysRunCommand2(AnsiString cmd)
{
	char* sResult;
   //	sResult = runCOMMANDKeysight(cmd.c_str(), keysight_ipaddress.c_str(), keysight_port.c_str());
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Keysight::KeysRunCommand(AnsiString cmd)
{
	char* sResult;

	if(m_bConnect == false) return "-1";

	//sResult = runCOMMANDKeysight(cmd.c_str(), keysight_ipaddress.c_str(), keysight_port.c_str());

	return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Keysight::KeysRunCommandWithNoResult(AnsiString cmd)
{
	if(m_bConnect == false) return;

	//runCOMMANDWithNoResultKeysight(cmd.c_str(), keysight_ipaddress.c_str(), keysight_port.c_str());
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Keysight::KeysGetTime()
{
	char* sResult;

	if(m_bConnect == false) return "Not Connected";

	//sResult = GetTimeKeysight(keysight_ipaddress.c_str(), keysight_port.c_str(), 50);
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Keysight::KeysGetResult()
{
	char* sResult;

	if(m_bConnect == false) return "Not Connected";

	//sResult = GetResultKeysight(keysight_ipaddress.c_str(), keysight_port.c_str(), 50);
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Keysight::KeysGetStatus()
{
	char* sResult;

	if(m_bConnect == false) return "Not Connected";

	//sResult = GetStatusKeysight(keysight_ipaddress.c_str(), keysight_port.c_str());
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Keysight::KeysGetMeasureValue()
{
	char* sResult;
	//sResult = getMeasureValueKeysight(keysight_ipaddress.c_str(), keysight_port.c_str());
	return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Keysight::KeysStopSequence()
{
	keysight_cmd = "SEQ:ABORT";
	//KeysRunCommandWithNoResult(keysight_cmd);
//	runCOMMANDKeysight(cmd.c_str(), keysight_ipaddress.c_str(), keysight_port.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TMod_Keysight::KeysRst()
{
	keysight_cmd = "*RST";
	//KeysRunCommandWithNoResult(keysight_cmd);
//	runCOMMANDKeysight(cmd.c_str(), keysight_ipaddress.c_str(), keysight_port.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMod_Keysight::KeysSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
{
	keysight_cmd = "CELL:DEF:QUICk 1";
	//KeysRunCommandWithNoResult(keysight_cmd);

	Sleep(keysighttime);
	keysight_cmd = "SEQ:STEP:DEF 1,1,PRECHARGE,30,1,2";
	//KeysRunCommandWithNoResult(keysight_cmd);

	Sleep(keysighttime);
	keysight_cmd = "SEQ:STEP:DEF 1,2,CHARGE," + cTime + "," + cCurr + "," + cVolt;
	//KeysRunCommandWithNoResult(keysight_cmd);

	Sleep(keysighttime);
	keysight_cmd = "SEQ:TEST:DEF 1,2,1,VOLT_LE,0.1,BEFORE,90,FAIL";
	//KeysRunCommandWithNoResult(keysight_cmd);

//	Sleep(keysighttime);
//	keysight_cmd = "SYST:PROB:LIM 1,0";
//    KeysRunCommandWithNoResult(keysight_cmd);
}
//---------------------------------------------------------------------------
void __fastcall TMod_Keysight::KeysStartCharge()
{
	//startCharge(keysight_ipaddress.c_str(), keysight_port.c_str(), 100, 500);
}
//---------------------------------------------------------------------------
void __fastcall TMod_Keysight::KeysStartChargeWithSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
{
	keysight_cmd = "CELL:DEF:QUICk 1";
	//KeysRunCommandWithNoResult(keysight_cmd);
	Sleep(keysighttime);
	keysight_cmd = "SEQ:STEP:DEF 1,1,PRECHARGE,30,1,2";
	//KeysRunCommandWithNoResult(keysight_cmd);
	Sleep(keysighttime);
	keysight_cmd = "SEQ:STEP:DEF 1,2,CHARGE," + cTime + "," + cCurr + "," + cVolt;
	//KeysRunCommandWithNoResult(keysight_cmd);
	Sleep(keysighttime);
	keysight_cmd = "SEQ:TEST:DEF 1,2,1,VOLT_LE,0.1,BEFORE,90,FAIL";
	//KeysRunCommandWithNoResult(keysight_cmd);
	Sleep(keysighttime);

    KeysStartCharge();
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TMod_Keysight::KeysGetDef(AnsiString seq_id, AnsiString step_id)
{
	UnicodeString sResult;
	keysight_cmd = "SEQ:STEP:DEF? " + seq_id + "," + step_id;
	//sResult = KeysRunCommand(keysight_cmd);
	return sResult;
}
//---------------------------------------------------------------------------

void __fastcall TMod_Keysight::Timer_AutoConnectTimer(TObject *Sender)
{
    AnsiString keysight_cmd;
	keysight_cmd = "*IDN?";
//	if(KeysRunCommand(keysight_cmd) == "-1") {
//		if(BaseForm->nForm[0]->VISATYPE == VISADOTNET)
//			BaseForm->nForm[0]->Timer_KeysightConnect->Enabled = true;
//		else if(BaseForm->nForm[0]->VISATYPE == VISA)
//			Mod_Visa->Connect(BaseForm->nForm[0]->editKeysightIPAddress->Text, BaseForm->nForm[0]->editKeysightPort->Text,Tag);
//	}
}
//---------------------------------------------------------------------------

