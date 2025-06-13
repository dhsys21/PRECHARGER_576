//---------------------------------------------------------------------------

#ifndef ModKeysightH
#define ModKeysightH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
// Keysight BT2200

//#include "keysight.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TMod_Keysight : public TDataModule
{
__published:	// IDE-managed Components
	TTimer *Timer_AutoConnect;
	void __fastcall Timer_AutoConnectTimer(TObject *Sender);
private:	// User declarations
    AnsiString keysight_cmd;
public:		// User declarations
	__fastcall TMod_Keysight(TComponent* Owner);
	void __fastcall KeysSetVisaAddress(AnsiString ipaddress, AnsiString port);
	int __fastcall KeysConnect(int type);
	AnsiString __fastcall KeysRunCommand(AnsiString cmd);
	UnicodeString __fastcall KeysRunCommand2(AnsiString cmd);
	void __fastcall KeysRunCommandWithNoResult(AnsiString cmd);
	AnsiString __fastcall KeysGetMeasureValue();

	void __fastcall KeysStopSequence();
	void __fastcall KeysRst();
	void __fastcall KeysSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
	void __fastcall KeysStartCharge();
	AnsiString __fastcall KeysGetTime();
    AnsiString __fastcall KeysGetResult();
	AnsiString __fastcall KeysGetStatus();
	void __fastcall KeysStartChargeWithSet(AnsiString cVolt, AnsiString cCurr, AnsiString cTime);
	UnicodeString __fastcall KeysGetDef(AnsiString seq_id, AnsiString step_id);

	bool m_bConnect[2];
	AnsiString keysight_ipaddress, keysight_port;
};
//---------------------------------------------------------------------------
extern PACKAGE TMod_Keysight *Mod_Keysight;
//---------------------------------------------------------------------------
#endif
