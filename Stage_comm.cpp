#include "FormTotal.h"
#include "RVMO_main.h"
#include "Util.h"
//---------------------------------------------------------------------------
void __fastcall TTotalForm::MakeData(int tx_mode, AnsiString cmd, AnsiString param)
{
	q_txMode.push(tx_mode);
	q_cmd.push(cmd.c_str());
	q_param.push(param.c_str());
}
void __fastcall TTotalForm::SendData(AnsiString Cmd, AnsiString Param)
{
	bool sock_con;
	if(sock != NULL) sock_con = true;
	else sock_con = false;

	if(sock_con)
	{
        AnsiString msg = "@" + Cmd + ";";
        sock->SendText(msg);
        WriteCommLog("TX", msg);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdForceStop(int traypos)
{
	// 자동검사 7. 검사종료	- Probe 해제 및 Tray 검사 대기
	DisplayStatus(nEND);
	Panel_State->Caption = " PreCharger Complete ... ";

    //* 각 포지션별 결과 저장
    WriteResultFile(nTrayPos);

    SetPcValue(PC_D_PRE_PROB_OPEN, 1);
    if(nTrayPos == 1){
        SetPcValue(PC_D_PRE_COMPLETE1, 1);
        WritePLCLog("AutoInspection_Measure", "PreCharger Complete ... , COMPLETE1 on, PC_INTERFACE_PROB_OPEN on");
    } else if(nTrayPos == 2){
        SetPcValue(PC_D_PRE_COMPLETE2, 1);
        WritePLCLog("AutoInspection_Measure", "PreCharger Complete ... , COMPLETE2 on, PC_INTERFACE_PROB_OPEN on");
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdForceStop_Original()
{
    CmdStop();
	tray.amf = true;

	PLCStatus = "run AMF. probe open.";
	if(PLCStatus != OldPLCStatus){
		WritePLCLog("CmdForceStop_Original", PLCStatus);
        OldPLCStatus = PLCStatus;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdTrayOut()
{
	AnsiString cell_serial_filename;
    int nCellSerial = 0;
    if(chkBypass->Checked == false){
        //* 2번 모두 측정 한 후에 ok/ng 쓰기
    	BadInfomation();
        nCellSerial = ReadCellSerial();
        if(nCellSerial != (tray.cell_count1 + tray.cell_count2))
        WritePLCLog("CmdTrayOut",
            "Cell Serial Count Error. CellSerial : " + IntToStr(nCellSerial)
            + ", CellCount : " + IntToStr(tray.cell_count1 + tray.cell_count2));
        WriteVoltCurrValue();
		WriteResultFile();
    }

    Sleep(100);
	// 자동검사 9(끝). 트레이 방출
	if(/*NgCount == (tray.cell_count1 + tray.cell_count2) || */
    	NgCount > editNGAlarmCount->Text.ToIntDef(10)){
        Form_Error->Tag = this->Tag;
        Form_Error->DisplayErrorMessage(this->Tag, nNgErr);
	}
	else{
        if(BaseForm->chkTest->Checked == false){
        	SetPcValue(PC_D_PRE_TRAY_OUT, 1);
            WritePLCLog("CmdTrayOut", "IROCV TRAY OUT = 1");
        }
		DisplayStatus(nFinish);
		Panel_State->Caption = " PreCharger Tray Out ... ";
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdTrayOut_Original()
{
	BadInfomation();
	WriteVoltCurrValue();
    WriteResultFile();
}
//--------------------------------------------------------------------------
void __fastcall TTotalForm::CmdManualMod(bool Set)
{
	this->InitTrayStruct(1);
    this->InitTrayStruct(2);

	if(Set){ //* Manual
		DisplayStatus(nManual);
        nSection = STEP_WAIT;
		nStep = 0;
		if(Timer_AutoInspection->Enabled == true)
            Timer_AutoInspection->Enabled = false;
	} else{ //* Auto
		DisplayStatus(nVacancy);
        nSection = STEP_WAIT;
		nStep = 0;
        if(Timer_AutoInspection->Enabled == false)
            Timer_AutoInspection->Enabled = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::AutoTestFinish(int traypos)
{
    tray.ams = false;
	SetResultList(traypos);
    //* 트레이 위치 이동 없을 때
	if(MeasureInfoForm->n_bManualStart == true)
	{
		WriteResultFile(nTrayPos);
		MeasureInfoForm->n_bManualStart = false;
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// 컨트롤러 명령어
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdAutoTest()
{
    tray.ams = true;
	WriteCommLog("CmdAutoTest()", "START CHARGING");
	dt1StartTime = StrToDateTime(Now());

    SendData("ASB");
    LASTCMD = "ASB";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdAutoStop(int traypos)
{
    Sleep(100);

    tray.amf = true;
    tray.ams = false;

    //* 마지막 종료 색상 표시
    DisplayChannelInfo(traypos);
    //* final data 저장.
    AutoTestFinish(traypos);
    CmdForceStop(traypos);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdStop()
{
    SendData("STP");
    LASTCMD = "STP";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdResetTimer()
{
    nRStep = 0;
    Timer_Reset->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdRebootTimer()
{
    nRStep = 0;
    Timer_Reboot->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdReset()
{
    SendData("RST");
    LASTCMD = "RST";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdReboot()
{
    SendData("RBT");
    LASTCMD = "RBT";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdDST()
{
    SendData("DST");
    LASTCMD = "DST";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdEna()
{
    SendData("ENA");
    LASTCMD = "ENA";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdCAL()
{
    SendData("CAL");
    LASTCMD = "CAL";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdSTC()
{
    SendData("STC");
    LASTCMD = "STC";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdReport()
{
    AnsiString CMD = "STAT:CELL:REP? (@1001:1032,2001:2032,3001:3032,4001:4032,5001:5016)";
    CMD = "TRB" + CMD + "\n";
    SendData(CMD);
    LASTCMD = "REP";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdSetStep()
{
    AnsiString CMD = "";
    AnsiString cTime, cCurr, cVolt;

    cTime = config.time;
	cCurr = convertCondition2(config.curr);
	cVolt = convertCondition2(config.volt);
    // "SEQ:TEST:DEF 1,2,2,VOLT_LE,0.1,BEFORE,90,FAIL";  // 100mV
    // "SEQ:TEST:DEF 1,2,1,CURR_LE,0.01,BEFORE,60,FAIL"; // 10mA
    // "SEQ:TEST:DEF 1,1,1,VOLT_GE,2.0,BEFORE,20,NEX";
    CMD = "SEQ:STEP:DEF 1,1,PRECHARGE,20,1.0,2.0\n";
    CMD += "SEQ:TEST:DEF 1,1,1,VOLT_GE,1.2,BEFORE,20,NEXT\n";
    CMD += "SEQ:STEP:DEF 1,2,PRECHARGE2," + cTime + "," + cCurr + "," + cVolt + "\n";
    CMD += "SEQ:TEST:DEF 1,2,1,CURR_LE,0.01,BEFORE,40,FAIL\n";
    CMD += "SEQ:TEST:DEF 1,2,2,VOLT_LE,0.1,BEFORE,40,FAIL\n";
    CMD += "SYST:ERR?";
    CMD = "TRB" + CMD + "\n";
    SendData(CMD);
    LASTCMD = "SET";
}
//---------------------------------------------------------------------------
bool __fastcall TTotalForm::CmdCheckSet()
{
    int time1 = StringToInt(charge[0].time, 0);
    int time2 = StringToInt(charge[1].time, 0);
    double curr1 = charge[0].curr * 1000;
    double curr2 = charge[1].curr * 1000;
    double volt1 = charge[0].volt * 1000;
    double volt2 = charge[1].volt * 1000;
    if(config.time ==  time1 && config.time == time2
        && config.curr == curr1 && config.curr == curr2
        && config.volt == volt1 && config.volt == volt2)
		return true;
    return false;
}

//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdDischargeSetStep()
{
    AnsiString CMD = "";
	AnsiString precharge_time = "20", precharge_curr = "1.0", precharge_volt = "2.0";
    AnsiString cTime, cCurr, cVolt;

    cTime = config.time;
	cCurr = convertCondition2(config.curr);
	cVolt = convertCondition2(config.volt);

    CMD = "SEQ:STEP:DEF 1,1,PRECHARGE," + precharge_time + "," + precharge_curr + "," + precharge_volt + "\n";
    CMD += "SEQ:STEP:DEF 1,2,DISCHARGE," + cTime + "," + cCurr + "," + cVolt + "\n";
    CMD += "SYST:ERR?";
    CMD = "TRB" + CMD + "\n";
    SendData(CMD);
    LASTCMD = "DEF";
}
//---------------------------------------------------------------------------
// 컨트롤러 명령어
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// PLC 명령어
//---------------------------------------------------------------------------
int __fastcall TTotalForm::GetTrayPos()
{
    int value = GetPlcValue(PLC_D_PRE_TRAY_POS);
    return value;
}
double __fastcall TTotalForm::GetPlcValue(int plc_address)
{
    double value = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, plc_address);
    return value;
}
//---------------------------------------------------------------------------
int __fastcall TTotalForm::GetPlcData(int plc_address, int bit_num)
{
    int value = Mod_PLC->GetData(Mod_PLC->plc_Interface_Data, plc_address, bit_num);
    return value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTotalForm::GetPlcValue(int plc_address, int size)
{
    AnsiString value = Mod_PLC->GetString(Mod_PLC->plc_Interface_Data, plc_address, size);
    return value;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetPcValue(int pc_address, int value)
{
    Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  pc_address, value);
}
//---------------------------------------------------------------------------
double __fastcall TTotalForm::GetPcValue(int pc_address)
{
    double value = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, pc_address);
    return value;
}
//---------------------------------------------------------------------------
// PLC 명령어
//---------------------------------------------------------------------------
