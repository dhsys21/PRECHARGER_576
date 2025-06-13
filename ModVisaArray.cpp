//---------------------------------------------------------------------------


#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TMod_VisaArray *Mod_VisaArray;
//---------------------------------------------------------------------------
__fastcall TMod_VisaArray::TMod_VisaArray(TComponent* Owner)
	: TDataModule(Owner)
{
	m_bConnect = false;
    Alarm_Count = 0;
    bMeas = false;
	sizevalues = 5000;
	sizereport = 64;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	접속 & 해제
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::SetTimeOut(int nTime)
{
	status = viSetAttribute(m_vi, VI_ATTR_TMO_VALUE, nTime);
	if (status < VI_SUCCESS) {
//		m_bConnect = false;
		viStatusDesc(m_vi, status, recv_status);
//		printf("viSetAttribute() error: %s\n", buf);
		viClose(m_vi);
		viClose(m_viRM);
		exit(EXIT_FAILURE);
	}
}
//---------------------------------------------------------------------------
bool __fastcall TMod_VisaArray::Connect(AnsiString ipaddress, AnsiString port, int stage_num, int WaitTime)
{
	try{
        nStage = stage_num;
		viOpenDefaultRM(&m_viRM);
		usbID = "TCPIP::" + ipaddress + "::" + port + "::SOCKET";
		if(VI_SUCCESS == viOpen(m_viRM, usbID.c_str(), VI_NULL, VI_NULL, &m_vi))
		{
			/* READ 시 데이터가 오기 까지 기다리는 최대 시간*/
			viSetAttribute(m_vi, VI_ATTR_TMO_VALUE, WaitTime);

			/* 종료 문자을 입력 받으면 시간 상관 없이 리턴*/
			viSetAttribute(m_vi, VI_ATTR_TERMCHAR_EN, VI_TRUE);

			actual = 0;
			m_bConnect = true;
			BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, "Connected...");

			if(Timer_CheckConnection->Enabled == false)
				Timer_CheckConnection->Enabled = true;

			Alarm_Count = 0;
			return true;
		}
		else
		{
			BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, "Connect_Fail...");
			m_bConnect = false;
			return false;
		}

	}
	catch(...){
		m_bConnect = false;
		return false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::DisConnect(int stage_num)
{
	m_bConnect = false;
	BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, "DisConnected...");

	viClose(m_vi);
	viClose(m_viRM);

}
//---------------------------------------------------------------------------
bool __fastcall TMod_VisaArray::isConnected(int stage_num)
{
	AnsiString cmd, sResult;
	bool bConnected = true;
	char recv_msg[91];
	//cmd = "*IDN?";
	cmd = "SYST:ERR?";
	BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, cmd);

	sResult = "";
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(100);
		memset(&recv_msg, 0, sizeof(recv_msg)) ;
		status = viRead(m_vi, (ViBuf)recv_msg, 91, &actual);
		recv_msg[actual] = 0;
		sResult = (AnsiString)recv_msg;
		BaseForm->nForm[stage_num]->WriteCommLog(KEYSIGHT, sResult);
	}

