//---------------------------------------------------------------------------


#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TMod_Visa *Mod_Visa;
//---------------------------------------------------------------------------
__fastcall TMod_Visa::TMod_Visa(TComponent* Owner)
	: TDataModule(Owner)
{
	for(int i=0;i<4;i++)
		m_bConnect[i] = false;

	sizevalues = 5000;
	sizereport = 64;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	접속 & 해제
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::SetTimeOut(int nTime, int stage_num)
{
	status = viSetAttribute(m_vi[stage_num], VI_ATTR_TMO_VALUE, nTime);
	if (status < VI_SUCCESS) {
		m_bConnect[stage_num] = false;
		viStatusDesc(m_vi[stage_num], status, recv_status);
//			printf("viSetAttribute() error: %s\n", buf);
		viClose(m_vi[stage_num]);
		viClose(m_viRM[stage_num]);
		exit(EXIT_FAILURE);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::Connect(AnsiString ipaddress, AnsiString port, int stage_num, int WaitTime)
{
	try{

		viOpenDefaultRM(&m_viRM[stage_num]);
		usbID = "TCPIP::" + ipaddress + "::" + port + "::SOCKET";
		if(VI_SUCCESS == viOpen(m_viRM[stage_num], usbID.c_str(), VI_NULL, VI_NULL, &m_vi[stage_num]))
		{
			/* READ 시 데이터가 오기 까지 기다리는 최대 시간*/
			viSetAttribute(m_vi[stage_num], VI_ATTR_TMO_VALUE, WaitTime);

			/* 종료 문자을 입력 받으면 시간 상관 없이 리턴*/
			viSetAttribute(m_vi[stage_num], VI_ATTR_TERMCHAR_EN, VI_TRUE);

			actual = 0;
			m_bConnect[stage_num] = true;
			Mod_Keysight->m_bConnect[stage_num] = true;
			BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, "Connected...");
//				if(Mod_Keysight->Timer_AutoConnect->Enabled == false) Mod_Keysight->Timer_AutoConnect->Enabled = true;
			Timer_CheckConnection->Enabled = true;
		}
		else
		{
			BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, "Connect_Fail...");
			m_bConnect[stage_num] = false;
			Mod_Keysight->m_bConnect[stage_num] = false;
			return;
		}

	}
	catch(...){
		m_bConnect[stage_num] = false;
		Mod_Keysight->m_bConnect[stage_num] = false;
		return;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::DisConnect(int stage_num)
{
	m_bConnect[stage_num] = false;
	//Mod_Keysight->m_bConnect[stage_num] = false;
	BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, "DisConnected...");

	viClose(m_vi[stage_num]);
	viClose(m_viRM[stage_num]);

}
//---------------------------------------------------------------------------
bool __fastcall TMod_Visa::isConnected(int stage_num)
{
	AnsiString cmd, sResult;
	bool bConnected = true;
	char recv_msg[91];
	cmd = "*IDN?";
	BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, cmd);

	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		memset(&recv_msg, 0, sizeof(recv_msg)) ;
		status = viRead(m_vi[stage_num], (ViBuf)recv_msg, 91, &actual);
		recv_msg[actual] = 0;
		sResult = (AnsiString)recv_msg;
		BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, sResult);
	}

	if(sResult.Pos("Keysight") > 0) bConnected = true;
    else bConnected = false;
	return bConnected;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::runCommand(AnsiString cmd,int stage_num)
{
	AnsiString sResult = "-1";

	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(100);
		status = viRead(m_vi[stage_num], (ViBuf)recv_status, 4000, &actual);
		recv_status[actual] = 0;
		sResult = (AnsiString)recv_status;
		return sResult;
	}

	return sResult;
}
//---------------------------------------------------------------------------
int __fastcall TMod_Visa::runCommandWithNoResult(AnsiString cmd,int stage_num)
{
	int sResult = -1;
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	if(VI_SUCCESS <= status){
		return 1;
	}
	return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::KeysCLS(int stage_num)
{
	AnsiString keysight_cmd = "*CLS";
	runCommandWithNoResult(keysight_cmd,stage_num);
}

//---------------------------------------------------------------------------
void __fastcall TMod_Visa::KeysRST(int stage_num)
{
	AnsiString keysight_cmd = "*RST";
	runCommandWithNoResult(keysight_cmd,stage_num);
}

//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::KeysGetPreChargeDef(AnsiString seq_id, AnsiString step_id,int stage_num)
{
	AnsiString cmd;
	UnicodeString sResult = "-1";
	char recv_msg[46];

	cmd = "SEQ:STEP:DEF? " + seq_id + "," + step_id;
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(100);
		memset(&recv_msg, 0, sizeof(recv_msg)) ;
		status = viRead(m_vi[stage_num], (ViBuf)recv_msg, 46, &actual);
		recv_msg[actual] = 0;
		sResult = (AnsiString)recv_msg;
	}
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::KeysGetChargeDef(AnsiString seq_id, AnsiString step_id,int stage_num)
{
	AnsiString cmd;
	UnicodeString sResult = "-1";
	char recv_msg[44];

	cmd = "SEQ:STEP:DEF? " + seq_id + "," + step_id;
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(100);
		memset(&recv_msg, 0, sizeof(recv_msg)) ;
		status = viRead(m_vi[stage_num], (ViBuf)recv_msg, 44, &actual);
		recv_msg[actual] = 0;
		sResult = (AnsiString)recv_msg;
	}
	return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::KeysSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime,int stage_num)
{
	AnsiString keysight_cmd;
	AnsiString precharge_volt, precharge_current;
	if(BaseForm->StringToDouble(cCurr, 0.5) < 1) precharge_current = cCurr;
	else precharge_current = 1.0;

	keysight_cmd = "SYST:PROB:LIM 2,0";
	runCommandWithNoResult(keysight_cmd,stage_num);

	Sleep(keysighttime);
	keysight_cmd = "CELL:DEF:QUICk 1";
	runCommandWithNoResult(keysight_cmd,stage_num);

	Sleep(keysighttime);
	keysight_cmd = "SEQ:STEP:DEF 1,1,PRECHARGE,30,0.1,1.0";
	runCommandWithNoResult(keysight_cmd,stage_num);

	Sleep(keysighttime);
	keysight_cmd = "SEQ:STEP:DEF 1,2,CHARGE," + cTime + "," + cCurr + "," + cVolt;
	runCommandWithNoResult(keysight_cmd,stage_num);

	Sleep(keysighttime);
	keysight_cmd = "SEQ:TEST:DEF 1,2,1,CURR_LE,0.01,BEFORE,60,FAIL";
	runCommandWithNoResult(keysight_cmd,stage_num);

    Sleep(keysighttime);
	keysight_cmd = "SEQ:TEST:DEF 1,2,2,VOLT_LE,0.1,BEFORE,60,FAIL";
	runCommandWithNoResult(keysight_cmd,stage_num);
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::KeysSet2(AnsiString cVolt, AnsiString cCurr, AnsiString cTime,int stage_num)
{
	cVoltage = cVolt;
	cCurrent = cCurr;
	cTime = cTime;
	Timer_SetCharge->Tag = stage_num;

    nSetStep = 0;
    Timer_SetCharge->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::KeysStartCharge(int stage_num)
{
	AnsiString keysight_cmd;
	int enabletime = 100;
	int inittime = 500;
	for(int i = 1; i < 9; i++)
	{
		keysight_cmd = "CELL:ENABLE (@" + IntToStr(i) + "001:" + IntToStr(i) + "032),1";
		runCommandWithNoResult(keysight_cmd,stage_num);
//		status = viWrite(m_vi, (ViBuf)(keysight_cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
		Sleep(enabletime);
	}

	for(int i = 1; i < 9; i++)
	{
		keysight_cmd = "CELL:INIT (@" + IntToStr(i) + "001:" + IntToStr(i) + "032)";
		runCommandWithNoResult(keysight_cmd,stage_num);
//        status = viWrite(m_vi, (ViBuf)(keysight_cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
		if(i % 2 == 0)
			Sleep(inittime);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::KeysStartCharge2(int stage_num)
{
	Timer_EnableCharge->Tag = stage_num;
	Timer_InitCharge->Tag = stage_num;

	nEnableStep = 0;
    Timer_EnableCharge->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::KeysStopSequence(int stage_num)
{
	AnsiString keysight_cmd = "SEQ:ABORT";
	runCommandWithNoResult(keysight_cmd,stage_num);
	keysight_cmd = "*CLS";
	runCommandWithNoResult(keysight_cmd,stage_num);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::getMeasureValue(int stage_num)
{
	AnsiString cmd;
	UnicodeString sResult, sResult1 = "-1", sResult2 = "-1";
//	if(nType == 0) cmd = "MEAS:VOLT? 0000";
//	else if(nType == 1) cmd = "MEAS:CURR? 0000";
	actual = 0;
	actual2 = 0;
    ViUInt32 retCount; /* Return count from string I/O */
	ViChar buffer[BUFFER_SIZE]; /* Buffer for string I/O */
	SetTimeOut(TIMEOUT_VALUE, stage_num);

	KeysCLS(stage_num);
	Sleep(100);

	cmd = "MEAS:CURR? 0000";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
        Sleep(150);
		memset(&recv_values, 0, sizeof(recv_values)) ;
		status = viRead(m_vi[stage_num], (ViBuf)recv_values, 5000, &actual);
		recv_values[actual] = 0;
		sResult1 = (AnsiString)recv_values;
	}

	cmd = "MEAS:VOLT? 0000";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(150);
		memset(&recv_values, 0, sizeof(recv_values)) ;
		status = viRead(m_vi[stage_num], (ViBuf)recv_values, 5000, &actual);
		recv_values[actual] = 0;
		sResult2 = (AnsiString)recv_values;
	}
    sResult = sResult2 + sResult1;

	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::getReport(int stage_num)
{
	AnsiString cmd;
	UnicodeString sResult = "";
	SetTimeOut(TIMEOUT_VALUE, stage_num);

	KeysCLS(stage_num);
	Sleep(50);

	for(int i = 1; i < 9; i++)
	{
		cmd = "STAT:CELL:REP? (@" + IntToStr(i) + "001:" + IntToStr(i) + "032)";
		status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
		if(VI_SUCCESS <= status){
			Sleep(50);
			memset(&recv_status, 0, sizeof(recv_status)) ;
			status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
			recv_status[actual] = 0;
			sResult += (AnsiString)recv_status;
		}
		else{
			for(int i = 0; i < 62; i += 2){
				recv_status[i] = '0';
				recv_status[i + 1] = ',';
			}
			recv_status[62] = '0';
			recv_status[63] = '\n';
			sResult += (AnsiString)recv_status;
		}
	}

	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::GetCellStatus(int nCount,int stage_num)
{
	AnsiString sResult = "-1", cmd;
	int nIndex = nCount - 1;

	cmd = "STAT:CELL:REP? (@" + IntToStr(nCount) + "001:" + IntToStr(nCount) + "032)";
	//cmd = "STAT:CELL:REP? (@1001:8032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
//			memset(&recv_status, 0, sizeof(recv_status));
		status = viRead(m_vi[stage_num], (ViBuf)recv_status, 1024, &actual);
		recv_status[actual] = 0;
		sResult = (AnsiString)recv_status;
		return sResult;
	}
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::GetSeqStatus(int stage_num)
{
	AnsiString sResult = "-1", cmd;

	cmd = "STAT:CELL:VERB? 1001";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_status, 1024, &actual);
		recv_status[actual] = 0;
		sResult = (AnsiString)recv_status;
		return sResult;
	}
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::GetFinalResult(int stage_num, int channel)
{
	AnsiString sResult = "-1", cmd;

	//cmd = "STAT:CELL:VERB? 1001";
    cmd = "STAT:CELL:VERB? " + IntToStr(channel / 32 + 1) + "0" + FormatFloat("00", (channel % 32 + 1));
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_status, 1024, &actual);
		recv_status[actual] = 0;
		sResult = (AnsiString)recv_status;
		return sResult;
	}
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::GetRunningTime(int nCount, int stage_num)
{
	AnsiString sResult = "", cmd;
	int nIndex = nCount - 1;

	cmd = "CELL:STEP:TIME? (@1001:1032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_time, 1024, &actual);
		recv_time[actual] = 0;
		sResult = (AnsiString)recv_time;
	}
	else sResult += "-1";

	cmd = "CELL:STEP:TIME? (@2001:2032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_time, 1024, &actual);
		recv_time[actual] = 0;
		sResult = (AnsiString)recv_time;
	}
	else sResult += "-1";

	cmd = "CELL:STEP:TIME? (@3001:3032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_time, 1024, &actual);
		recv_time[actual] = 0;
		sResult = (AnsiString)recv_time;
	}
	else sResult += "-1";

	cmd = "CELL:STEP:TIME? (@4001:4032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_time, 1024, &actual);
		recv_time[actual] = 0;
		sResult = (AnsiString)recv_time;
	}
	else sResult += "-1";

	cmd = "CELL:STEP:TIME? (@5001:5032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_time, 1024, &actual);
		recv_time[actual] = 0;
		sResult = (AnsiString)recv_time;
	}
	else sResult += "-1";

	cmd = "CELL:STEP:TIME? (@6001:6032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_time, 1024, &actual);
		recv_time[actual] = 0;
		sResult = (AnsiString)recv_time;
	}
	else sResult += "-1";

	cmd = "CELL:STEP:TIME? (@7001:7032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_time, 1024, &actual);
		recv_time[actual] = 0;
		sResult = (AnsiString)recv_time;
	}
	else sResult += "-1";

	cmd = "CELL:STEP:TIME? (@8001:8032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi[stage_num], (ViBuf)recv_time, 1024, &actual);
		recv_time[actual] = 0;
		sResult = (AnsiString)recv_time;
	}
	else sResult += "-1";

    return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_Visa::GetRunningTime2(int nCount, int stage_num)
{
	AnsiString sResult = "-1", cmd;
	int nIndex = nCount - 1;

	cmd = "CELL:STEP:TIME? (@" + IntToStr(nCount) + "001:" + IntToStr(nCount) + "032)";
	//cmd = "CELL:TIME? (@1001:8032)";
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
//		memset(&recv_status, 0, sizeof(recv_status));
		status = viRead(m_vi[stage_num], (ViBuf)recv_time, 1024, &actual);
		recv_time[actual] = 0;
		sResult = (AnsiString)recv_time;
		return sResult;
	}
    return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::KeysStartChargeWithSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime,int stage_num)
{
	KeysSet(cVolt, cCurr, cTime,stage_num);
	KeysStartCharge(stage_num);
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::Timer_CheckConnectionTimer(TObject *Sender)
{
	for(int i=0;i<4;i++)
	{
		if(isConnected(i) == false) {
			DisConnect(i);
			Sleep(100);
			Connect(BaseForm->nForm[i]->editKeysightIPAddress->Text, BaseForm->nForm[i]->editKeysightPort->Text,i, 500);
		}
	}

}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::Timer_SetChargeTimer(TObject *Sender)
{
	// Set Charge
	AnsiString keysight_cmd;
	AnsiString precharge_volt, precharge_current;
	if(BaseForm->StringToDouble(cCurrent, 0.5) < 1) precharge_current = cCurrent;
	else precharge_current = 1.0;

	int stage_num = Timer_EnableCharge->Tag;

	switch(nSetStep)
	{
		case 0:
			keysight_cmd = "SYST:PROB:LIM 2,0";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 1:
			keysight_cmd = "CELL:DEF:QUICk 1";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 2:
			keysight_cmd = "SEQ:STEP:DEF 1,1,PRECHARGE,30,0.1,1.0";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 3:
			keysight_cmd = "SEQ:STEP:DEF 1,2,CHARGE," + cTime + "," + cCurrent + "," + cVoltage;
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 4:
			keysight_cmd = "SEQ:TEST:DEF 1,2,1,CURR_LE,0.01,BEFORE,60,FAIL";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 5:
			keysight_cmd = "SEQ:TEST:DEF 1,2,2,VOLT_LE,0.1,BEFORE,60,FAIL";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 6:
			nSetStep = 99;
			Timer_SetCharge->Enabled = false;
            BaseForm->nForm[stage_num]->nStep = 1;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::Timer_EnableChargeTimer(TObject *Sender)
{
	//* Enable
	AnsiString keysight_cmd;

	int stage_num = Timer_EnableCharge->Tag;
	switch(nEnableStep)
	{
		case 0:
			keysight_cmd = "CELL:ENABLE (@1001:1032),1";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 1:
			keysight_cmd = "CELL:ENABLE (@2001:2032),1";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 2:
			keysight_cmd = "CELL:ENABLE (@3001:3032),1";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 3:
			keysight_cmd = "CELL:ENABLE (@4001:4032),1";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 4:
			keysight_cmd = "CELL:ENABLE (@5001:5032),1";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 5:
			keysight_cmd = "CELL:ENABLE (@6001:6032),1";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 6:
			keysight_cmd = "CELL:ENABLE (@7001:7032),1";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 7:
			keysight_cmd = "CELL:ENABLE (@8001:8032),1";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nEnableStep++;
			break;
		case 8:
			nEnableStep = 99;
			nInitStep = 0;
			Timer_EnableCharge->Enabled = false;
            Timer_InitCharge->Enabled = true;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::Timer_InitChargeTimer(TObject *Sender)
{
	//* Init
    AnsiString keysight_cmd;

	int stage_num = Timer_InitCharge->Tag;
	switch(nInitStep)
	{
		case 0:
			keysight_cmd = "CELL:INIT (@1001:1032)";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nInitStep++;
			break;
		case 1:
			keysight_cmd = "CELL:INIT (@2001:2032)";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nInitStep++;
			break;
		case 2:
			keysight_cmd = "CELL:INIT (@3001:3032)";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nInitStep++;
			break;
		case 3:
			keysight_cmd = "CELL:INIT (@4001:4032)";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nInitStep++;
			break;
		case 4:
			keysight_cmd = "CELL:INIT (@5001:5032)";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nInitStep++;
			break;
		case 5:
			keysight_cmd = "CELL:INIT (@6001:6032)";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nInitStep++;
			break;
		case 6:
			keysight_cmd = "CELL:INIT (@7001:7032)";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nInitStep++;
			break;
		case 7:
			keysight_cmd = "CELL:INIT (@8001:8032)";
			runCommandWithNoResult(keysight_cmd,stage_num);
			nInitStep++;
			break;
		case 8:
            nInitStep = 99;
			Timer_InitCharge->Enabled = false;

			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::Timer_GetMeasTimer(TObject *Sender)
{
	//* Get Voltage, Current
    AnsiString cmd;
	UnicodeString sResult1 = "-1", sResult2 = "-1";
	int stage_num = Timer_GetMeas->Tag;

	actual = 0;
	actual2 = 0;
	ViUInt32 retCount; /* Return count from string I/O */
	ViChar buffer[BUFFER_SIZE]; /* Buffer for string I/O */
	SetTimeOut(TIMEOUT_VALUE, stage_num);

	switch(nMeasStep)
	{
		case 0:
			KeysCLS(stage_num);
			Timer_GetMeas->Interval = 50;
			nMeasStep++;
			break;
		case 1:
			cmd = "MEAS:CURR? 0000";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetMeas->Interval = 150;
			nMeasStep++;
			break;
		case 2:
			if(VI_SUCCESS <= status){
				memset(&recv_values, 0, sizeof(recv_values)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_values, 5000, &actual);
				recv_values[actual] = 0;
				sResult1 = (AnsiString)recv_values;
			}
			Timer_GetMeas->Interval = 50;
			nMeasStep++;
			break;
		case 3:
			cmd = "MEAS:VOLT? 0000";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetMeas->Interval = 150;
			nMeasStep++;
			break;
		case 4:
			if(VI_SUCCESS <= status){
				memset(&recv_values, 0, sizeof(recv_values)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_values, 5000, &actual);
				recv_values[actual] = 0;
				sResult2 = (AnsiString)recv_values;
			}
			nMeasStep++;

			meas_result = sResult2 + sResult1;
            bMeas = true;
            Timer_GetMeas->Interval = 50;
			Timer_GetMeas->Enabled = false;
			break;
		default:
        	break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::Timer_GetReportTimer(TObject *Sender)
{
	//* Get Report 0, 1, 2, 3, 4
    AnsiString cmd;
	int stage_num = Timer_GetMeas->Tag;

	SetTimeOut(TIMEOUT_VALUE, stage_num);
	int timer1 = 50, timer2 = 10;
	switch(nReportStep)
	{
		case 0:
			KeysCLS(stage_num);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 1:
			cmd = "STAT:CELL:REP? (@1001:1032)";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 2:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
				recv_status[actual] = 0;
				report_result += (AnsiString)recv_status;
			}
			else{
				for(int i = 0; i < 62; i += 2){
					recv_status[i] = '0';
					recv_status[i + 1] = ',';
				}
				recv_status[62] = '0';
				recv_status[63] = '\n';
				report_result += (AnsiString)recv_status;
			}
			Timer_GetReport->Interval = timer2;
			nReportStep++;
			break;
		case 3:
			cmd = "STAT:CELL:REP? (@2001:2032)";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 4:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
				recv_status[actual] = 0;
				report_result += (AnsiString)recv_status;
			}
			else{
				for(int i = 0; i < 62; i += 2){
					recv_status[i] = '0';
					recv_status[i + 1] = ',';
				}
				recv_status[62] = '0';
				recv_status[63] = '\n';
				report_result += (AnsiString)recv_status;
			}
			Timer_GetReport->Interval = timer2;
			nReportStep++;
			break;
		case 5:
			cmd = "STAT:CELL:REP? (@3001:3032)";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 6:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
				recv_status[actual] = 0;
				report_result += (AnsiString)recv_status;
			}
			else{
				for(int i = 0; i < 62; i += 2){
					recv_status[i] = '0';
					recv_status[i + 1] = ',';
				}
				recv_status[62] = '0';
				recv_status[63] = '\n';
				report_result += (AnsiString)recv_status;
			}
			Timer_GetReport->Interval = timer2;
			nReportStep++;
			break;
		case 7:
			cmd = "STAT:CELL:REP? (@4001:4032)";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 8:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
				recv_status[actual] = 0;
				report_result += (AnsiString)recv_status;
			}
			else{
				for(int i = 0; i < 62; i += 2){
					recv_status[i] = '0';
					recv_status[i + 1] = ',';
				}
				recv_status[62] = '0';
				recv_status[63] = '\n';
				report_result += (AnsiString)recv_status;
			}
			Timer_GetReport->Interval = timer2;
			nReportStep++;
			break;
		case 9:
			cmd = "STAT:CELL:REP? (@5001:5032)";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 10:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
				recv_status[actual] = 0;
				report_result += (AnsiString)recv_status;
			}
			else{
				for(int i = 0; i < 62; i += 2){
					recv_status[i] = '0';
					recv_status[i + 1] = ',';
				}
				recv_status[62] = '0';
				recv_status[63] = '\n';
				report_result += (AnsiString)recv_status;
			}
			Timer_GetReport->Interval = timer2;
			nReportStep++;
			break;
		case 11:
			cmd = "STAT:CELL:REP? (@6001:6032)";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 12:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
				recv_status[actual] = 0;
				report_result += (AnsiString)recv_status;
			}
			else{
				for(int i = 0; i < 62; i += 2){
					recv_status[i] = '0';
					recv_status[i + 1] = ',';
				}
				recv_status[62] = '0';
				recv_status[63] = '\n';
				report_result += (AnsiString)recv_status;
			}
			Timer_GetReport->Interval = timer2;
			nReportStep++;
			break;
		case 13:
			cmd = "STAT:CELL:REP? (@7001:7032)";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 14:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
				recv_status[actual] = 0;
				report_result += (AnsiString)recv_status;
			}
			else{
				for(int i = 0; i < 62; i += 2){
					recv_status[i] = '0';
					recv_status[i + 1] = ',';
				}
				recv_status[62] = '0';
				recv_status[63] = '\n';
				report_result += (AnsiString)recv_status;
			}
			Timer_GetReport->Interval = timer2;
			nReportStep++;
			break;
		case 15:
			cmd = "STAT:CELL:REP? (@8001:8032)";
			status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 16:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
				recv_status[actual] = 0;
				report_result += (AnsiString)recv_status;
			}
			else{
				for(int i = 0; i < 62; i += 2){
					recv_status[i] = '0';
					recv_status[i + 1] = ',';
				}
				recv_status[62] = '0';
				recv_status[63] = '\n';
				report_result += (AnsiString)recv_status;
			}
            bReport = true;
			Timer_GetReport->Interval = timer2;
            Timer_GetReport->Enabled = false;
			nReportStep++;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::Timer_GetInfoTimer(TObject *Sender)
{
	//* Get Current, Voltage, Report
    AnsiString cmd;
	int stage_num = Timer_GetInfo->Tag;

	int timer1 = 10, timer2 = 50, timer3 = 150;

	actual = 0;
	actual2 = 0;
	ViUInt32 retCount; /* Return count from string I/O */
	ViChar buffer[BUFFER_SIZE]; /* Buffer for string I/O */
	SetTimeOut(TIMEOUT_VALUE, stage_num);

	switch(nGetInfoStep)
	{
		case 0:
			KeysCLS(stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 1:
			cmd = "MEAS:CURR? 0000";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer3;
			nGetInfoStep++;
			break;
		case 2:
			InfoResult += ViReadMeas(stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 3:
			cmd = "MEAS:VOLT? 0000";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer3;
			nGetInfoStep++;
			break;
		case 4:
			InfoResult += ViReadMeas(stage_num);
			nGetInfoStep++;
			Timer_GetInfo->Interval = timer2;
			break;
		case 5:
			cmd = "STAT:CELL:REP? (@1001:1032)";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 6:
			InfoResult += ViReadReport(stage_num);
			Timer_GetInfo->Interval = timer1;
			nGetInfoStep++;
			break;
		case 7:
			cmd = "STAT:CELL:REP? (@2001:2032)";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 8:
			InfoResult += ViReadReport(stage_num);
			Timer_GetInfo->Interval = timer1;
			nGetInfoStep++;
			break;
		case 9:
			cmd = "STAT:CELL:REP? (@3001:3032)";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 10:
			InfoResult += ViReadReport(stage_num);
			Timer_GetInfo->Interval = timer1;
			nGetInfoStep++;
			break;
		case 11:
			cmd = "STAT:CELL:REP? (@4001:4032)";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 12:
			InfoResult += ViReadReport(stage_num);
			Timer_GetInfo->Interval = timer1;
			nGetInfoStep++;
			break;
		case 13:
			cmd = "STAT:CELL:REP? (@5001:5032)";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 14:
			InfoResult += ViReadReport(stage_num);
			Timer_GetInfo->Interval = timer1;
			nGetInfoStep++;
			break;
		case 15:
			cmd = "STAT:CELL:REP? (@6001:6032)";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 16:
			InfoResult += ViReadReport(stage_num);
			Timer_GetInfo->Interval = timer1;
			nGetInfoStep++;
			break;
		case 17:
			cmd = "STAT:CELL:REP? (@7001:7032)";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 18:
			InfoResult += ViReadReport(stage_num);
			Timer_GetInfo->Interval = timer1;
			nGetInfoStep++;
			break;
		case 19:
			cmd = "STAT:CELL:REP? (@8001:8032)";
			ViWriteCmd(cmd, stage_num);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 20:
			InfoResult += ViReadReport(stage_num);
			bReport = true;
			Timer_GetInfo->Interval = timer1;
			Timer_GetInfo->Enabled = false;
			nGetInfoStep = 99;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa::ViWriteCmd(AnsiString cmd, int stage_num)
{
	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TMod_Visa::ViReadReport(int stage_num)
{
	UnicodeString sResult = "";

	if(VI_SUCCESS <= status){
		memset(&recv_report, 0, sizeof(recv_report)) ;
		status = viRead(m_vi[stage_num], (ViBuf)recv_report, sizereport, &actual);
		recv_report[actual] = 0;
		sResult = (AnsiString)recv_report;
	}
	else{
		for(int i = 0; i < 62; i += 2){
			recv_report[i] = '0';
			recv_report[i + 1] = ',';
		}
		recv_report[62] = '0';
		recv_report[63] = '\n';
		sResult = (AnsiString)recv_report;
	}

    return sResult;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TMod_Visa::ViReadMeas(int stage_num)
{
	UnicodeString sResult = "";
    if(VI_SUCCESS <= status){
		memset(&recv_values, 0, sizeof(recv_values)) ;
		status = viRead(m_vi[stage_num], (ViBuf)recv_values, sizevalues, &actual);
		recv_values[actual] = 0;
		sResult = (AnsiString)recv_values;
	}
	else{
		for(int i = 0; i < 510; i += 2){
			recv_report[i] = '0';
			recv_report[i + 1] = ',';
		}
		recv_report[510] = '0';
		recv_report[511] = '\n';
		sResult = (AnsiString)recv_values;
	}

    return sResult;
}
//---------------------------------------------------------------------------
