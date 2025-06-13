//---------------------------------------------------------------------------

#ifndef ModVisaThreadH
#define ModVisaThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "visa.h"

#include <ExtCtrls.hpp>
#define HV_ETX						"\n"
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------

class SendThread_VISA : public TThread
{
private:
	void __fastcall MainFunc();
	void __fastcall ErrorFunc();
	void __fastcall SendIDN(AnsiString cmd);
	void __fastcall SendMEAS();
	void __fastcall SendReport();
	void __fastcall SendSeqStatus();
	void __fastcall SendSetStep(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
	void __fastcall SendSetStep2(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
	void __fastcall SendGetStepDefinition(AnsiString cmd);
    void __fastcall SendRST(AnsiString cmd);

protected:
	void __fastcall Execute()
	{
		while(!Terminated){
//			Synchronize(&MainFunc);
			MainFunc();
			Sleep(m_Sleep);
		}
	}
public:
	__fastcall SendThread_VISA(bool CreateSuspended) : TThread(CreateSuspended)
	{
		FreeOnTerminate = true;
	}

	void __fastcall Initialization()
	{
		m_dequeMsg.clear();
		m_SendMsg = "";
		m_ReSendCount = 0;
	}

	deque<AnsiString> m_dequeMsg;
	AnsiString m_SendMsg;
	unsigned long actual;
	int m_ReSendCount;

	char recv[1024];
	ViStatus status;

	int m_Tag;
	int m_Sleep;
};


class RecvThread_VISA : public TThread
{
private:
	void __fastcall MainFunc();

protected:
	void __fastcall Execute()
	{
		while(!Terminated){
//			Synchronize(&MainFunc);
			MainFunc();
			Sleep(m_Sleep);
		}
	}
public:
	__fastcall RecvThread_VISA(bool CreateSuspended) : TThread(CreateSuspended)
	{
		FreeOnTerminate = true;
	}

	void __fastcall Initialization()
	{
		m_dequeMsg.clear();
		msg = "";
	}

	deque<AnsiString> m_dequeMsg;
	AnsiString msg;

	int m_Tag;
	int m_Sleep;
};

//---------------------------------------------------------------------------
//	Recv Index 정보
//---------------------------------------------------------------------------
#define RECV_INDEX_IDN							1
#define RECV_INDEX_MEAS							2
#define RECV_INDEX_REPORT						3
#define RECV_INDEX_SEQSTATUS	                4
#define RECV_INDEX_STEPDEFINITION	            5
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class TMod_Visa_Thread : public TDataModule
{
__published:	// IDE-managed Components
	TTimer *Timer_CheckConnection;
	void __fastcall Timer_CheckConnectionTimer(TObject *Sender);
private:	// User declarations
    AnsiString HostID;
	ViSession m_viRM;

	void __fastcall ReadMsg_IDN(AnsiString msg);
	void __fastcall ReadMsg_MEAS(AnsiString msg);
	void __fastcall ReadMsg_Report(AnsiString msg);
	void __fastcall ReadMsg_SeqStatus(AnsiString msg);
    void __fastcall ReadMsg_GetStepDefinition(AnsiString msg);
public:		// User declarations
	__fastcall TMod_Visa_Thread(TComponent* Owner);

	SendThread_VISA *m_SendTh;
	RecvThread_VISA *m_RecvTh;

	ViSession m_vi;
	ViStatus status;

	bool m_bConnect;
	int m_RecvIndex;
	unsigned long actual;

	AnsiString m_Condition;
	AnsiString readMEAS;
	AnsiString readReport;
	AnsiString readSeqStatus;
    AnsiString readStepDefinition;

	char recv_time[5000];

	//* 연결 함수
	void __fastcall Connect(AnsiString ipaddress, AnsiString port);
	void __fastcall DisConnect_BeforeClose();
	void __fastcall DisConnect();
    void __fastcall Initialization();

	//* 송, 수신 함수
	void __fastcall SendMsg(AnsiString msg);
	void __fastcall SendMsg_IDN();
	void __fastcall SendMsg_GetMeas();
	void __fastcall SendMsg_GetReport();
	void __fastcall SendMsg_GetSeqStatus();

	void __fastcall SendMsg_SetStep(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
	void __fastcall SendMsg_GetStepDefinition(AnsiString seq_id, AnsiString step_id);
	void __fastcall SendMsg_StopSequence();
	void __fastcall SendMsg_Rst();

    void __fastcall RecvMsg(AnsiString msg);

//	void __fastcall KeysSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
//	void __fastcall KeysStartCharge();
//	void __fastcall KeysStartChargeWithSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
//	void __fastcall KeysStopSequence();
//
//	void __fastcall KeysCLS();
//	void __fastcall KeysRST();
//	AnsiString __fastcall KeysGetPreChargeDef(AnsiString seq_id, AnsiString step_id);
//	AnsiString __fastcall KeysGetChargeDef(AnsiString seq_id, AnsiString step_id);
//	AnsiString __fastcall getMeasureValue();
//	AnsiString __fastcall getReport();
//	AnsiString __fastcall GetSeqStatus();

};
//---------------------------------------------------------------------------
extern PACKAGE TMod_Visa_Thread *Mod_Visa_Thread;
//---------------------------------------------------------------------------
#endif
