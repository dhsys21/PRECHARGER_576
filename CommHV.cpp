//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TComm_HV *Comm_HV;
//---------------------------------------------------------------------------
__fastcall TComm_HV::TComm_HV(TComponent* Owner)
	: TDataModule(Owner)
{
	m_SendTh = new SendThread_HV(true);
	m_SendTh->m_Sleep = 200;

	m_RecvTh = new RecvThread_HV(true);
	m_RecvTh->m_Sleep = 200;

	m_bConnect = false;
}
//---------------------------------------------------------------------------








//---------------------------------------------------------------------------
//	Thread
//---------------------------------------------------------------------------
void __fastcall SendThread_HV::MainFunc()
{
	if(m_SendMsg.IsEmpty())
	{
		if(!m_dequeMsg.empty())
		{
			m_SendMsg = m_dequeMsg.front();
			m_dequeMsg.pop_front();

			viWrite(Form_Main->m_Comm_HV[m_Tag]->m_vi,
				(ViBuf)(m_SendMsg + (AnsiString)HV_ETX).c_str(), m_SendMsg.Length() + 1, &actual);

			if(m_SendMsg == "*IDN?")
				Form_Main->m_Comm_HV[m_Tag]->m_RecvIndex = HV_RECV_INDEX_INIT;
			else if(m_SendMsg == "STAT:OPER:TEST:COND?")
				Form_Main->m_Comm_HV[m_Tag]->m_RecvIndex = HV_RECV_INDEX_CINDITION;
			else if(m_SendMsg == "RES?")
				Form_Main->m_Comm_HV[m_Tag]->m_RecvIndex = HV_RECV_INDEX_RESULT;
			else if(m_SendMsg == "INIT:SEQ2")
				Form_Main->m_Comm_HV[m_Tag]->Timer_Result->Enabled = true;

			if(m_SendMsg.Pos("?"))
			{
				err = viRead(Form_Main->m_Comm_HV[m_Tag]->m_vi, (ViBuf)recv, 1024, &actual);
				recv[actual] = 0;
            	
				if(err == VI_SUCCESS)
				{
					Form_Main->m_Comm_HV[m_Tag]->m_RecvTh->m_dequeMsg.push_back((AnsiString)recv);
					m_SendMsg = "";
					Form_Main->m_Comm_HV[m_Tag]->m_bConnect = true;
				}
				else
				{
					Initialization();
					Synchronize(&ErrorFunc);
					Form_Main->m_Comm_HV[m_Tag]->m_bConnect = false;
				}
			}
			else m_SendMsg = "";
		}
		else if(Form_Info_HV->m_bAuto) Form_Main->m_Comm_HV[m_Tag]->SendMsg_Condition();
	}
}
//---------------------------------------------------------------------------
void __fastcall SendThread_HV::ErrorFunc()
{
	Form_Error->DisplayErrorMessage("HV #" + IntToStr(m_Tag + 1) + " 통신 에러",
										"HV #" + IntToStr(m_Tag + 1) + " 통신 설정을 확인 해주세요.",
										"USB 케이블 연결 상태를 확인 해주세요.");
    Form_Error->Tag = m_Tag;
}
//---------------------------------------------------------------------------
void __fastcall RecvThread_HV::MainFunc()
{
	if(!m_dequeMsg.empty())
	{
		msg = m_dequeMsg.front();
		m_dequeMsg.pop_front();

		Form_Main->m_Comm_HV[m_Tag]->RecvMsg(msg);
	}
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
//	접속 & 해제
//---------------------------------------------------------------------------
void __fastcall TComm_HV::Connect(AnsiString sirealNumber)
{
	try{
		viOpenDefaultRM(&m_viRM);

							//	제조업체 ID::모델번호::일련번호::USBTMC인터페이스번호		VL000219
		usbID = "USB0::0x0B3E::0x1017::" + sirealNumber + "::0::INSTR";
		if(VI_SUCCESS == viOpen(m_viRM, usbID.c_str(), VI_NULL, VI_NULL, &m_vi))
		{
			m_SendTh->Initialization();
			m_RecvTh->Initialization();

			m_SendTh->Suspended = false;
			m_RecvTh->Suspended = false;

			m_bConnect = true;

			Initialization();
			Form_Configuration->AdvSmoothButton8Click(this);
            Form_Configuration->AdvSmoothButton1Click(this);
		}
		else
		{
			DisConnect_BeforeClose();

			ShowMessage("HV #" + IntToStr(this->Tag + 1) + " : 통신 접속 실패.");
			return;
		}
	}
	catch(...){
		DisConnect_BeforeClose();

		ShowMessage("HV #" + IntToStr(this->Tag + 1) + " : 통신 에러.");

		return;
	}
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::DisConnect_BeforeClose()
{
	m_SendTh->Initialization();
	m_RecvTh->Initialization();

	m_SendTh->Suspended = true;
	m_RecvTh->Suspended = true;

	m_bConnect = false;

	Form_Main->CommunicationStateUpdate(COMMUNICATION_TYPE_HV, this->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::DisConnect()
{
	DisConnect_BeforeClose();

	viClose(m_vi);
	viClose(m_viRM);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::Initialization()
{
	SendMsg_Init();
}
//---------------------------------------------------------------------------






//---------------------------------------------------------------------------
//	발신
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg(AnsiString msg)
{
	m_SendTh->m_dequeMsg.push_back(msg);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_Init()
{
	SendMsg("*IDN?");
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_Local()
{
	SendMsg("SYST:LOC");
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_DefaultState()
{
	SendMsg("*RST");
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_Start()
{
	for(int i = 0; i < 14; i++) readValue[i] = "";
	SendMsg("INIT:SEQ2");
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::Timer_ResultTimer(TObject *Sender)
{
	Timer_Result->Enabled = false;
	SendMsg_Result();
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_Result()
{
	SendMsg("RES?");
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_Aborts()
{
//	SendMsg("TEST:ABOR");
	m_SendTh->m_dequeMsg.push_front("TEST:ABOR");
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_Condition()
{
	SendMsg("STAT:OPER:TEST:COND?");
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_TestModeSetting(AnsiString mode)
{
	SendMsg("SOUR:FUNC:MODE " + mode);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_VoltSetting(AnsiString mode, AnsiString value)
{
	SendMsg("SOUR:" + mode + ":VOLT " + value);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_UpperLimitSetting(AnsiString mode, AnsiString value)
{
	SendMsg("SENS:" + mode + ":JUDG " + value);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_LowerLimitState(AnsiString mode, AnsiString value)
{
	SendMsg("SENS:" + mode + ":JUDG:LOW:STAT " + value);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_LowerLimitSetting(AnsiString mode, AnsiString value)
{
	SendMsg("SENS:" + mode + ":JUDG:LOW " + value);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_BuzzerVolumeSettings(AnsiString value)
{
	SendMsg("SYST:CONF:BEEP:VOL:FAIL " + value);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::SendMsg_TestTimeSetting(AnsiString mode, AnsiString value)
{
	SendMsg("SOUR:" + mode + ":VOLT:TIM " + value);
	Timer_Result->Interval = value.ToDouble() * 1000 + 200;
}
//---------------------------------------------------------------------------






//---------------------------------------------------------------------------
//	수신
//---------------------------------------------------------------------------
void __fastcall TComm_HV::RecvMsg(AnsiString msg)
{
	msg.Delete(msg.Length(), 1);

	if(m_RecvIndex == HV_RECV_INDEX_INIT) ReadMsg_Init(msg);
	else if(m_RecvIndex == HV_RECV_INDEX_CINDITION) ReadMsg_Condition(msg);
	else if(m_RecvIndex == HV_RECV_INDEX_RESULT) ReadMsg_Result(msg);

//	if(m_RecvIndex != HV_RECV_INDEX_CINDITION)
//	{
//		SaveLog((AnsiString)LOG_PATH, Now().FormatString("yyyy-mm-dd hh:nn:ss.zzz "),
//										"HV #" + IntToStr(this->Tag + 1), "Recv>", msg);
//	}
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::ReadMsg_Init(AnsiString msg)
{
	Form_Main->CommunicationStateUpdate(COMMUNICATION_TYPE_HV, this->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::ReadMsg_Condition(AnsiString msg)
{
	m_Condition = msg;
	if(!(msg == "32" || msg == "256"))
	{
    	if(m_SendTh->m_dequeMsg.empty()) SendMsg_Aborts();
	}
}
//---------------------------------------------------------------------------
void __fastcall TComm_HV::ReadMsg_Result(AnsiString msg)
{
	for(int i = 0; i < 13; i++)
	{
		readValue[i] = msg.SubString(1, msg.Pos(",") - 1);
		msg.Delete(1, msg.Pos(","));
	}
	readValue[13] = msg;
}
//---------------------------------------------------------------------------


