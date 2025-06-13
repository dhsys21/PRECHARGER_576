//---------------------------------------------------------------------------

#ifndef ModVisaArrayH
#define ModVisaArrayH
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
class TMod_VisaArray : public TDataModule
{
__published:	// IDE-managed Components
	TTimer *Timer_CheckConnection;
	TTimer *Timer_EnableCharge;
	TTimer *Timer_InitCharge;
	TTimer *Timer_GetMeas;
	TTimer *Timer_GetReport;
	TTimer *Timer_SetCharge;
	TTimer *Timer_GetInfo;
	TTimer *Timer_EnableCharge2;
	TTimer *Timer_InitCharge2;
	TTimer *Timer_CheckConnection2;
	void __fastcall Timer_CheckConnectionTimer(TObject *Sender);
	void __fastcall Timer_EnableChargeTimer(TObject *Sender);
	void __fastcall Timer_InitChargeTimer(TObject *Sender);
	void __fastcall Timer_GetMeasTimer(TObject *Sender);
	void __fastcall Timer_GetReportTimer(TObject *Sender);
	void __fastcall Timer_SetChargeTimer(TObject *Sender);
	void __fastcall Timer_GetInfoTimer(TObject *Sender);
	void __fastcall Timer_EnableCharge2Timer(TObject *Sender);
	void __fastcall Timer_InitCharge2Timer(TObject *Sender);
	void __fastcall Timer_CheckConnection2Timer(TObject *Sender);
private:	// User declarations
    AnsiString usbID;
	ViSession m_viRM;
public:		// User declarations
	__fastcall TMod_VisaArray(TComponent* Owner);

	bool __fastcall Connect(AnsiString ipaddress, AnsiString port, int stage_num, int WaitTime = 1000);
	void __fastcall DisConnect(int stage_num);
	bool __fastcall isConnected(int stage_num);
	AnsiString __fastcall runCommand(AnsiString cmd);
	int __fastcall runCommandWithNoResult(AnsiString cmd);
	void __fastcall KeysSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
	void __fastcall KeysSet2(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
	void __fastcall KeysEnable();
	void __fastcall KeysInit();
	void __fastcall KeysStartCharge();
	void __fastcall KeysStartCharge2();
	void __fastcall KeysStartChargeWithSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
	void __fastcall KeysStopSequence();

	void __fastcall KeysCLS();
	void __fastcall KeysRST();
	AnsiString __fastcall KeysGetPreChargeDef(AnsiString seq_id, AnsiString step_id);
	AnsiString __fastcall KeysGetChargeDef(AnsiString seq_id, AnsiString step_id);
	AnsiString __fastcall getMeasureValue();
	AnsiString __fastcall getReport();
	AnsiString __fastcall GetCellStatus(int nCount);
	AnsiString __fastcall GetSeqStatus();
	AnsiString __fastcall GetFinalResult(int channel);
	void __fastcall SetTimeOut(int nTime);

	AnsiString visa_address;
	ViSession m_vi;
	//ViSession m_vi2;
	unsigned long actual;
	unsigned long actual2;
	//ViPUInt32 actual;
	char recv_values[5000];
	char recv_report[64];
	char recv_status[64];

	char recv_time[5000];
	ViStatus status;
	bool m_bConnect;

    int Alarm_Count;
    int nStage;

	//* Set, Enable, Init Index
    int nConnStep, nConnErrorCount;
	int nSetStep, nEnableStep, nInitStep;
	int nMeasStep, nReportStep;
	int nGetInfoStep;
    AnsiString SetVoltage, SetCurrent, SetTime;

	//* Meas(Current, Voltage) Result, Report Result
	UnicodeString meas_result, report_result;
	AnsiString InfoResult;

	void __fastcall ViWriteCmd(AnsiString cmd);
	UnicodeString __fastcall ViReadReport();
	UnicodeString __fastcall ViReadMeas();

	void __fastcall WriteKeysightLog(AnsiString msg, int stageno);

	bool bMeas;
	bool bReport;
	int sizevalues;
	int sizereport;
};
//---------------------------------------------------------------------------
extern PACKAGE TMod_VisaArray *Mod_VisaArray;
//---------------------------------------------------------------------------
#endif
