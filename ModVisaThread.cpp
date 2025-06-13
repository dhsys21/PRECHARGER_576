//---------------------------------------------------------------------------


#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TMod_Visa_Thread *Mod_Visa_Thread;
//---------------------------------------------------------------------------
__fastcall TMod_Visa_Thread::TMod_Visa_Thread(TComponent* Owner)
	: TDataModule(Owner)
{
//	m_SendTh = new SendThread_VISA(true);
//	m_SendTh->m_Sleep = 200;
//
//	m_RecvTh = new RecvThread_VISA(true);
//	m_RecvTh->m_Sleep = 200;
//
//	m_bConnect = false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	Thread START
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::MainFunc()
{
	if(m_SendMsg.IsEmpty())
	{
		if(!m_dequeMsg.empty())
		{
			m_SendMsg = m_dequeMsg.front();
			m_dequeMsg.pop_front();

//			viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
//				(ViBuf)(m_SendMsg + (AnsiString)HV_ETX).c_str(), m_SendMsg.Length() + 1, &actual);

			if(m_SendMsg == "*IDN?"){
				BaseForm->m_Visa[m_Tag]->m_RecvIndex = RECV_INDEX_IDN;
				SendIDN(m_SendMsg);
			}
			else if(m_SendMsg == "*RST"){
				SendRST(m_SendMsg);
			}
			else if(m_SendMsg == "MEAS:VOLT? 0000"){
				BaseForm->m_Visa[m_Tag]->m_RecvIndex = RECV_INDEX_MEAS;
				SendMEAS();
			}
			else if(m_SendMsg.Pos("STAT:CELL:REP?")){
				BaseForm->m_Visa[m_Tag]->m_RecvIndex = RECV_INDEX_REPORT;
                SendReport();
			}
			else if(m_SendMsg.Pos("STAT:CELL:VERB?")){
				BaseForm->m_Visa[m_Tag]->m_RecvIndex = RECV_INDEX_SEQSTATUS;
                SendSeqStatus();
			}
			else if(m_SendMsg.Pos("CELL:DEF:QUICk")){
				AnsiString cVolt, cCurr, cTime;
				AnsiString msg = m_SendMsg;
				msg = msg.Delete(1, msg.Pos(",")); //.SubString(1, strVolts.Pos(",") - 1).Trim()
				cVolt = msg.SubString(1, msg.Pos(",") - 1).Trim();
				msg = msg.Delete(1, msg.Pos(","));
				cCurr = msg.SubString(1, msg.Pos(",") - 1).Trim();
				msg = msg.Delete(1, msg.Pos(","));
				cTime = msg.Trim();

				SendSetStep2(cVolt, cCurr, cTime);
			}
			else if(m_SendMsg.Pos("SEQ:STEP:DEF")){
				BaseForm->m_Visa[m_Tag]->m_RecvIndex = RECV_INDEX_STEPDEFINITION;
				SendGetStepDefinition(m_SendMsg);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::ErrorFunc()
{
	Form_Error->DisplayErrorMessage("PreCharger #" + IntToStr(m_Tag + 1) + " 통신 에러",
										"PreCharger #" + IntToStr(m_Tag + 1) + " 통신 설정을 확인 해주세요.",
										"LAN 케이블 연결 상태를 확인 해주세요.", m_Tag);
	Form_Error->Tag = m_Tag;
}
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::SendIDN(AnsiString cmd)
{
	// Send Command
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

    // Receive Result
	char recv_msg[91];
	memset(&recv_msg, 0, sizeof(recv_msg)) ;
	status = viRead(BaseForm->m_Visa[m_Tag]->m_vi, (ViBuf)recv_msg, 91, &actual);
	recv_msg[actual] = 0;

	if(status >= VI_SUCCESS)
	{
		BaseForm->m_Visa[m_Tag]->m_RecvTh->m_dequeMsg.push_back((AnsiString)recv_msg);
		m_SendMsg = "";
		BaseForm->m_Visa[m_Tag]->m_bConnect = true;
	}
	else
	{
		BaseForm->m_Visa[m_Tag]->Initialization();
		Synchronize(&ErrorFunc);
		BaseForm->m_Visa[m_Tag]->m_bConnect = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::SendRST(AnsiString cmd)
{
	// Send Command
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
}
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::SendMEAS()
{
	AnsiString cmd, sResult = "";

	actual = 0;
	ViUInt32 retCount; /* Return count from string I/O */
	ViChar buffer[BUFFER_SIZE]; /* Buffer for string I/O */

	char recv_values[4096];
	memset(&recv_values, 0, sizeof(recv_values)) ;
	cmd = "MEAS:VOLT? 0000";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(150);
	status = viRead(BaseForm->m_Visa[m_Tag]->m_vi, (ViBuf)recv_values, 4096, &actual);
	recv_values[actual] = 0;
	sResult = (AnsiString)recv_values;

	memset(&recv_values, 0, sizeof(recv_values)) ;
	cmd = "MEAS:CURR? 0000";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(150);
	status = viRead(BaseForm->m_Visa[m_Tag]->m_vi, (ViBuf)recv_values, 4096, &actual);
	recv_values[actual] = 0;
	sResult += (AnsiString)recv_values;

//	if(status >= VI_SUCCESS)
//	{
		BaseForm->m_Visa[m_Tag]->m_RecvTh->m_dequeMsg.push_back(sResult);

		m_SendMsg = "";
		BaseForm->m_Visa[m_Tag]->m_bConnect = true;
//	}
//	else
//	{
//		BaseForm->m_Visa[m_Tag]->Initialization();
//		Synchronize(&ErrorFunc);
//		BaseForm->m_Visa[m_Tag]->m_bConnect = false;
//	}

}
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::SendReport()
{
	AnsiString cmd, sResult = "";
//	UnicodeString sResult = "";
	char recv_status[64];
	actual = 0;

	for(int i = 1; i < 9; i++)
	{
		//* init value start
		memset(&recv_status, 0, sizeof(recv_status)) ;
		for(int nIndex = 0; nIndex < 62; nIndex += 2){
			recv_status[nIndex] = '0';
			recv_status[nIndex + 1] = ',';
		}
		recv_status[62] = '0';
		recv_status[63] = '\n';
		//* init value end

		cmd = "STAT:CELL:REP? (@" + IntToStr(i) + "001:" + IntToStr(i) + "032)";
		viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

		Sleep(50);
		status = viRead(BaseForm->m_Visa[m_Tag]->m_vi, (ViBuf)recv_status, 64, &actual);
		recv_status[actual] = 0;
		sResult += (AnsiString)recv_status;
	}

	//if(VI_SUCCESS <= status){
			BaseForm->m_Visa[m_Tag]->m_RecvTh->m_dequeMsg.push_back(sResult);
			m_SendMsg = "";
			BaseForm->m_Visa[m_Tag]->m_bConnect = true;
//		}
//		else{
//			BaseForm->m_Visa[m_Tag]->Initialization();
//			Synchronize(&ErrorFunc);
//			BaseForm->m_Visa[m_Tag]->m_bConnect = false;
//		}

}
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::SendSeqStatus()
{
    AnsiString sResult = "-1", cmd;
	char recv_status[1024];
    memset(&recv_status, 0, sizeof(recv_status)) ;

	cmd = "STAT:CELL:VERB? 1001";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(50);
	status = viRead(BaseForm->m_Visa[m_Tag]->m_vi, (ViBuf)recv_status, 1024, &actual);
	recv_status[actual] = 0;
	sResult = (AnsiString)recv_status;

	//if(VI_SUCCESS <= status){
			BaseForm->m_Visa[m_Tag]->m_RecvTh->m_dequeMsg.push_back(sResult);
			m_SendMsg = "";
			BaseForm->m_Visa[m_Tag]->m_bConnect = true;
//		}
//		else{
//			BaseForm->m_Visa[m_Tag]->Initialization();
//			Synchronize(&ErrorFunc);
//			BaseForm->m_Visa[m_Tag]->m_bConnect = false;
//		}
}
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::SendSetStep(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
{
	AnsiString cmd;
	AnsiString precharge_volt, precharge_current;
	int sleep_time = 200;
	if(BaseForm->StringToDouble(cCurr, 0.5) < 1) precharge_current = cCurr;
	else precharge_current = 1.0;

	cmd = "SYST:PROB:LIM 2,0";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(sleep_time);
	cmd = "CELL:DEF:QUICk 1";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(sleep_time);
	cmd = "SEQ:STEP:DEF 1,1,PRECHARGE,30,0.1,1.0";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(sleep_time);
	cmd = "SEQ:STEP:DEF 1,2,CHARGE," + cTime + "," + cCurr + "," + cVolt;
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

//    Sleep(sleep_time);
//	keysight_cmd = "SEQ:TEST:DEF 1,1,1,VOLT_LE,0.1,BEFORE,90,FAIL";
//	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
//				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(sleep_time);
	cmd = "SEQ:TEST:DEF 1,2,1,VOLT_LE,0.1,BEFORE,90,FAIL";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
}
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::SendSetStep2(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
{
	AnsiString cmd;
	AnsiString precharge_volt, precharge_current;
	int sleep_time = 100;
	if(BaseForm->StringToDouble(cCurr, 0.5) < 1) precharge_current = cCurr;
	else precharge_current = 1.0;

    actual = 0;
	ViUInt32 retCount; /* Return count from string I/O */
	ViChar buffer[BUFFER_SIZE]; /* Buffer for string I/O */

	cmd = "SYST:PROB:LIM 2,0";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(sleep_time);
	cmd = "CELL:DEF:QUICk 1";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(sleep_time);
	cmd = "SEQ:STEP:DEF 1,1,PRECHARGE,30,0.1,1.0";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(sleep_time);
	cmd = "SEQ:STEP:DEF 1,2,CHARGE," + cTime + "," + cCurr + "," + cVolt;
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

//    Sleep(sleep_time);
//	keysight_cmd = "SEQ:TEST:DEF 1,1,1,VOLT_LE,0.1,BEFORE,90,FAIL";
//	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
//				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(sleep_time);
	cmd = "SEQ:TEST:DEF 1,2,1,VOLT_LE,0.1,BEFORE,90,FAIL";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
}
//---------------------------------------------------------------------------
void __fastcall SendThread_VISA::SendGetStepDefinition(AnsiString cmd)
{
	AnsiString sResult = "";
	char recv_msg[44];
	memset(&recv_msg, 0, sizeof(recv_msg)) ;

//    cmd = "SEQ:STEP:DEF? 1,1";
	viWrite(BaseForm->m_Visa[m_Tag]->m_vi,
				(ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);

	Sleep(100);
	status = viRead(BaseForm->m_Visa[m_Tag]->m_vi, (ViBuf)recv_msg, 44, &actual);
	recv_msg[actual] = 0;
	sResult = (AnsiString)recv_msg;

	//if(VI_SUCCESS <= status){
			BaseForm->m_Visa[m_Tag]->m_RecvTh->m_dequeMsg.push_back(sResult);
			m_SendMsg = "";
			BaseForm->m_Visa[m_Tag]->m_bConnect = true;
//		}
//		else{
//			BaseForm->m_Visa[m_Tag]->Initialization();
//			Synchronize(&ErrorFunc);
//			BaseForm->m_Visa[m_Tag]->m_bConnect = false;
//		}
}
//---------------------------------------------------------------------------
void __fastcall RecvThread_VISA::MainFunc()
{
	if(!m_dequeMsg.empty())
	{
		msg = m_dequeMsg.front();
		m_dequeMsg.pop_front();

		BaseForm->m_Visa[m_Tag]->RecvMsg(msg);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	Thread END
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	접속 & 해제  START
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::Connect(AnsiString ipaddress, AnsiString port)
{
	try{

		viOpenDefaultRM(&m_viRM);
		HostID = "TCPIP::" + ipaddress + "::" + port + "::SOCKET";
		if(VI_SUCCESS == viOpen(m_viRM, HostID.c_str(), VI_NULL, VI_NULL, &m_vi))
		{
			/* READ 시 데이터가 오기 까지 기다리는 최대 시간*/
			viSetAttribute(m_vi, VI_ATTR_TMO_VALUE, 100);

			/* 종료 문자을 입력 받으면 시간 상관 없이 리턴*/
			viSetAttribute(m_vi, VI_ATTR_TERMCHAR_EN, VI_TRUE);

			actual = 0;
			BaseForm->nForm[this->Tag]->WriteCommLog(KEYSIGHT, "Connected...");

			m_SendTh->Initialization();
			m_RecvTh->Initialization();

			m_SendTh->Suspended = false;
			m_RecvTh->Suspended = false;

			m_bConnect = true;
			Initialization();

			Timer_CheckConnection->Enabled = true;
		}
		else
		{
			BaseForm->nForm[this->Tag]->WriteCommLog(KEYSIGHT, "Connect_Fail...");
			DisConnect_BeforeClose();

			//ShowMessage("HV #" + IntToStr(this->Tag + 1) + " : 통신 접속 실패.");
			return;
		}

	}
	catch(...){
		DisConnect_BeforeClose();

		//ShowMessage("HV #" + IntToStr(this->Tag + 1) + " : 통신 에러.");
		return;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::DisConnect_BeforeClose()
{
//	m_SendTh->Initialization();
//	m_RecvTh->Initialization();
//
//	m_SendTh->Suspended = true;
//	m_RecvTh->Suspended = true;
//
//	m_bConnect = false;

	//Form_Main->CommunicationStateUpdate(COMMUNICATION_TYPE_HV, this->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::DisConnect()
{
	DisConnect_BeforeClose();

	BaseForm->nForm[this->Tag]->WriteCommLog(KEYSIGHT, "DisConnected...");

	viClose(m_vi);
	viClose(m_viRM);

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	접속 & 해제   END
//---------------------------------------------------------------------------



void __fastcall TMod_Visa_Thread::Timer_CheckConnectionTimer(TObject *Sender)
{
//	for(int i=0;i<4;i++)
//	{
//		if(isConnected(i) == false) {
//			DisConnect(i);
//			Sleep(100);
//			Connect(BaseForm->nForm[i]->editKeysightIPAddress->Text, BaseForm->nForm[i]->editKeysightPort->Text,i);
//		}
//	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::Initialization()
{
	SendMsg_IDN();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  송신 명령어 START
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::SendMsg(AnsiString msg)
{
	m_SendTh->m_dequeMsg.push_back(msg);
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::SendMsg_IDN()
{
	SendMsg("*IDN?");
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::SendMsg_SetStep(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
{
	AnsiString msg = "CELL:DEF:QUICk 1," + cVolt + "," + cCurr + "," + cTime;
	SendMsg(msg);
}
void __fastcall TMod_Visa_Thread::SendMsg_GetStepDefinition(AnsiString seq_id, AnsiString step_id)
{
	AnsiString msg = "SEQ:STEP:DEF? " + seq_id + "," + step_id;
	SendMsg(msg);
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::SendMsg_GetMeas()
{
	SendMsg("MEAS:VOLT? 0000");
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::SendMsg_GetReport()
{
	SendMsg("STAT:CELL:REP?");
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::SendMsg_GetSeqStatus()
{
    SendMsg("STAT:CELL:VERB?");
}
void __fastcall TMod_Visa_Thread::SendMsg_StopSequence()
{
	SendMsg("SEQ:ABORT");
//	m_SendTh->m_dequeMsg.push_front("TEST:ABOR");
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::SendMsg_Rst()
{
	SendMsg("*RST");
}

//---------------------------------------------------------------------------
//  송신 명령어 END
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
//  수신 명령어 START
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::RecvMsg(AnsiString msg)
{
	msg.Delete(msg.Length(), 1);

	if(m_RecvIndex == RECV_INDEX_IDN) ReadMsg_IDN(msg);
	else if(m_RecvIndex == RECV_INDEX_MEAS) ReadMsg_MEAS(msg);
	else if(m_RecvIndex == RECV_INDEX_REPORT) ReadMsg_Report(msg);
	else if(m_RecvIndex == RECV_INDEX_SEQSTATUS) ReadMsg_SeqStatus(msg);
	else if(m_RecvIndex == RECV_INDEX_STEPDEFINITION) ReadMsg_GetStepDefinition(msg);
//	else if(m_RecvIndex == HV_RECV_INDEX_CINDITION) ReadMsg_Condition(msg);
//	else if(m_RecvIndex == HV_RECV_INDEX_RESULT) ReadMsg_Result(msg);
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::ReadMsg_IDN(AnsiString msg)
{
	BaseForm->nForm[this->Tag]->WriteCommLog(KEYSIGHT, msg);
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::ReadMsg_MEAS(AnsiString msg)
{
    readMEAS = msg;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::ReadMsg_Report(AnsiString msg)
{
	readReport = msg;
}
//---------------------------------------------------------------------------
void __fastcall TMod_Visa_Thread::ReadMsg_SeqStatus(AnsiString msg)
{
	readSeqStatus = msg;
}
void __fastcall TMod_Visa_Thread::ReadMsg_GetStepDefinition(AnsiString msg)
{
    readStepDefinition = msg;
}
//---------------------------------------------------------------------------
//  수신 명령어 END
//---------------------------------------------------------------------------

//void __fastcall TMod_Visa::KeysCLS()
//{
//	AnsiString keysight_cmd = "*CLS";
//	runCommandWithNoResult(keysight_cmd,stage_num);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMod_Visa::KeysRST()
//{
//	AnsiString keysight_cmd = "*RST";
//	runCommandWithNoResult(keysight_cmd,stage_num);
//}
//
////---------------------------------------------------------------------------
//AnsiString __fastcall TMod_Visa::KeysGetPreChargeDef(AnsiString seq_id, AnsiString step_id)
//{
//	AnsiString cmd;
//	UnicodeString sResult = "-1";
//	char recv_msg[46];
//
//	cmd = "SEQ:STEP:DEF? " + seq_id + "," + step_id;
//	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
//	if(VI_SUCCESS <= status){
//		Sleep(100);
//		memset(&recv_msg, 0, sizeof(recv_msg)) ;
//		status = viRead(m_vi[stage_num], (ViBuf)recv_msg, 46, &actual);
//		recv_msg[actual] = 0;
//		sResult = (AnsiString)recv_msg;
//	}
//	return sResult;
//}
////---------------------------------------------------------------------------
//AnsiString __fastcall TMod_Visa::KeysGetChargeDef(AnsiString seq_id, AnsiString step_id)
//{
//	AnsiString cmd;
//	UnicodeString sResult = "-1";
//	char recv_msg[44];
//
//	cmd = "SEQ:STEP:DEF? " + seq_id + "," + step_id;
//	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
//	if(VI_SUCCESS <= status){
//		Sleep(100);
//		memset(&recv_msg, 0, sizeof(recv_msg)) ;
//		status = viRead(m_vi[stage_num], (ViBuf)recv_msg, 44, &actual);
//		recv_msg[actual] = 0;
//		sResult = (AnsiString)recv_msg;
//	}
//	return sResult;
//}
////---------------------------------------------------------------------------
//void __fastcall TMod_Visa::KeysSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
//{
//	AnsiString keysight_cmd;
//	AnsiString precharge_volt, precharge_current;
//	if(BaseForm->StringToDouble(cCurr, 0.5) < 1) precharge_current = cCurr;
//	else precharge_current = 1.0;
//
//	keysight_cmd = "SYST:PROB:LIM 2,0";
//	runCommandWithNoResult(keysight_cmd,stage_num);
//
//	Sleep(keysighttime);
//	keysight_cmd = "CELL:DEF:QUICk 1";
//	runCommandWithNoResult(keysight_cmd,stage_num);
//
//	Sleep(keysighttime);
//	keysight_cmd = "SEQ:STEP:DEF 1,1,PRECHARGE,30,0.1,1.0";
//	runCommandWithNoResult(keysight_cmd,stage_num);
//
//	Sleep(keysighttime);
//	keysight_cmd = "SEQ:STEP:DEF 1,2,CHARGE," + cTime + "," + cCurr + "," + cVolt;
//	runCommandWithNoResult(keysight_cmd,stage_num);
//
////    Sleep(keysighttime);
////	keysight_cmd = "SEQ:TEST:DEF 1,1,1,VOLT_LE,0.1,BEFORE,90,FAIL";
////	runCommandWithNoResult(keysight_cmd,stage_num);
//
//	Sleep(keysighttime);
//	keysight_cmd = "SEQ:TEST:DEF 1,2,1,VOLT_LE,0.1,BEFORE,90,FAIL";
//	runCommandWithNoResult(keysight_cmd,stage_num);
//}
////---------------------------------------------------------------------------
//void __fastcall TMod_Visa::KeysStartCharge()
//{
//	AnsiString keysight_cmd;
//	int enabletime = 100;
//	int inittime = 500;
//	for(int i = 1; i < 9; i++)
//	{
//		keysight_cmd = "CELL:ENABLE (@" + IntToStr(i) + "001:" + IntToStr(i) + "032),1";
//		runCommandWithNoResult(keysight_cmd,stage_num);
////		status = viWrite(m_vi, (ViBuf)(keysight_cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
//		Sleep(enabletime);
//	}
//
//	for(int i = 1; i < 9; i++)
//	{
//		keysight_cmd = "CELL:INIT (@" + IntToStr(i) + "001:" + IntToStr(i) + "032)";
//		runCommandWithNoResult(keysight_cmd,stage_num);
////        status = viWrite(m_vi, (ViBuf)(keysight_cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
//		Sleep(inittime);
//	}
//}
////---------------------------------------------------------------------------
//void __fastcall TMod_Visa::KeysStopSequence()
//{
//	AnsiString keysight_cmd = "SEQ:ABORT";
//	runCommandWithNoResult(keysight_cmd,stage_num);
//	keysight_cmd = "*CLS";
//	runCommandWithNoResult(keysight_cmd,stage_num);
//}
////---------------------------------------------------------------------------
//AnsiString __fastcall TMod_Visa::getMeasureValue()
//{
//	AnsiString cmd;
//	UnicodeString sResult = "-1";
////	if(nType == 0) cmd = "MEAS:VOLT? 0000";
////	else if(nType == 1) cmd = "MEAS:CURR? 0000";
//	actual = 0;
//	actual2 = 0;
//    ViUInt32 retCount; /* Return count from string I/O */
//	ViChar buffer[BUFFER_SIZE]; /* Buffer for string I/O */
//	SetTimeOut(TIMEOUT_VALUE, stage_num);
//
//	KeysCLS(stage_num);
//    Sleep(50);
//
//	cmd = "MEAS:VOLT? 0000";
//	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
//	if(VI_SUCCESS <= status){
//		Sleep(100);
//		memset(&recv_values, 0, sizeof(recv_values)) ;
//		status = viRead(m_vi[stage_num], (ViBuf)recv_values, 5000, &actual);
//		recv_values[actual] = 0;
//		sResult = (AnsiString)recv_values;
//	}
//
//	cmd = "MEAS:CURR? 0000";
//	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
//	if(VI_SUCCESS <= status){
//		Sleep(100);
//		memset(&recv_values, 0, sizeof(recv_values)) ;
//		status = viRead(m_vi[stage_num], (ViBuf)recv_values, 5000, &actual);
//		recv_values[actual] = 0;
//		sResult += (AnsiString)recv_values;
//	}
//
//	/* viPrintf, viScanf 사용예제
//	AnsiString cmd;
//	UnicodeString sResult;
////	if(nType == 0) cmd = "MEAS:VOLT? 0000";
////	else if(nType == 1) cmd = "MEAS:CURR? 0000";
//
//	ViChar recv_values[2050];
//	status = viPrintf(m_vi, (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
//	if(VI_SUCCESS <= status){
//		Sleep(50);
//		memset(&recv_values, 0, sizeof(recv_values)) ;
//		status = viScanf(m_vi, "%t", &recv_values);
//		sResult = (UnicodeString)recv_values;
//		return sResult;
//	}
//	*/
//	return sResult;
//}
////---------------------------------------------------------------------------
//AnsiString __fastcall TMod_Visa::getReport()
//{
//	AnsiString cmd;
//	UnicodeString sResult = "";
//	Mod_Visa->SetTimeOut(TIMEOUT_VALUE, stage_num);
//
//	KeysCLS(stage_num);
//	Sleep(50);
//
//	for(int i = 1; i < 9; i++)
//	{
//		cmd = "STAT:CELL:REP? (@" + IntToStr(i) + "001:" + IntToStr(i) + "032)";
//		status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
//		if(VI_SUCCESS <= status){
//			Sleep(50);
//			memset(&recv_status, 0, sizeof(recv_status)) ;
//			status = viRead(m_vi[stage_num], (ViBuf)recv_status, 64, &actual);
//			recv_status[actual] = 0;
//			sResult += (AnsiString)recv_status;
//		}
//		else{
//			for(int i = 0; i < 31; i += 2){
//				recv_status[i] = '0';
//				recv_status[i + 1] = ',';
//			}
//			recv_status[62] = '0';
//			recv_status[63] = '\n';
//			sResult += (AnsiString)recv_status;
//		}
//	}
//
//	return sResult;
//}
////---------------------------------------------------------------------------
//
//AnsiString __fastcall TMod_Visa::GetSeqStatus()
//{
//	AnsiString sResult = "-1", cmd;
//
//	cmd = "STAT:CELL:VERB? 1001";
//	status = viWrite(m_vi[stage_num], (ViBuf)(cmd + (AnsiString)HV_ETX).c_str(), cmd.Length() + 1, &actual);
//	if(VI_SUCCESS <= status){
//		Sleep(50);
//		status = viRead(m_vi[stage_num], (ViBuf)recv_status, 1024, &actual);
//		recv_status[actual] = 0;
//		sResult = (AnsiString)recv_status;
//		return sResult;
//	}
//	return sResult;
//}
////---------------------------------------------------------------------------
//void __fastcall TMod_Visa::KeysStartChargeWithSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime)
//{
//	KeysSet(cVolt, cCurr, cTime,stage_num);
//	KeysStartCharge(stage_num);
//}
////---------------------------------------------------------------------------
