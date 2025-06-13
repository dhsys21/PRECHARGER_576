//---------------------------------------------------------------------------

#ifndef CommHVH
#define CommHVH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>


#include "visa.h"

#include "Define.h"
#include <ExtCtrls.hpp>

#define HV_ETX						"\n"



class SendThread_HV : public TThread
{
private:
	void __fastcall MainFunc();
	void __fastcall ErrorFunc();
	
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
	__fastcall SendThread_HV(bool CreateSuspended) : TThread(CreateSuspended)
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
	ViStatus err;

	int m_Tag;
	int m_Sleep;
};


class RecvThread_HV : public TThread
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
	__fastcall RecvThread_HV(bool CreateSuspended) : TThread(CreateSuspended)
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
//	Recv Index Á¤º¸
//---------------------------------------------------------------------------
#define HV_RECV_INDEX_INIT					1
#define HV_RECV_INDEX_CINDITION				2
#define HV_RECV_INDEX_RESULT				3
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
class TComm_HV : public TDataModule
{
__published:	// IDE-managed Components
	TTimer *Timer_Result;
	void __fastcall Timer_ResultTimer(TObject *Sender);
private:	// User declarations
	void __fastcall Initialization();

//	void __fastcall SendMsg(AnsiString msg);


	void __fastcall ReadMsg_Init(AnsiString msg);
	void __fastcall ReadMsg_Condition(AnsiString msg);
	void __fastcall ReadMsg_Result(AnsiString msg);
                     
	AnsiString usbID;
	ViSession m_viRM;
public:		// User declarations
	__fastcall TComm_HV(TComponent* Owner);

	void __fastcall SendMsg(AnsiString msg);

	void __fastcall Connect(AnsiString sirealNumber);
	void __fastcall DisConnect_BeforeClose();
	void __fastcall DisConnect();

	void __fastcall SendMsg_Init();
	void __fastcall SendMsg_DefaultState();
	void __fastcall SendMsg_Local();
	void __fastcall SendMsg_Start();
	void __fastcall SendMsg_Result();
	void __fastcall SendMsg_Aborts();
    void __fastcall SendMsg_Condition();
	void __fastcall SendMsg_TestModeSetting(AnsiString mode);
	void __fastcall SendMsg_VoltSetting(AnsiString mode, AnsiString value);
	void __fastcall SendMsg_UpperLimitSetting(AnsiString mode, AnsiString value);
	void __fastcall SendMsg_LowerLimitState(AnsiString mode, AnsiString value);
	void __fastcall SendMsg_LowerLimitSetting(AnsiString mode, AnsiString value);
	void __fastcall SendMsg_BuzzerVolumeSettings(AnsiString value);
	void __fastcall SendMsg_TestTimeSetting(AnsiString mode, AnsiString value);

	void __fastcall RecvMsg(AnsiString msg);

	SendThread_HV *m_SendTh;
	RecvThread_HV *m_RecvTh;

	ViSession m_vi;

	AnsiString m_Condition;
	AnsiString readValue[14];
	bool m_bConnect;

	int m_RecvIndex;
};
//---------------------------------------------------------------------------
extern PACKAGE TComm_HV *Comm_HV;
//---------------------------------------------------------------------------
#endif
