//---------------------------------------------------------------------------

#ifndef ModVisaH
#define ModVisaH
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
class TMod_Visa : public TDataModule
{
__published:	// IDE-managed Components
	TTimer *Timer_CheckConnection;
	TTimer *Timer_EnableCharge;
	TTimer *Timer_InitCharge;
	TTimer *Timer_GetMeas;
	TTimer *Timer_GetReport;
	TTimer *Timer_SetCharge;
	TTimer *Timer_GetInfo;
	void __fastcall Timer_CheckConnectionTimer(TObject *Sender);
	void __fastcall Timer_EnableChargeTimer(TObject *Sender);
	void __fastcall Timer_InitChargeTimer(TObject *Sender);
	void __fastcall Timer_GetMeasTimer(TObject *Sender);
	void __fastcall Timer_GetReportTimer(TObject *Sender);
	void __fastcall Timer_SetChargeTimer(TObject *Sender);
	void __fastcall Timer_GetInfoTimer(TObject *Sender);
private:	// User declarations
	AnsiString usbID;
	ViSession m_viRM[4];
//	ViSession m_viRM2;
public:		// User declarations
	__fastcall TMod_Visa(TComponent* Owner);
	void __fastcall Connect(AnsiString ipaddress, AnsiString port, int stage_num, int WaitTime = 200);
	void __fastcall DisConnect(int stage_num);
	bool __fastcall isConnected(int stage_num);
	AnsiString __fastcall runCommand(AnsiString cmd,int stage_num);
	int __fastcall runCommandWithNoResult(AnsiString cmd,int stage_num);
	void __fastcall KeysSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime,int stage_num);
	void __fastcall KeysSet2(AnsiString cVolt, AnsiString cCurr, AnsiString cTime,int stage_num);
	void __fastcall KeysStartCharge(int stage_num);
	void __fastcall KeysStartCharge2(int stage_num);
	void __fastcall KeysStartChargeWithSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime,int stage_num);
	void __fastcall KeysStopSequence(int stage_num);

	void __fastcall KeysCLS(int stage_num);
    void __fastcall KeysRST(int stage_num);
	AnsiString __fastcall KeysGetPreChargeDef(AnsiString seq_id, AnsiString step_id, int stage_num);
	AnsiString __fastcall KeysGetChargeDef(AnsiString seq_id, AnsiString step_id, int stage_num);
	AnsiString __fastcall getMeasureValue(int stage_num);
	AnsiString __fastcall getReport(int stage_num);
	AnsiString __fastcall GetCellStatus(int nCount,int stage_num);
	AnsiString __fastcall GetSeqStatus(int stage_num);
    AnsiString __fastcall GetFinalResult(int stage_num, int channel);
	AnsiString __fastcall GetRunningTime(int nCount, int stage_num);
	AnsiString __fastcall GetRunningTime2(int nCount, int stage_num);
	void __fastcall SetTimeOut(int nTime, int stage_num);

	AnsiString visa_address;
	ViSession m_vi[4];
	//ViSession m_vi2;
	unsigned long actual;
	unsigned long actual2;
	//ViPUInt32 actual;
	char recv_values[5000];
	char recv_report[64];
	char recv_status[64];

	char recv_time[5000];
	ViStatus status;
	bool m_bConnect[4];

	//* Set, Enable, Init Index
	int nSetStep, nEnableStep, nInitStep;
	int nMeasStep, nReportStep;
	int nGetInfoStep;
    AnsiString cVoltage, cCurrent, cTime;

	//* Meas(Current, Voltage) Result, Report Result
	UnicodeString meas_result, report_result;
	UnicodeString InfoResult;

	void __fastcall ViWriteCmd(AnsiString cmd, int stage_num);
	UnicodeString __fastcall ViReadReport(int stage_num);
	UnicodeString __fastcall ViReadMeas(int stage_num);

	bool bMeas;
	bool bReport;
	int sizevalues;
	int sizereport;
};
//---------------------------------------------------------------------------
extern PACKAGE TMod_Visa *Mod_Visa;
//---------------------------------------------------------------------------
#endif