//	if(sResult.Pos("Keysight") > 0) bConnected = true;
//    else bConnected = false;

	if(sResult != ""){
		bConnected = true;
		Alarm_Count = 0;
	}
	else {
		//bConnected = false;
		Alarm_Count++;
	}

    if(Alarm_Count > 5) bConnected = false;

	return bConnected;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	Run Command
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_VisaArray::runCommand(AnsiString cmd)
{
	AnsiString sResult = "-1";

	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(100);
		status = viRead(m_vi, (ViBuf)recv_status, 4000, &actual);
		recv_status[actual] = 0;
		sResult = (AnsiString)recv_status;
		return sResult;
	}

	return sResult;
}
//---------------------------------------------------------------------------
int __fastcall TMod_VisaArray::runCommandWithNoResult(AnsiString cmd)
{
	int sResult = -1;
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	if(VI_SUCCESS <= status){
		return 1;
	}
	return sResult;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	Commands
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysCLS()
{
	AnsiString keysight_cmd = "*CLS";
	runCommandWithNoResult(keysight_cmd);
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysRST()
{
	AnsiString keysight_cmd = "*RST";
	runCommandWithNoResult(keysight_cmd);
}

//---------------------------------------------------------------------------
AnsiString __fastcall TMod_VisaArray::KeysGetPreChargeDef(AnsiString seq_id, AnsiString step_id)
{
	AnsiString cmd;
	UnicodeString sResult = "-1";
	char recv_msg[46];

	cmd = "SEQ:STEP:DEF? " + seq_id + "," + step_id;
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(100);
		memset(&recv_msg, 0, sizeof(recv_msg)) ;
		status = viRead(m_vi, (ViBuf)recv_msg, 46, &actual);
		recv_msg[actual] = 0;
		sResult = (AnsiString)recv_msg;
	}
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_VisaArray::KeysGetChargeDef(AnsiString seq_id, AnsiString step_id)
{
	AnsiString cmd;
	UnicodeString sResult = "-1";
	char recv_msg[44];

	cmd = "SEQ:STEP:DEF? " + seq_id + "," + step_id;
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(100);
		memset(&recv_msg, 0, sizeof(recv_msg)) ;
		status = viRead(m_vi, (ViBuf)recv_msg, 44, &actual);
		recv_msg[actual] = 0;
		sResult = (AnsiString)recv_msg;
	}
	return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
{
	AnsiString keysight_cmd;
	AnsiString precharge_volt, precharge_current;
	if(BaseForm->StringToDouble(cCurr, 0.5) < 1) precharge_current = cCurr;
	else precharge_current = 1.0;

	keysight_cmd = "SYST:PROB:LIM 2,0";
	runCommandWithNoResult(keysight_cmd);

	Sleep(keysighttime);
	keysight_cmd = "CELL:DEF:QUICk 1";
	runCommandWithNoResult(keysight_cmd);

	Sleep(keysighttime);
	keysight_cmd = "SEQ:STEP:DEF 1,1,PRECHARGE,20,1,1.0";
	runCommandWithNoResult(keysight_cmd);

	Sleep(keysighttime);
	keysight_cmd = "SEQ:STEP:DEF 1,2,CHARGE," + cTime + "," + cCurr + "," + cVolt;
	runCommandWithNoResult(keysight_cmd);

	Sleep(keysighttime);
	keysight_cmd = "SEQ:TEST:DEF 1,2,1,CURR_LE,0.01,BEFORE,60,FAIL";
	runCommandWithNoResult(keysight_cmd);

    Sleep(keysighttime);
	keysight_cmd = "SEQ:TEST:DEF 1,2,2,VOLT_LE,0.1,BEFORE,60,FAIL";
	runCommandWithNoResult(keysight_cmd);
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysEnable()
{
	AnsiString keysight_cmd;

	keysight_cmd = "CELL:ENABLE (@1001:1032,2001:2032,3001:3032,4001:4032,5001:5032,6001:6032,7001:7032,8001:8032),1";
	runCommandWithNoResult(keysight_cmd);
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysInit()
{
	AnsiString keysight_cmd;
	keysight_cmd = "CELL:INIT (@1001:1032,2001:2032,3001:3032,4001:4032,5001:5032,6001:6032,7001:7032,8001:8032)";
	runCommandWithNoResult(keysight_cmd);
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysSet2(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
{
	SetVoltage = cVolt;
	SetCurrent = cCurr;
	SetTime = cTime;
   //	Timer_SetCharge->Tag = stage_num;

    nSetStep = 0;
    Timer_SetCharge->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysStartCharge()
{
	AnsiString keysight_cmd;
	int enabletime = 100;
	int inittime = 500;
	for(int i = 1; i < 9; i++)
	{
		keysight_cmd = "CELL:ENABLE (@" + IntToStr(i) + "001:" + IntToStr(i) + "032),1";
		runCommandWithNoResult(keysight_cmd);
		Sleep(enabletime);
	}

	for(int i = 1; i < 9; i++)
	{
		keysight_cmd = "CELL:INIT (@" + IntToStr(i) + "001:" + IntToStr(i) + "032)";
		runCommandWithNoResult(keysight_cmd);
		if(i % 2 == 0)
			Sleep(inittime);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysStartCharge2()
{
	nEnableStep = 0;
	//* 8번 실행
	Timer_EnableCharge->Enabled = true;
	//* 1번 실행
	//Timer_EnableCharge2->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysStopSequence()
{
	AnsiString keysight_cmd = "SEQ:ABORT";
	runCommandWithNoResult(keysight_cmd);
	keysight_cmd = "*CLS";
	runCommandWithNoResult(keysight_cmd);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_VisaArray::getMeasureValue()
{
	AnsiString cmd;
	UnicodeString sResult, sResult1 = "-1", sResult2 = "-1";

	actual = 0;
	actual2 = 0;
    ViUInt32 retCount; /* Return count from string I/O */
	ViChar buffer[BUFFER_SIZE]; /* Buffer for string I/O */
	SetTimeOut(TIMEOUT_VALUE);

	KeysCLS();
	Sleep(100);

	cmd = "MEAS:CURR? 0000";
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
        Sleep(150);
		memset(&recv_values, 0, sizeof(recv_values)) ;
		status = viRead(m_vi, (ViBuf)recv_values, 5000, &actual);
		recv_values[actual] = 0;
		sResult1 = (AnsiString)recv_values;
	}

	cmd = "MEAS:VOLT? 0000";
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(150);
		memset(&recv_values, 0, sizeof(recv_values)) ;
		status = viRead(m_vi, (ViBuf)recv_values, 5000, &actual);
		recv_values[actual] = 0;
		sResult2 = (AnsiString)recv_values;
	}
    sResult = sResult2 + sResult1;

	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_VisaArray::getReport()
{
	AnsiString cmd;
	UnicodeString sResult = "";
	SetTimeOut(TIMEOUT_VALUE);

	KeysCLS();
	Sleep(50);

	cmd = "STAT:CELL:REP? (@1001:1032,2001:2032,3001:3032,4001:4032,5001:5032,6001:6032,7001:7016)";
    status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	if(VI_SUCCESS <= status){
		Sleep(100);
		memset(&recv_status, 0, sizeof(recv_status)) ;
		status = viRead(m_vi, (ViBuf)recv_status, 416, &actual);
		recv_status[actual] = 0;
		sResult += (AnsiString)recv_status;
	}
	else{
		for(int i = 0; i < 414; i += 2){
			recv_status[i] = '0';
			recv_status[i + 1] = ',';
		}
		recv_status[414] = '0';
		recv_status[415] = '\n';
		sResult += (AnsiString)recv_status;
	}
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_VisaArray::GetCellStatus(int nCount)
{
	AnsiString sResult = "-1", cmd;
	int nIndex = nCount - 1;

	cmd = "STAT:CELL:REP? (@" + IntToStr(nCount) + "001:" + IntToStr(nCount) + "032)";
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi, (ViBuf)recv_status, 1024, &actual);
		recv_status[actual] = 0;
		sResult = (AnsiString)recv_status;
		return sResult;
	}
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_VisaArray::GetSeqStatus()
{
	AnsiString sResult = "-1", cmd;

	cmd = "STAT:CELL:VERB? 1001";
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(50);
		status = viRead(m_vi, (ViBuf)recv_status, 1024, &actual);
		recv_status[actual] = 0;
		sResult = (AnsiString)recv_status;
		return sResult;
	}
	return sResult;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_VisaArray::GetFinalResult(int channel)
{
	AnsiString sResult = "-1", cmd;

	//cmd = "STAT:CELL:VERB? 1001";
    cmd = "STAT:CELL:VERB? " + IntToStr(channel / 32 + 1) + "0" + FormatFloat("00", (channel % 32 + 1));
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
	if(VI_SUCCESS <= status){
		Sleep(10);
		status = viRead(m_vi, (ViBuf)recv_status, 1024, &actual);
		recv_status[actual] = 0;
		sResult = (AnsiString)recv_status;
		return sResult;
	}
	return sResult;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	Timer
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::KeysStartChargeWithSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
{
	KeysSet(cVolt, cCurr, cTime);
	KeysStartCharge();
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::Timer_CheckConnectionTimer(TObject *Sender)
{
	switch(nConnStep)
	{
		case 0:
			KeysCLS();
			nConnStep = 1;
			break;
		case 1:
			if(isConnected(nStage) == false){
				nConnStep = 2;
                nConnErrorCount++;
			}
			else {
				nConnStep = 0;
				nConnErrorCount = 0;
			}
			break;
		case 2:
			if(nConnErrorCount < 3)
				nConnStep = 0;
			else
				nConnStep = 3;
			break;
		case 3:
            nConnErrorCount++;
			DisConnect(nStage);
			nConnStep = 4;
			break;
		case 4:
			if(Connect(BaseForm->nForm[nStage]->editKeysightIPAddress->Text, BaseForm->nForm[nStage]->editKeysightPort->Text,nStage, 500) == false)
				nConnStep = 3;
			else
				nConnStep = 0;
			break;
		default:
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::Timer_CheckConnection2Timer(TObject *Sender)
{
    if(isConnected(nStage) == false) {
		DisConnect(nStage);
		Sleep(100);
		Connect(BaseForm->nForm[nStage]->editKeysightIPAddress->Text, BaseForm->nForm[nStage]->editKeysightPort->Text,nStage, 500);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::Timer_SetChargeTimer(TObject *Sender)
{
	// Set Charge
	AnsiString keysight_cmd;
	AnsiString precharge_volt, precharge_current;
	if(BaseForm->StringToDouble(SetCurrent, 0.5) < 1) precharge_current = SetCurrent;
	else precharge_current = 1.0;

	switch(nSetStep)
	{
		case 0:
			keysight_cmd = "SYST:PROB:LIM 5,0";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nSetStep++;
			break;
		case 1:
			keysight_cmd = "CELL:DEF:QUICk 1";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nSetStep++;
			break;
		case 2:
			keysight_cmd = "SEQ:STEP:DEF 1,1,PRECHARGE,20,1,2.0";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nSetStep++;
			break;
		case 3:
			keysight_cmd = "SEQ:STEP:DEF 1,2,CHARGE," + SetTime + "," + SetCurrent + "," + SetVoltage;
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nSetStep++;
			break;
//		case 4:
//			keysight_cmd = "SEQ:TEST:DEF 1,2,1,CURR_LE,0.01,BEFORE,60,FAIL";
//			runCommandWithNoResult(keysight_cmd);
//			WriteKeysightLog(keysight_cmd, nStage);
//			nSetStep++;
//			break;
		case 4:
			keysight_cmd = "SEQ:TEST:DEF 1,2,1,VOLT_LE,0.1,BEFORE," + SetTime + ",FAIL";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nSetStep++;
			break;
		case 5:
			nSetStep = 99;
			Timer_SetCharge->Enabled = false;
			BaseForm->nForm[nStage]->nStep = 1;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::Timer_EnableChargeTimer(TObject *Sender)
{
	//* Enable
	AnsiString keysight_cmd;

	switch(nEnableStep)
	{
		case 0:
			keysight_cmd = "CELL:ENABLE (@1001:1032),1";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nEnableStep++;
			break;
		case 1:
			keysight_cmd = "CELL:ENABLE (@2001:2032),1";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nEnableStep++;
			break;
		case 2:
			keysight_cmd = "CELL:ENABLE (@3001:3032),1";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nEnableStep++;
			break;
		case 3:
			keysight_cmd = "CELL:ENABLE (@4001:4032),1";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nEnableStep++;
			break;
		case 4:
			keysight_cmd = "CELL:ENABLE (@5001:5032),1";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nEnableStep++;
			break;
		case 5:
			keysight_cmd = "CELL:ENABLE (@6001:6032),1";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nEnableStep++;
			break;
		case 6:
			keysight_cmd = "CELL:ENABLE (@7001:7032),1";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nEnableStep++;
			break;
		case 7:
			keysight_cmd = "CELL:ENABLE (@8001:8032),1";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
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
void __fastcall TMod_VisaArray::Timer_InitChargeTimer(TObject *Sender)
{
	//* Init
	AnsiString keysight_cmd;

	switch(nInitStep)
	{
		case 0:
			keysight_cmd = "CELL:INIT (@1001:1032)";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nInitStep++;
			break;
		case 1:
			keysight_cmd = "CELL:INIT (@2001:2032)";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nInitStep++;
			break;
		case 2:
			keysight_cmd = "CELL:INIT (@3001:3032)";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nInitStep++;
			break;
		case 3:
			keysight_cmd = "CELL:INIT (@4001:4032)";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nInitStep++;
			break;
		case 4:
			keysight_cmd = "CELL:INIT (@5001:5032)";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nInitStep++;
			break;
		case 5:
			keysight_cmd = "CELL:INIT (@6001:6032)";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nInitStep++;
			break;
		case 6:
			keysight_cmd = "CELL:INIT (@7001:7032)";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nInitStep++;
			break;
		case 7:
			keysight_cmd = "CELL:INIT (@8001:8032)";
			runCommandWithNoResult(keysight_cmd);
			WriteKeysightLog(keysight_cmd, nStage);
			nInitStep++;
			break;
		case 8:
            nInitStep = 99;
			Timer_InitCharge->Enabled = false;
			BaseForm->nForm[this->Tag]->Timer_KeysightGetInfo->Enabled = true;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::Timer_GetMeasTimer(TObject *Sender)
{
	//* Get Voltage, Current
    AnsiString cmd;
	UnicodeString sResult1 = "-1", sResult2 = "-1";

	actual = 0;
	actual2 = 0;
	ViUInt32 retCount; /* Return count from string I/O */
	ViChar buffer[BUFFER_SIZE]; /* Buffer for string I/O */
	SetTimeOut(TIMEOUT_VALUE);

	switch(nMeasStep)
	{
		case 0:
			KeysCLS();
			Timer_GetMeas->Interval = 50;
			nMeasStep++;
			break;
		case 1:
			cmd = "MEAS:CURR? 0000";
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetMeas->Interval = 150;
			nMeasStep++;
			break;
		case 2:
			if(VI_SUCCESS <= status){
				memset(&recv_values, 0, sizeof(recv_values)) ;
				status = viRead(m_vi, (ViBuf)recv_values, 5000, &actual);
				recv_values[actual] = 0;
				sResult1 = (AnsiString)recv_values;
			}
			Timer_GetMeas->Interval = 50;
			nMeasStep++;
			break;
		case 3:
			cmd = "MEAS:VOLT? 0000";
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetMeas->Interval = 150;
			nMeasStep++;
			break;
		case 4:
			if(VI_SUCCESS <= status){
				memset(&recv_values, 0, sizeof(recv_values)) ;
				status = viRead(m_vi, (ViBuf)recv_values, 5000, &actual);
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
void __fastcall TMod_VisaArray::Timer_GetReportTimer(TObject *Sender)
{
	//* Get Report 0, 1, 2, 3, 4
    AnsiString cmd;

	SetTimeOut(TIMEOUT_VALUE);
	int timer1 = 50, timer2 = 10;
	switch(nReportStep)
	{
		case 0:
			KeysCLS();
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 1:
			cmd = "STAT:CELL:REP? (@1001:1032)";
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 2:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi, (ViBuf)recv_status, 64, &actual);
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
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 4:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi, (ViBuf)recv_status, 64, &actual);
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
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 6:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi, (ViBuf)recv_status, 64, &actual);
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
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 8:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi, (ViBuf)recv_status, 64, &actual);
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
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 10:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi, (ViBuf)recv_status, 64, &actual);
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
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 12:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi, (ViBuf)recv_status, 64, &actual);
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
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 14:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi, (ViBuf)recv_status, 64, &actual);
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
			status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
			Timer_GetReport->Interval = timer1;
			nReportStep++;
			break;
		case 16:
			if(VI_SUCCESS <= status){
				memset(&recv_status, 0, sizeof(recv_status)) ;
				status = viRead(m_vi, (ViBuf)recv_status, 64, &actual);
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
void __fastcall TMod_VisaArray::Timer_GetInfoTimer(TObject *Sender)
{
	//* Get Current, Voltage, Report
	AnsiString cmd;

	int timer1 = 100, timer2 = 100, timer3 = 200;

	actual = 0;
	actual2 = 0;
	ViUInt32 retCount; /* Return count from string I/O */
	ViChar buffer[BUFFER_SIZE]; /* Buffer for string I/O */
	SetTimeOut(TIMEOUT_VALUE);

	switch(nGetInfoStep)
	{
		case 0:
			if(Timer_CheckConnection->Enabled == true)
				Timer_CheckConnection->Enabled = false;

			KeysCLS();
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 1:
			cmd = "MEAS:VOLT? 0000";
			ViWriteCmd(cmd);
			Timer_GetInfo->Interval = timer3;
			nGetInfoStep++;
			break;
		case 2:
			InfoResult += ViReadMeas();
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 3:
			cmd = "MEAS:CURR? 0000";
			ViWriteCmd(cmd);
			Timer_GetInfo->Interval = timer3;
			nGetInfoStep++;
			break;
		case 4:
			InfoResult += ViReadMeas();
			nGetInfoStep++;
			Timer_GetInfo->Interval = timer2;
			break;
		case 5:
			cmd = "STAT:CELL:REP? (@1001:1032,2001:2032,3001:3032,4001:4032,5001:5032,6001:6032,7001:7016)";
			ViWriteCmd(cmd);
			Timer_GetInfo->Interval = timer2;
			nGetInfoStep++;
			break;
		case 6:
			InfoResult += ViReadReport();
			Timer_GetInfo->Interval = timer1;
			nGetInfoStep++;
			break;
		case 7:
			InfoResult += ViReadReport();
			bMeas = true;
			Timer_GetInfo->Interval = timer1;
			Timer_GetInfo->Enabled = false;
			nGetInfoStep = 99;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::ViWriteCmd(AnsiString cmd)
{
	status = viWrite(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TMod_VisaArray::ViReadReport()
{
	UnicodeString sResult = "";

	if(VI_SUCCESS <= status){
		memset(&recv_report, 0, sizeof(recv_report)) ;
		status = viRead(m_vi, (ViBuf)recv_report, 416, &actual);
		recv_report[actual] = 0;
		sResult = (AnsiString)recv_report;
	}
	else{
		for(int i = 0; i < 414; i += 2){
			recv_report[i] = '9';
			recv_report[i + 1] = ',';
		}
		recv_report[414] = '9';
		recv_report[415] = '\n';
		sResult = (AnsiString)recv_report;
	}

    return sResult;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TMod_VisaArray::ViReadMeas()
{
	UnicodeString sResult = "";
    if(VI_SUCCESS <= status){
		memset(&recv_values, 0, sizeof(recv_values)) ;
		status = viRead(m_vi, (ViBuf)recv_values, sizevalues, &actual);
		recv_values[actual] = 0;
		sResult = (AnsiString)recv_values;
	}
	else{
		for(int i = 0; i < 510; i += 2){
			recv_values[i] = '99';
			recv_values[i + 1] = ',';
		}
		recv_values[510] = '99';
		recv_values[511] = '\n';
		sResult = (AnsiString)recv_values;
	}

    return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::Timer_EnableCharge2Timer(TObject *Sender)
{
// (@1001:8032)
//* Enable
	AnsiString keysight_cmd;

	switch(nEnableStep)
	{
		case 0:
			keysight_cmd = "CELL:ENABLE (@1001:8032),1";
			runCommandWithNoResult(keysight_cmd);
			nEnableStep++;
			break;
		case 1:
			nEnableStep = 99;
			nInitStep = 0;
			Timer_EnableCharge2->Enabled = false;
            Timer_InitCharge2->Enabled = true;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::Timer_InitCharge2Timer(TObject *Sender)
{
// (@1001:8032)
//* Init
	AnsiString keysight_cmd;

    switch(nInitStep)
	{
		case 0:
			keysight_cmd = "CELL:INIT (@1001:8032)";
			runCommandWithNoResult(keysight_cmd);
		case 4:
			nInitStep = 99;
			Timer_InitCharge2->Enabled = false;
			BaseForm->nForm[this->Tag]->Timer_KeysightGetInfo->Enabled = true;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_VisaArray::WriteKeysightLog(AnsiString msg, int stageno)
{
	BaseForm->nForm[stageno]->WriteKeysightLog(msg);
}
//---------------------------------------------------------------------------
