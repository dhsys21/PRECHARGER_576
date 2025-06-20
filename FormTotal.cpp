//---------------------------------------------------------------------------

#include <vcl.h>
#include <DateUtils.hpp>
#pragma hdrstop

#include "FormTotal.h"
#include "RVMO_main.h"
#include "Util.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvSmoothButton"
#pragma link "AdvSmoothPanel"
#pragma resource "*.dfm"

TTotalForm *TotalForm;

//---------------------------------------------------------------------------
__fastcall TTotalForm::TTotalForm(TComponent* Owner)
	: TForm(Owner)
{
	sock = NULL;

	CurrentGrp = GrpMain;
	bconfig = false;
	stageno = -1;

	clNoCell = cl_no->Color;
	clLine = cl_line->Color;

	this->Width = pback->Width * 2;
	this->Height = pback->Height;

    ReadchannelMapping();
	MakePanel(BaseForm->lblLineNo->Caption);

	curr_min = StringToDouble(editCurrMin->Text, 50);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::FormShow(TObject *Sender)
{
	ReadRemeasureInfo();

	stage.init = true;
    stage.arl = nAuto;

	ReadSystemInfo();
	Initialization();

	Timer_PLCConnect->Enabled = true;
	//btnConnectPRECHARGERClick(this);
	config.recontact = true;
	lblTitle->Caption = "STAGE" + IntToStr(this->Tag+1);

	OldPLCStatus = "";
	PLCStatus = "";
    OldErrorCheckStatus = "";
	ErrorCheckStatus = "";
	OldPreChargerStatus = "";
	PreChargerStatus = "";

	pProcess[0] = pReady;
	pProcess[1] = pTrayIn;
	pProcess[2] = pBarcode;
	pProcess[3] = pProbeDown;
	pProcess[4] = pCharge;
	pProcess[5] = pFinish;
	pProcess[6] = pProbeDown;
	pProcess[7] = pTrayOut;

	NgAlarmCount = 0;

	btnMeasureInfoClick(this);
}
//---------------------------------------------------------------------------
// 구조체 초기화 : 트레이 정보, 재측정 정보
//---------------------------------------------------------------------------
void __fastcall TTotalForm::InitData(int traypos)
{
    int channel;
    //* real_data(REAL_TIME) 초기화 시작
    real_data.stage_status = 0;
    real_data.step_index = "";
    real_data.bBT1 = false;
    real_data.bBT2 = false;
    real_data.step_time = 0;
    real_data.test_time = 0;
    for(int i = 0; i < CHANNELCOUNT; i++){
        channel = GetChannel(chMap, traypos, i) - 1;

        real_data.volt[channel] = 0.0;
        real_data.final_volt[channel] = 0.0;

        real_data.curr[channel] = 0.0;
        real_data.final_curr[channel] = 0.0;

        real_data.capa[channel] = 0.0;
        real_data.final_capa[channel] = 0.0;

        real_data.status[channel] = 0;
        real_data.final_status[channel] = 0;

        real_data.result[channel] = "";
        real_data.final_result[channel] = "";
    }
    //* real_data(REAL_TIME) 초기화 종료

    //* tray(TRAY_INFO) 초기화 시작
    tray.rst = false;
    tray.set = false;
    tray.ams = false;
    tray.amf = false;
    tray.trayin = false;
    tray.channel_charging = false;
    tray.end_charging = false;
    tray.trayid = "";
    tray.mdl_name = "";
    tray.batch_id = "";
    tray.lot_id = "";
    tray.cell_model = "";
    tray.lot_number = "";
    tray.cell_count = 0;
    for(int i = 0; i < CHANNELCOUNT; i++){
        channel = GetChannel(chMap, traypos, i) - 1;
        tray.cell[channel] = 0;
        tray.measure_result[channel] = 0;
        tray.error_time_count[channel] = 0;
        tray.cell_data[channel] = "";
        tray.cell_serial[channel] = "";
    }
    //* tray(TRAY_INFO) 초기화 종료

    //* charge(CHARGE_CONFIG) 초기화  - BT1, BT2 2개의 셋팅
    charge[0].volt = 0.0;
    charge[0].curr = 0.0;
    charge[0].time = 0;
    charge[0].failvolt = 0.0;

    charge[1].volt = 0.0;
    charge[1].curr = 0.0;
    charge[1].time = 0;
    charge[1].failvolt = 0.0;
    //* charge(CHARGE_CONFIG) 초기화
}
void __fastcall TTotalForm::InitTrayStruct(int traypos)
{
	this->WriteRemeasureInfo();
    InitData(traypos);

    int channel;
	for(int i=0; i < CHANNELCOUNT; ++i){
        channel = GetChannel(chMap, traypos, i) - 1;

		panel[channel]->Caption = "";
		panel[channel]->Color = cl_line->Color;
	}

    if(traypos == 1) MeasureInfoForm->btnInit1Click(this);
    else if(traypos == 2) MeasureInfoForm->btnInit2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Initialization()
{
    Initialization(1);
    Initialization(2);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Initialization(int traypos)
{
    int channel;
	for(int i = 0; i < CHANNELCOUNT; i++)
	{
        channel = GetChannel(chMap, traypos, i) - 1;

        m_sTempVlot[channel] = channel + 1;
        m_sTempCurr[channel] = IntToStr(GetChPosF(chReverseMap, channel))	+ "-"
        	+ IntToStr(GetChPosR(chReverseMap, channel));

		m_sTempVlot_Value[channel] = 0;
		m_sTempCurr_Value[channel] = 0;

        real_data.result[channel] = "";
        real_data.final_result[channel] = "";
		real_data.status[channel] = 0;
	}

	dt1StartTime = StrToDateTime(Now().FormatString("yyyy/mm/dd hh:nn:ss"));
	dt2ChargingTime = StrToDateTime(Now().FormatString("yyyy/mm/dd hh:nn:ss"));
    testTime->Caption = "0";

	PLCInitialization();
	this->InitTrayStruct(traypos);

	nSection = STEP_WAIT;
	nStep = 0;

    bStatus = false;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::PLCInitialization()
{
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_TRAY_OUT, 0);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_OPEN, 0);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_CLOSE, 0);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CHARGING, 0);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_ERROR, 0);

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Curr_Data, PC_D_PRE_NG_COUNT, 0);

	Mod_PLC->PLC_Write_Result = true;
	for(int i = 0; i < LINECOUNT; i++)
	{
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + (i * 2), 0);
	}

	for(int i = 0; i < MAXCHANNEL; i++)
	{
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_VOLTAGE_VALUE + i, 0);
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Curr_Data, PC_D_PRE_CURRENT_VALUE + i, 0);

	}

	WriteMINMAX(Tag);

}
//---------------------------------------------------------------------------
// 측정정보보기 버튼
void __fastcall TTotalForm::InitMeasureForm()
{
    memset(&real_data, 0, sizeof(real_data));

	MeasureInfoForm->stage = this->Tag;
	MeasureInfoForm->Tag = this->Tag;
	MeasureInfoForm->pstage->Caption = lblTitle->Caption;
	MeasureInfoForm->BringToFront();
	MeasureInfoForm->Visible = true;
	DisplayChannelInfo(1);
    DisplayChannelInfo(2);
}
//---------------------------------------------------------------------------
// 메인화면 패널 만들기
// 1,2 라인 / 3,4 라인 다름.
void __fastcall TTotalForm::MakePanel(AnsiString type)
{
	int nx, ny, nw, nh;
    nh = (pBase->Height - 25) / LINECOUNT;
    nw = (pBase->Width - 25) / LINECOUNT;

	if(type == "3") //* 왼쪽 위가 1번, 오른쪽 방향으로 1 -> 24
	{
		nx = 2;
		ny = 1;

		for(int index = 0; index < MAXCHANNEL;){
			panel[index] = new TPanel(this);
			panel[index]->Parent = pBase;
			panel[index]->Left =  nx;
			panel[index]->Top = ny;
			panel[index]->Width = nw;
			panel[index]->Height = nh;

			panel[index]->Color = pnormal1->Color;
            panel[index]->ParentBackground = false;

			panel[index]->BevelInner = bvNone;
			panel[index]->BevelKind = bkNone;
			panel[index]->BevelOuter = bvNone;
			panel[index]->Tag = index;
//			panel[index]->Caption = index;
//			panel[index]->Hint = IntToStr(index+1) + " (" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
			panel[index]->ShowHint = true;

			index += 1;
			nx = nx + nw + 1;
			if(index % 2 == 0) nx += 1;
			if(index % (LINECOUNT / 6) == 0) nx += 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny + nh + 1;
				nx = 2;
				if( (index / LINECOUNT) % (LINECOUNT / 6) == 0) ny += 2;
			}
		}
	}
    else if(type == "4") //* 오른쪽 위가 1번, 왼쪽 방향으로 24 <- 1
	{
		nx = pBase->Width - nw - 2;
		ny = 1;

		for(int index=0; index<MAXCHANNEL;){
			panel[index] = new TPanel(this);
			panel[index]->Parent = pBase;
			panel[index]->Left =  nx;
			panel[index]->Top = ny;
			panel[index]->Width = nw;
			panel[index]->Height = nh;

			panel[index]->Color = pnormal1->Color;
            panel[index]->ParentBackground = false;

			panel[index]->BevelInner = bvNone;
			panel[index]->BevelKind = bkNone;
			panel[index]->BevelOuter = bvNone;
			panel[index]->Tag = index;
//			panel[index]->Caption = index;
//			panel[index]->Hint = IntToStr(index+1) + " (" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
			panel[index]->ShowHint = true;

			index += 1;
			nx = nx - nw - 1;
			if(index % 2 == 0) nx -= 1;
			if(index % (LINECOUNT / 6) == 0) nx -= 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny + nh + 1;
				nx = pBase->Width - nw - 2;
				if( (index / LINECOUNT) % (LINECOUNT / 6) == 0) ny += 2;
			}
		}
	}
	else if(type == "1") //* 왼쪽 아래가 1번. 오른쪽 방향으로 1 -> 24
	{
		nx = 2;
		ny = pBase->Height - nh - 1;

		for(int index=0; index<MAXCHANNEL;){
			panel[index] = new TPanel(this);
			panel[index]->Parent = pBase;
			panel[index]->Left =  nx;
			panel[index]->Top = ny;
			panel[index]->Width = nw;
			panel[index]->Height = nh;

			panel[index]->Color = pnormal1->Color;
			panel[index]->ParentBackground = false;

			panel[index]->BevelInner = bvNone;
			panel[index]->BevelKind = bkNone;
			panel[index]->BevelOuter = bvNone;
			panel[index]->Tag = index;
//			panel[index]->Caption = index;
//			panel[index]->Hint = IntToStr(index+1) + " (" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
			panel[index]->ShowHint = true;

			index += 1;
			nx = nx + nw + 1;
			if(index % 2 == 0) nx += 1;
			if(index % (LINECOUNT / 6) == 0) nx += 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - 1;
				nx = 2;
				if( (index / LINECOUNT) % (LINECOUNT / 6) == 0) ny -= 2;
			}
		}
	}
    else if(type == "2") //* 오른쪽 아래가 1번. 왼쪽 방향으로 24 <- 1
	{
		nx = pBase->Width - nw - 2;
		ny = pBase->Height - nh - 1;

		for(int index=0; index<MAXCHANNEL;){
			panel[index] = new TPanel(this);
			panel[index]->Parent = pBase;
			panel[index]->Left =  nx;
			panel[index]->Top = ny;
			panel[index]->Width = nw;
			panel[index]->Height = nh;

			panel[index]->Color = pnormal1->Color;
			panel[index]->ParentBackground = false;

			panel[index]->BevelInner = bvNone;
			panel[index]->BevelKind = bkNone;
			panel[index]->BevelOuter = bvNone;
			panel[index]->Tag = index;
//			panel[index]->Caption = index;
//			panel[index]->Hint = IntToStr(index+1) + " (" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
			panel[index]->ShowHint = true;

			index += 1;
			nx = nx - (nw + 1);
			if(index % 2 == 0) nx -= 1;
			if(index % (LINECOUNT / 6) == 0) nx -= 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - 1;
				nx = pBase->Width - nw - 2;
				if( (index / LINECOUNT) % (LINECOUNT / 6) == 0) ny -= 2;
			}
		}
	}
    //* 채널 위치 -> 릴레이가 12줄이므로 위치를 계산해야 함
    for(int index = 0; index < MAXCHANNEL; index++)
    {
        if(panel[index] != NULL)
            panel[index]->Hint = IntToStr(index + 1) + " : "
            	+ IntToStr(GetChPosF(chReverseMap, index))
                + "-" + IntToStr(GetChPosR(chReverseMap, index));
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Timer_PLCConnectTimer(TObject *Sender)
{
	if(Mod_PLC->ClientSocket_PC->Active == false && Mod_PLC->ClientSocket_PLC->Active == false)
		Mod_PLC->Connect(PLC_IPADDRESS, PLC_PLCPORT, PLC_PCPORT);
	Timer_PLCConnect->Enabled = false;
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
//  Timer - AutoInspection, ManualInspection, FinishCharging, Reset, Reboot
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Timer_AutoInspectionTimer(TObject *Sender)
{
	if(stage.arl == nAuto && Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_STAGE_AUTO_READY) == 0){
        SetPcValue(PC_D_PRE_STAGE_AUTO_READY, 1);
		PreChargerStatus = "PreCharger STAGE AUTO READY = 1";
		WritePLCLog("PreCharger STAGE AUTO/MANUAL", PreChargerStatus);
	}
	else if(stage.arl == nLocal && Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_STAGE_AUTO_READY) == 1){
        SetPcValue(PC_D_PRE_STAGE_AUTO_READY, 0);
		PreChargerStatus = "PreCharger STAGE AUTO READY = 0";
		WritePLCLog("PreCharger STAGE AUTO/MANUAL", PreChargerStatus);
	}

    if(ErrorCheck())
	{
		Panel_State->Color = clRed;
		Panel_State->Font->Color = clWhite;
		return;
	}
	else
	{
		Panel_State->Caption = "";
		Panel_State->Color = clWhite;
		Panel_State->Font->Color = clBlack;

        BaseForm->advPLCInterfaceShow->Color = clWhite;
	}

	if(stage.arl == nAuto && BaseForm->nForm[0]->Client->Active == true)
	{
        if(GetPlcValue(PLC_D_PRE_TRAY_IN) == 1) tray.trayin = true;
        else tray.trayin = false;

        nTrayPos = GetTrayPos();
		switch(nSection)
		{
			case STEP_WAIT:
				AutoInspection_Wait();
				break;
			case STEP_MEASURE:
				AutoInspection_Measure();
				break;
			case STEP_FINISH:
				AutoInspection_Finish();
				break;
		}
	}
}
//---------------------------------------------------------------------------
bool __fastcall TTotalForm::ErrorCheck()
{
	if(!Client->Active)
	{
	  Panel_State->Caption = "PRECHARGER Connection Fail.";
      if(ReContactTimer->Enabled == false)
      	ReContactTimer->Enabled = true;
      DisplayStatus(nNoAnswer);
	   return true;
	}
    else if(stage.bconnected == false)
    {
        Panel_State->Caption = "BT Connection Fail.";
      	DisplayStatus(nNoAnswer);
	   return true;
    }
	else{
        Panel_State->Caption = "";
		if(stage.alarm_status == nNoAnswer)
        	DisplayStatus(nVacancy);
    }

	if(!Mod_PLC->ClientSocket_PC->Active && !Mod_PLC->ClientSocket_PLC->Active)
	{
		ErrorCheckStatus = "PLC - PC Connection Fail.";
		Panel_State->Caption = ErrorCheckStatus;
		if(OldErrorCheckStatus != ErrorCheckStatus) {
			OldErrorCheckStatus = ErrorCheckStatus;
			WritePLCLog("ErrorCheck", ErrorCheckStatus);
		}

		return true;
	}

	if(Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_PRE_ERROR))
	{
		ErrorCheckStatus = "PLC - Error!!";
		Panel_State->Caption = ErrorCheckStatus;
		if(OldErrorCheckStatus != ErrorCheckStatus) {
			OldErrorCheckStatus = ErrorCheckStatus;
			WritePLCLog("ErrorCheck", ErrorCheckStatus);
		}
        BaseForm->advPLCInterfaceShow->Color = clRed;

        //* 2025 04 08 plc 연결이 안되면 아래 코드 때문에 수동측정도 못함.
		//Initialization();
		//if(Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data[this->Tag], PLC_D_PRE_PROB_CLOSE)
		//		&& Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data[this->Tag], PLC_D_PRE_TRAY_IN))
		//	CmdForceStop();

		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::AutoInspection_Wait()
{
	AnsiString trayid;
	int trayin = 0;

	switch(nStep)
	{
		case 0:
            trayin = GetPlcValue(PLC_D_PRE_TRAY_IN);
			if(trayin)
			{
				if(chkBypass->Checked == true)
				{
					DisplayProcess(sTrayIn, "AutoInspection_Wait", "PreCharger Bypass ...");
					CmdTrayOut();
					nStep = 0;
					nSection = STEP_FINISH;
				}
				else
				{
					DisplayStatus(nIN);
					DisplayProcess(sTrayIn, "AutoInspection_Wait", "PreCharger Tray In ...");

					m_dateTime = Now();
					Initialization(nTrayPos);
					nStep = 1;
				}
			}
			else
			{
				DisplayStatus(nVacancy);
				DisplayProcess(sReady, "AutoInspection_Wait", " PreCharger is ready... ");
			}
			break;
		case 1:
            trayid = GetPlcValue(PLC_D_PRE_TRAY_ID, 10);
			pTrayid->Caption = trayid;
			DisplayStatus(nREADY);

			if(trayid != "")
			{
				DisplayProcess(sBarcode, "AutoInspection_Wait", "BCR OK ...(" + trayid + ")");
				editTrayId->Text = trayid;
				tray.trayid = trayid;
				tray.cell_count = 0;
				nStep = 2;
			}
			else
			{
				DisplayProcess(sBarcode, "AutoInspection_Wait", " BCR Error ... ", true);
				return;
			}
			break;
		case 2:
			DisplayStatus(nREADY);
            for(int i = 0; i < LINECOUNT; i++)
			{
				for(int j = 0; j < LINECOUNT; j++)
				{
					tray.cell[i * LINECOUNT + j] = GetPlcData(PLC_D_PRE_TRAY_CELL_DATA + (i * 2), j);
                    tray.cell_count += tray.cell[i * LINECOUNT + j];
				}
			}
			nStep = 3;
			break;
		case 3:

			if(tray.cell_count > 0)
			{
				WriteTrayInfo();
				DisplayTrayInfo();
				DisplayProcess(sProbeDown, "AutoInspection_Wait", " PROBE IS CLOSED ... ");

                SetPcValue(PC_D_PRE_PROB_CLOSE, 1);

				WriteCommLog("AutoInspection_Wait", "PC_INTERFACE_PROB_CLOSE ...");
				WritePLCLog("AutoInspection_Wait", "PC_D_PRE_PROB_CLOSE = 1");

				nSection = STEP_MEASURE;
				nStep = 0;
			}
			else
			{
				DisplayProcess(sBarcode, "AutoInspection_Wait", " NO CELL ... ", true);

				return;
			}

			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::AutoInspection_Measure()
{
	//* Charging
	if(tray.ams == true && tray.amf == false){
        SetPcValue(PC_D_PRE_CHARGING, 1);
		DisplayProcess(sCharge, "AutoInspection_Measure", " Start charging ... ");
	}
	else{
        SetPcValue(PC_D_PRE_CHARGING, 0);
	}

	double plc_probe_close, plc_tray_in, plc_probe_open;
	switch(nStep)
	{
		case 0:
            plc_probe_close = GetPlcValue(PLC_D_PRE_PROB_CLOSE);
            plc_tray_in = GetPlcValue(PLC_D_PRE_TRAY_IN);
			if(plc_probe_close == 1 && plc_tray_in == 1)
			{
				DisplayStatus(nRUN);
				DisplayProcess(sProbeDown, "AutoInspection_Measure", "PLC - PROBE CLOSED");

                SetPcValue(PC_D_PRE_PROB_CLOSE, 0);

				tray.rst = false;
				nStep = 1;
			}
			break;
		case 1:
            nStep_Count = 0;
			tray.set = CmdCheckSet();
            if(tray.set == true)
            	nStep = 3;
            else {
                CmdEna();
                BaseForm->WaitForMilliSeconds(1000);
                CmdSetStep();
                nStep = 2;
            }
            WriteCommLog("AutoInspection_Measure", "Set for charging, DisplayStatus(nRUN)");
			DisplayStatus(nRUN);
			break;
		case 2:
			nStep_Count++;
            tray.set = CmdCheckSet();
			if(tray.set == true)
			{
				nStep = 3;
                WriteCommLog("AutoInspection_Measure", "verify setting and start charging, DisplayStatus(nRUN)");
			}
			else{
                if(nStep_Count > 3){
                    nStep = 1;
                    WriteCommLog("AutoInspection_Measure", "Precharger is not set. - run CmdSetStep()");
                }
			}
			break;
        case 3:
            tray.ams = false;
            tray.amf = false;
            CmdAutoTest();
            DisplayStatus(nRUN);

            nStep = 4;
        	break;
		case 4:
			if(StatusImage->Picture == BaseForm->statusImage[nNoAnswer]->Picture)
                StatusImage->Picture = BaseForm->statusImage[nRUN]->Picture;

            plc_probe_open = GetPlcValue(PLC_D_PRE_PROB_OPEN);
			if(plc_probe_open == 1 && tray.amf == true)
			{
				DisplayProcess(sProbeOpen, "AutoInspection_Measure", " PLC - PROBE IS OPEN ... ");
				WriteCommLog("AutoInspection_Measure", "PreCharger Finish... ");

                SetPcValue(PC_D_PRE_PROB_OPEN, 0);

                //* NG count 후 셋팅값(20개) 이상이면 에러창
				CmdTrayOut();
				nStep = 0;
				nSection = STEP_FINISH;
			}
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::AutoInspection_Finish()
{
    SetPcValue(PC_D_PRE_CHARGING, 0);

	double plc_tray_in;
	//DisplayStatus(nFinish);
	switch(nStep)
	{
		case 0:       
            plc_tray_in = GetPlcValue(PLC_D_PRE_TRAY_IN);

			if(plc_tray_in == 0)
			{
				WriteCommLog("AutoInspection_Finish", "TRAY OUT");
                SetPcValue(PC_D_PRE_TRAY_OUT, 0);
                SetPcValue(PC_D_PRE_PROB_OPEN, 0);
                SetPcValue(PC_D_PRE_PROB_CLOSE, 0);

				DisplayProcess(sTrayOut, "AutoInspection_Finish", " PreCharger Tray Out ... ");

				//* Tray ID 초기화
				pTrayid->Caption = "";
				editTrayId->Text = "";

				nSection = STEP_WAIT;
				nStep = 0;
			}
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Timer_ManualInspectionTimer(TObject *Sender)
{
    switch(nManualStep)
	{
		case 0:
        	nSetStepCount = 0;
			if(CmdCheckSet() == true)
				nManualStep = 2;
            else {
                CmdEna();
                BaseForm->WaitForMilliSeconds(1000);
                CmdSetStep();
                nManualStep = 1;
            	WriteCommLog("SET", "SetTrayID - CmdSetStep()");
            }
			break;
		case 1:
            nSetStepCount++;
			if(CmdCheckSet() == true){
				nManualStep = 2;
            }
            else {
                if(nSetStepCount > 3){
                	nManualStep = 0;
            		WriteCommLog("SET", "SetTrayID - CmdSetStep()");
                }
            }
			break;
		case 2:
			CmdAutoTest();
			WriteCommLog("ASB", "SetTrayID (" + tray.trayid + ")");
            Timer_ManualInspection->Enabled = false;
			nManualStep++;
			break;
		default:
			break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Timer_FinishChargingTimer(TObject *Sender)
{
    switch(nFinishStep)
    {
        case 0:
            tray.amf = true;
            tray.ams = false;

            nFinishStep = 1;
            break;
        case 1:
            //* FinishCharging 횟수 - 일정횟수 이상 반복하면 중단.
            //* (에러 발생하여 진행이 안되는 경우)
            nFinishCount++;
            if(nFinishCount > 3) nFinishStep = 3;
            else{
                //* check report : 명령어 실행 + 결과 읽기를 한 함수에서 같이 해야 함.
                //* real_data.nResult 에 report 값 대입
                CmdReport();
                nResponseCount = 0;
                real_data.bBT1 = false;  //* bt1 응답 읽음
                real_data.bBT2 = false;  //* bt2 응답 읽음
                nFinishStep = 2;
            }
            break;
        case 2:
        	//* STAT:CELL:REP? 결과를 읽었는지 확인
            nResponseCount++;
            if(nResponseCount > 5) nFinishStep = 1;

            if(real_data.bBT1 == true && real_data.bBT2 == true)
                nFinishStep = 3;
            break;
        case 3:
            //* 마지막 데이터 표시위해 한번 더
    		DisplayChannelInfo(nTrayPos);

            //* final data 정리
            AutoTestFinish();
            nFinishStep = 4;
            break;
        case 4:
            //* probe open
            SetPcValue(PC_D_PRE_PROB_OPEN, 1);
            nFinishStep = 5;
            break;
        case 5:
            Timer_FinishCharging->Enabled = false;
            nFinishStep = 99;
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Timer_ResetTimer(TObject *Sender)
{
    //reset
    switch(nRStep){
        case 0:
            nRStepCount = 0;
            pnlStatus->Caption = "RST";
            //CmdReset();
            nRStep = 1;
        	break;
        case 1:
            if(nRStepCount > 5 && (pnlStatus->Caption == "IDL" || pnlStatus->Caption == "RUN"))
                nRStep = 2;
            else if(nRStepCount > 15){
                pnlResetMsg->Visible = false;
                //* error display
				Form_Error->DisplayErrorMessage(this->Tag, nResetErr);
				Form_Error->Tag = this->Tag;
                nRStep = 2;
            }else
                nRStepCount++;

            pnlResetMsg->Visible = true;
            pnlResetMsg->Caption = "PreCharger is under RESET. Please wait...(" + IntToStr(nRStepCount) + ")";
            break;
        case 2:
            pnlResetMsg->Visible = false;
			Timer_Reset->Enabled = false;
            nRStep = 99;
            break;
        default:
        	break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TTotalForm::Timer_RebootTimer(TObject *Sender)
{
    //reboot
    switch(nRStep){
        case 0:
            nRStepCount = 0;
            pnlStatus->Caption = "RBT";
			//CmdReboot();
            nRStep = 1;
        	break;
        case 1:
            if(nRStepCount > 10 && (pnlStatus->Caption == "IDL" || pnlStatus->Caption == "RUN"))
                nRStep = 2;
            else if(nRStepCount > 60){
                pnlResetMsg->Visible = false;
                //* error display
                Form_Error->DisplayErrorMessage(this->Tag, nRebootErr);
				Form_Error->Tag = this->Tag;
                nRStep = 2;
            }else
                nRStepCount++;

            pnlResetMsg->Visible = true;
            pnlResetMsg->Caption = "PreCharger is under REBOOT. Please wait... (" + IntToStr(nRStepCount) + ")";
            break;
        case 2:
            pnlResetMsg->Visible = false;
            Timer_Reboot->Enabled = false;
            nRStep = 99;
            break;
        default:
        	break;
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Timer - AutoInspection, ManualInspection, FinishCharging, Reset, Reboot
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//  충전데이터 처리 함수
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ChannelStatus()
{
	if(tray.channel_charging == true){
		DisplayChannelInfo(nTrayPos);
	}

	int dTime, dTime2;
	dt1CurrentTime = StrToDateTime(Now());

	dTime = SecondsBetween(dt1CurrentTime, dt1StartTime);
	testTime->Caption = IntToStr(dTime);

	dt1FinishTime = StrToDateTime(Now());
	dTime2 = SecondsBetween(dt1CurrentTime, dt1FinishTime);

	//* keysightEndTime -> 설정시간이 모두 지나고 keysightEndTime 이 되면 종료한다.
	if(tray.ams == true && dTime > 30 && testTime->Caption.ToIntDef(0) > config.time + 7)
	{
        CmdStop();
        //if(BaseForm->WaitForMilliSeconds(1000) == true)
        CmdAutoStop();
	}

}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::StatusTimerTimer(TObject *Sender)
{
    StageStatus();

    if(tray.ams == true)
    	ChannelStatus();

    if(GetPlcValue(PLC_D_PRE_TRAY_IN) == 1) ShowPLCSignal(pnlTrayIn, true);
    else ShowPLCSignal(pnlTrayIn, false);

    if(GetPlcValue(PLC_D_PRE_PROB_OPEN) == 1) ShowPLCSignal(pnlProbeOpen, true);
    else ShowPLCSignal(pnlProbeOpen, false);

    if(GetPlcValue(PLC_D_PRE_PROB_CLOSE) == 1) ShowPLCSignal(pnlProbeClose, true);
    else ShowPLCSignal(pnlProbeClose, false);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::StageStatus()
{
    //* 한 단계에서 일정시간 이상 진행이 안되면 에러 표시
	stage.alarm_cnt += 1;
	switch (stage.alarm_status){
		case nVacancy:
			stage.alarm_cnt = 0;
			break;
		case nIN:
			if(stage.alarm_cnt > 100){
				ErrorMsg(nRedEnd);
				stage.alarm_cnt = 0;
			}
			break;
		case nREADY:
			if(stage.alarm_cnt > 100){
				ErrorMsg(nReadyError);
				stage.alarm_cnt = 0;
			}
			break;
		case nRUN:
			if(stage.alarm_cnt > 300){
				ErrorMsg(nRunningError);
				stage.alarm_cnt = 0;
			}
			break;
		case nEND:
			if(stage.alarm_cnt > 100){
				ErrorMsg(nBlueEnd);
				stage.alarm_cnt = 0;
			}
			break;
		case nFinish:
            if(stage.alarm_cnt > 100){
				ErrorMsg(nFinishError);
				stage.alarm_cnt = 0;
			}
			break;
		case nManual:
		case nNoAnswer:
			stage.alarm_cnt = 0;
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTotalForm::GetStatus(AnsiString status)
{
	int nvalue = status.ToIntDef(0);

	 switch(status.ToIntDef(0)){
		case 0x00: return "Wait(00)";
		case 0x01: return "Contact(01)";
		case 0x02: return "ContactFinished(02)";
		case 0x03: return "Charging(03)";
		case 0x04: return "Discharging(04)";
		case 0x05: return "Rest(05)";
		case 0x06:
//			WriteResultFile();
			return "StepFinished(06)";
		case 0x07: return "Calibration(07)";
		case 0x08: return "Ready(08)";

		case 0x10: return "CmuxComm Error(10)";
		case 0x20: return "CmuxStatus Error(20)";
		case 0x30: return "PowerOVP Error(30)";
		case 0x40: return "PowerUVP Error(40)";
		case 0x50: return "DTrip Error(50)";
		default: return status;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::DisplayChannelInfo(int traypos)
{
    if(real_data.step_index == "+1")
		pstep->Caption = "[1, 1]";
	else if(real_data.step_index == "+2")
		pstep->Caption = "[1, 2]";
	else
		pstep->Caption = "";
	AnsiString sResult, finalresult, strVolt, strCurr;
	AnsiString channelno = "", channeldata = "";
	double volt, curr;
    int channel;
	try{
		for(int i = 0; i < MAXCHANNEL / 2; ++i){
            channel = chMap[(traypos - 1) * (MAXCHANNEL / 2) + i + 1] - 1;
			if(tray.amf)
			{
				if(tray.cell[channel] == 1){
				//* 2024 04 10 충전종료에러 때문에 조건 수정
				//* volt, curr -> final_volt, final_curr
				//* 10, 1000 => 100, 500
					if(real_data.final_result[channel] == "0" || real_data.final_result[channel] == "2"
						|| (real_data.final_curr[channel] < 100 && real_data.final_volt[channel] < 500)){
						//* 결과 NG
						panel[channel]->Color = cl_error->Color;

      				}
					else {
						//* 결과 OK
						panel[channel]->Color = cl_end->Color;
					}
				}

				if(m_sTempCurr[channel].Pos("-") > 1)
				{
					m_sTempCurr[channel] = "0.0";
                    m_sTempVlot[channel] = "0.0";
                }
			}
			else if(tray.ams)
			{
				 if(m_sTempCurr[channel] != "Cell"
                 	&& (real_data.status[channel] > -2 && StringToDouble(real_data.volt[channel],0) > 100)){
					m_sTempVlot[channel] = real_data.volt[channel];
					m_sTempCurr[channel] = real_data.curr[channel];

					//GetCodeColor(panel[channel], i);
				 }
//                 else if(m_sTempCurr[channel] != "Cell"
//					&& (real_data.status[channel] > -2 && BaseForm->StringToDouble(real_data.volt[channel],0) <= 100)){
//					//* 이전값 그대로 표시
//				 }
				 else{
                    m_sTempVlot[channel] = real_data.volt[channel];
					m_sTempCurr[channel] = real_data.curr[channel];

					//GetCodeColor(panel[channel], i);
				 }

                 if(testTime->Caption.ToIntDef(0) > 5)
					GetCodeColor(panel[channel], channel);
			}

			if(MeasureInfoForm->Visible && MeasureInfoForm->stage == this->Tag)
			{
				if(m_sTempCurr[channel].Pos("-") > 1)
				{
					MeasureInfoForm->pvolt[channel]->Color = MeasureInfoForm->pnormal1->Color;
					MeasureInfoForm->pcurr[channel]->Color = MeasureInfoForm->pnormal2->Color;
				}
				else if(m_sTempCurr[channel] == "Cell")
				{
					if(panel[channel]->Color == cl_error->Color || panel[channel]->Color == cl_outflow->Color)
					{
						MeasureInfoForm->pvolt[channel]->Color = panel[channel]->Color;
						MeasureInfoForm->pcurr[channel]->Color = panel[channel]->Color;
					}
					else
					{
						MeasureInfoForm->pvolt[channel]->Color = cl_no->Color;
						MeasureInfoForm->pcurr[channel]->Color = cl_no->Color;
					}
				}
				else
				{
					if(panel[channel]->Color == cl_line->Color ||
						panel[channel]->Color == cl_charge->Color || panel[channel]->Color == cl_end->Color)
					{
						MeasureInfoForm->pvolt[channel]->Color = MeasureInfoForm->pnormal1->Color;
						MeasureInfoForm->pcurr[channel]->Color = MeasureInfoForm->pnormal2->Color;
					}
					else
					{
						MeasureInfoForm->pvolt[channel]->Color = panel[channel]->Color;
						MeasureInfoForm->pcurr[channel]->Color = panel[channel]->Color;
					}
				}

				MeasureInfoForm->pvolt[channel]->Caption = m_sTempVlot[channel];
				MeasureInfoForm->pcurr[channel]->Caption = m_sTempCurr[channel];
			}
		}
	}catch(...){}
    //pnlPos->Caption = "DisplayChannelInfo()";
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTotalForm::GetCodeColor(TPanel *pnl, int index)
{
	TColor clr = clBlack;
	AnsiString str;

	if(tray.cell[index] == 0 && StringToDouble(real_data.volt[index], 0) > 500
		&& MeasureInfoForm->n_bManualStart == false)
		clr = cl_outflow->Color;		// 유출
	else if(tray.cell[index] == 0) clr = cl_no->Color;
	else {
        //* 2025 03 05 status 0 : idle, -2 : done, -4, -5 : abort, -6 : chan exit, -7 : HW Fail
		if(real_data.status[index] < -2
			|| (StringToDouble(real_data.curr[index], 0) < 100 && StringToDouble(real_data.volt[index], 0) < 500))
			clr = cl_error->Color;
		else
			clr = cl_charge->Color;
    }
	if(clr != clBlack)
		pnl->Color = clr;
	return str;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTotalForm::GetCodeColor2(TPanel *pnl, int index)
{
	TColor clr = clBlack;
	AnsiString str;
	if(real_data.status[index] >= 0){
//		int code = StrToInt(real_data.status_code[index]);
		int code = StringToInt(real_data.status[index], 1);

		switch(code){
			case chstNone:
				if(tray.cell[index] == 0) clr = cl_no->Color;
				else clr = cl_error->Color;
				break;
			case chstRunning:		// 충전 Step
				//if(tray.cell[index] == 0 && m_sTempVlot[index].ToDouble() > 500 && MeasureInfoForm->n_bManualStart == false)
                if(tray.cell[index] == 0 && StringToDouble(m_sTempVlot[index], 0) > 500 && MeasureInfoForm->n_bManualStart == false)
				{
					clr = cl_outflow->Color;		// 유출
				}
				else if(tray.cell[index] == 0) clr = cl_no->Color;
				else clr = cl_charge->Color;
				break;
			case chstFail:
				if(tray.cell[index] == 0) clr = cl_no->Color;
				else clr = cl_error->Color;
				break;
			case chstAbort:
				if(tray.cell[index] == 0) clr = cl_no->Color;
				else clr = cl_error->Color;
				break;
			case chstOK:
				clr = cl_end->Color;
				break;
			default:
				break;
		}

		if(clr != clBlack) pnl->Color = clr;
	}
	return str;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetTrayID(AnsiString str_id)
{
	InitTrayStruct(nTrayPos);
	tray.trayid = str_id.Trim();

	if(tray.trayid.IsEmpty())
		tray.trayid = Now().FormatString("yymmddhhnnss");

	tray.cell_count = 0;
    nTrayPos = StringToInt(pnlTrayPos->Caption->Text, 1);
    int channel, rchannel;
	for(int i = 0; i < MAXCHANNEL / 2; i++)
	{
        channel = chMap[(nTrayPos - 1) * (MAXCHANNEL / 2) + i + 1] - 1;
		tray.cell[channel] = 1;
		tray.cell_count++;
		tray.cell_data[channel] = i;
	}

	for(int i = 0; i < MAXCHANNEL / 2; i++)
	{
        //channel = chMap[(nTrayPos - 1) * (MAXCHANNEL / 2) + i + 1] - 1;
        channel = (nTrayPos - 1) * (MAXCHANNEL / 2) + i;
        rchannel = chReverseMap[(nTrayPos - 1) * (MAXCHANNEL / 2) + i + 1];
        if(rchannel >= 289) rchannel  = rchannel - 288;
		m_sTempVlot[channel] = channel;
        m_sTempCurr[channel] = IntToStr((rchannel - 1)/LINECOUNT + 1) + "-" + IntToStr((rchannel - 1)%LINECOUNT + 1);
		//m_sTempCurr[i] = IntToStr((i + 20)/20) + "-" + IntToStr((i % 20)+1);;
		m_sTempVlot_Value[channel] = 0;
		m_sTempCurr_Value[channel] = 0;
	}

	pTrayid->Caption = tray.trayid;
	editTrayId->Text = tray.trayid;

	WriteCommLog("AMS", "SetTrayID (" + tray.trayid + ")");

    //* Manual Charging Start
    m_dateTime = Now();
    nManualStep = 0;
    nSetStepCount = 0;
    if(stage.bconnected == true && Client->Active == true)
    	Timer_ManualInspection->Enabled = true;
    else
        ShowMessage("Equipment is not connected!");
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetResultList()
{
	for(int index = 0; index < MAXCHANNEL; ++index)
	{
        //* 셀이 있을 때 NONE(0), FAIL(2) 은 NG (셀이 있는데 충전이 안되거나 에러가 나면 불량
		if(tray.cell[index] == 1)
		{
			if(real_data.final_result[index] == "0" || real_data.final_result[index] == "2")
				tray.measure_result[index] = 1;
			else
				tray.measure_result[index] = 0;
		}
        //* 셀이 없을 때 RUNNING91), OK(4) 는 NG (셀이 없으면 충전이 안되어야 함.)
		else if(tray.cell[index] == 0)
		{
			if(real_data.final_result[index] == "1" || real_data.final_result[index] == "4")
				tray.measure_result[index] = 1;
			else
            	tray.measure_result[index] = 0;
        }
	}

	CmdForceStop();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::BadInfomation()
{
	int ngCount = 0;
	NgCount = 0;
	for(int i = 0; i < LINECOUNT; ++i){
		for(int j = 0; j < LINECOUNT; j++)
		{
            //* 셀이 있는데 Fail 이면 1, OK 면 0
            //* measure_result == 1 : NG, == 0 : OK
			if((tray.cell[(i * LINECOUNT) + j] == 1) && tray.measure_result[(i * LINECOUNT) + j] == 1)
			{
				Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + (i * 2), j, true);
				acc_remeasure[(i * LINECOUNT) + j] += 1;   // 셀이 있고 에러일 때 count 증가
				ngCount++;
				NgCount++;
			}
			else if(tray.cell[(i * LINECOUNT) + j] == 1 && tray.measure_result[(i * LINECOUNT) + j] == 0)
			{
				Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + (i * 2), j, false);
			}
            //* 셀이 없으면 1.
			else
			{
				Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + (i * 2), j, true);
				ngCount++;
			}
		}
	}

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_NG_COUNT, ngCount);
}

void __fastcall TTotalForm::WriteVoltCurrValue()
{
	// ir value 2 Word
	// 2 Word :  value / 65536 => 윗 주소에 쓰기, value % 65536 => 아래 주소에 쓰기 // herald 2017 11 30
    Mod_PLC->PLC_Write_Result = true;
	for(int i = 0; i < MAXCHANNEL; i++)
	{
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_VOLTAGE_VALUE + i, FormatFloat("00000", (real_data.final_volt[i].ToDouble() * 10)) % (256 * 256));
	}

	for(int i = 0; i < MAXCHANNEL; i++)
	{
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Curr_Data, PC_D_PRE_CURRENT_VALUE + i, FormatFloat("00000", (real_data.final_curr[i].ToDouble() * 10)) % (256 * 256));
	}

}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteMINMAX(int stage_num)
{
	currMin = StringToDouble(editCurrMin->Text, 50);
	setChargeVolt = StringToDouble(editChargeVolt->Text, 4200);
	setChargeCurr = StringToDouble(editChargeCurrent->Text, 200);
	setChargeTime = StringToDouble(editChargeTime->Text, 60);
	//* 2021 02 05
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CURRENT_MIN, currMin);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CHARGE_VOLTAGE, setChargeVolt);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CHARGE_CURRENT, setChargeCurr);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CHARGE_TIME, setChargeTime);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  충전데이터 처리 함수
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//  CONTROLLER connection
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ClientConnect(TObject *Sender, TCustomWinSocket *Socket)
{
	pConInfo->Font->Color = clrConInfo->Color;
	pConInfo->Caption = "PreCharger is connected";
	sock = Socket;

	cmd_mode = 0;
    m_bConnect = true;

	if(stage.arl == nLocal){
		this->CmdManualMod(true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ReContactTimerTimer(TObject *Sender)
{
	ReContactTimer->Enabled = false;
	if(config.recontact == true){
		Client->Active = true;
	}
}
void __fastcall TTotalForm::ClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
	pConInfo->Font->Color = clRed;
	pConInfo->Caption = "Connection failed.";
	ReContactTimer->Enabled = true;
	sock = NULL;
	m_bConnect = false;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ClientError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode)
{
	AnsiString str;
	str = "Connection failed";
    ReContactTimer->Enabled = true;
	pConInfo->Caption = str;
    WriteCommLog("CONNECTION", str);
	ErrorCode = 0;
	Socket->Close();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ClientConnecting(TObject *Sender, TCustomWinSocket *Socket)
{
    pConInfo->Caption = "PreCharger is connecting";
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  CONTROLLER connection
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// 컨트롤러에서 데이터 받아서 처리
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ClientRead(TObject *Sender, TCustomWinSocket *Socket)
{
    const int bufferSize = 1024; // 버퍼 크기 설정
    unsigned char buffer[bufferSize] = {0};  // 바이트 데이터를 저장할 버퍼
    int bytesReceived = Socket->ReceiveBuf(buffer, bufferSize);

    if(bytesReceived > 0){
        dataBuffer.insert(dataBuffer.end(), buffer, buffer + bytesReceived);

        AnsiString hexOutput;
        for(int i = 0; i < dataBuffer.size(); i++){
            hexOutput += IntToHex(dataBuffer[i], 2);
        }

        // @로 시작하는지 확인
        if (!dataBuffer.empty() && dataBuffer[0] == 0x40) {
            // ;(0x3B)로 끝나는지 확인
            if (dataBuffer.back() == 0x3B) {
                // 데이터가 완전하면 처리
                AnsiString finalHex;
                for (size_t i = 0; i < dataBuffer.size(); i++) {
                    finalHex += IntToHex(dataBuffer[i], 2);
                }

                rxq.push(finalHex.c_str());

                // 버퍼 초기화
                dataBuffer.clear();
            }
        } else {
            // 유효한 시작 바이트가 아니면 버퍼를 초기화
            dataBuffer.clear();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::rxTimerTimer(TObject *Sender)
{
	AnsiString RxStr;
	bool flag;

	if(rxq.empty() == false){	// 데이터가 있으면 처리
		RxStr = rxq.front().data();
		rxq.pop();
		SendMessage(BaseForm->nForm[Tag]->Handle, COMM_RECEIVE, 0, (LPARAM)&RxStr);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::OnReceiveStage(TMessage& Msg)
{
	AnsiString *msg, param;
	int cmd = 0;
	msg = (AnsiString*)Msg.LParam;
	int nvalue = 0;

	try{

		if(msg->Trim().IsEmpty()){
			return;
		}

		cmd = DataCheck(*msg, param); 	// cmd , check sum 확인
        WriteCommLog("RX", param);

        //* SEN, MON, 기타 데이터 처리
		switch(cmd){
            case SEN:
                ProcessSEN(param);
                break;
            case MON:
                ProcessMON(param);
                break;
            case RPY:
                ProcessRPY(param);
                break;
            default:
                break;
		}

        //* 충전 데이터 처리 : MON 데이터 저장, 최종 결과 데이터 저장
        //ChannelStatus();


	}catch(...){
		this->WriteCommLog("ERR", "Except Error : " + *msg);
	}
}
//---------------------------------------------------------------------------
int __fastcall TTotalForm::DataCheck(AnsiString msg, AnsiString &param)
{
    AnsiString stx, etx;
	AnsiString cmd;

	try{
		int data_len = msg.Length();
		int param_len = 0;

		AnsiString header = "";
		if(data_len >= 18)
			header = BaseForm->HexToAscii(msg, 18);
		if(header.SubString(1, 4) == "@MON"){
			param_len = data_len - 18; //@MONaaaa#
			param = header + msg.SubString(19, param_len);
		}
		else{
			param = BaseForm->HexToAscii(msg);
		}

		cmd = header.SubString(2,3);

		if(cmd == "SEN") return SEN;
		else if(cmd == "MON") return MON;
		else if(cmd == "BT1" || cmd == "BT2") return RPY;
		//else return RPY;
    }catch(...){
		this->WriteCommLog("ERR", "Except Error : " + msg);
	}

	return -1;
}
//---------------------------------------------------------------------------
// 컨트롤러에서 데이터 받아서 처리
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// SEN, MON, RPY 처리
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ProcessMON(AnsiString param)
{
    AnsiString monData1 = param.SubString(1, 9);
    AnsiString monData2 = param.SubString(10, param.Length() - 9);

    AnsiString run_count = monData1.SubString(5,4);

    int runtimelen = 16;
    AnsiString runtime = monData2.SubString(1, runtimelen);

    int statuslen = 4 * MAXCHANNEL;
    AnsiString status = monData2.SubString(runtimelen + 1, statuslen);

    int currentlen = 8 * MAXCHANNEL;
    AnsiString current = monData2.SubString(runtimelen + statuslen + 1, currentlen);

    int voltagelen = 8 * MAXCHANNEL;
    AnsiString voltage = monData2.SubString(runtimelen + statuslen + currentlen + 1, voltagelen);

    int capacitylen = 8 * MAXCHANNEL;
    AnsiString capacity = monData2.SubString(runtimelen + statuslen + currentlen + voltagelen + 1, capacitylen);

    SET_MONDATA(run_count, runtime, status, current, voltage, capacity);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SET_MONDATA(AnsiString runcount, AnsiString runtime,
	AnsiString status, AnsiString current, AnsiString voltage, AnsiString capacity)
{
    //* runcount : 현재 동작중인 채널 갯수
    SetRunCount(runcount);

    //* runtime : BT 부팅이후 시간 (ms)

    //* status, voltage, current
    startOffset = (nTrayPos - 1) * (MAXCHANNEL / 2);  // 0 or 288
    SetStatus(status);
    SetVoltage(voltage);
    SetCurrent(current);

	//* mon 데이터 저장
    if(stage.status == RUN)
		WriteMonData(nTrayPos);

    //* update final 데이터 (충전 종료 후 최종 값 저장용)
	SetFinalData();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ProcessSEN(AnsiString param)
{
    AnsiString eqStatus = "";
    AnsiString runCount = "";
    AnsiString connection = "";
    AnsiString servo = "";
    AnsiString stepping1 = "";
    AnsiString stepping2 = "";
    AnsiString temperature = "";
    AnsiString runStatus = "";
    AnsiString btSetting1 = "", btSetting2 = "";
    AnsiString servoStatus = "";

    vector<AnsiString> senStatus = BaseForm->SplitString(param, "#");

    if(senStatus.size() == 4){
        runStatus = senStatus[0];
        btSetting1 = senStatus[1];
        btSetting2 = senStatus[2];
        servoStatus = senStatus[3];

        //* runStatus - @SENIDL0F8901
        for(int nIndex = 0; nIndex < 13; nIndex++){
            eqStatus = runStatus.SubString(5, 3);       //* SSS
            runCount = runStatus.SubString(8, 4);       //* CCCC
            connection = runStatus.SubString(12, 2);    //* TT
        }

        //* btSetting1, btSetting2
        vector<AnsiString> btSet1 = BaseForm->SplitString(btSetting1, ",");
        vector<AnsiString> btSet2 = BaseForm->SplitString(btSetting2, ",");

        //* servo status - bbbbcccc 예약

        SET_SENDATA(eqStatus, runCount, connection, servoStatus, btSet1, btSet2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SET_SENDATA(AnsiString eqstatus, AnsiString runcount,
	AnsiString connection, AnsiString servostatus, vector<AnsiString> btset1, vector<AnsiString> btset2)
{
    //* 상태정보 RUN, IDL, WDT, RST, RBT
    //stage.equipstatus = GetEquipStatus(eqstatus);
    SetEquipStatus(eqstatus);

    //* 연결정보 01(BT1), 02(BT2), 03(BT1, BT2)
    if(connection == "00"){
        BaseForm->SetColorPanel(BaseForm->pnlBT1, false);
        BaseForm->SetColorPanel(BaseForm->pnlBT2, false);
        stage.bconnected = false;
    }
    if(connection == "01") {
    	BaseForm->SetColorPanel(BaseForm->pnlBT1, true);
        BaseForm->SetColorPanel(BaseForm->pnlBT2, false);
        stage.bconnected = false;
    }
    else if(connection == "02") {
    	BaseForm->SetColorPanel(BaseForm->pnlBT1, false);
        BaseForm->SetColorPanel(BaseForm->pnlBT2, true);
        stage.bconnected = false;
    }
    else if(connection == "03") {
        BaseForm->SetColorPanel(BaseForm->pnlBT1, true);
        BaseForm->SetColorPanel(BaseForm->pnlBT2, true);
        stage.bconnected = true;
    }

    //* BT SETTING (time, current, voltage)
    //* PRECHARGE2,+30,+1.23000000E+00,+4.00000000E+00#PRECHARGE2,+30,+1.23000000E+00,+4.00000000E+00
    if(btset1.size() == 4){
        charge[0].time = StringToInt(btset1[1], 180);
        charge[0].curr = StringToDouble(btset1[2], 1.23);
        charge[0].volt = StringToDouble(btset1[3], 4);
    }

    if(btset2.size() == 4){
        charge[1].time = StringToInt(btset2[1], 180);
        charge[1].curr = StringToDouble(btset2[2], 1.23);
        charge[1].volt = StringToDouble(btset2[3], 4);
    }

    lblSet1->Caption = FormatFloat("0", charge[0].volt * 1000.0) + "mV "
    	+ FormatFloat("0", charge[0].curr * 1000.0) + "mA " + FormatFloat("0", charge[0].time) + "s";

    //* 충전종료 및 Final 데이터 저장
    if(stage.status == IDL && stage.oldstatus == RUN)
    {
        CmdAutoStop();
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ProcessRPY(AnsiString param)
{
    LASTRESPONSE = param;

    if(LASTCMD == "REP"){
        SetFinalResult(param);
//    } else if(LASTCMD == "RST" || LASTCMD == "RBT"){
//        if(param.Pos("No error") > 0)
//            nRStep = 2;
    } else if(LASTCMD == "SET"){
        if(param.Pos("No error") > 0)
            MeasureInfoForm->nStep = 1; // check setup
        else
            MeasureInfoForm->nStep = 3; // reset
    } else if(LASTCMD == "ENA"){
        if(param.Pos("No error"))
            MeasureInfoForm->nStep = 4; // end
        else if(param.Pos("-1001") > 0)
            MeasureInfoForm->nStep = 3; // reset
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetRunCount(AnsiString strRuncount)
{
    AnsiString hexStr = "";
    hexStr = strRuncount;
    int16_t hexValue = strtol(hexStr.c_str(), NULL, 16);
    stage.runcount = hexValue;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetStatus(AnsiString strStatus)
{
    AnsiString hexStr = "";
    int ch = 0;
    for(int nIndex = 0; nIndex < MAXCHANNEL / 2; nIndex++){
        hexStr = strStatus.SubString(nIndex * 4 + 3, 2) + strStatus.SubString(nIndex * 4 + 1, 2);
        int16_t hexValue = strtol(hexStr.c_str(), NULL, 16);

        ch = chMap[startOffset + nIndex + 1];
        real_data.status[ch - 1] = hexValue;
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetVoltage(AnsiString strVoltage)
{
    AnsiString hexStr = "";
    int ch = 0;
    for(int nIndex = 0; nIndex < MAXCHANNEL / 2; nIndex++){
        hexStr = strVoltage.SubString(nIndex * 8 + 7, 2) + strVoltage.SubString(nIndex * 8 + 5, 2)
        	+ strVoltage.SubString(nIndex * 8 + 3, 2) + strVoltage.SubString(nIndex * 8 + 1, 2);

        uint32_t hexValue = strtoul(hexStr.c_str(), NULL, 16);
        float fVal;
        memcpy(&fVal, &hexValue, sizeof(float));
        double dVal = static_cast<double>(fVal) * 1000;

        ch = chMap[startOffset + nIndex + 1];
		real_data.volt[ch - 1] = FormatFloat("0.0", dVal);
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetCurrent(AnsiString strCurrent)
{
    AnsiString hexStr = "";
    int ch = 0;
    for(int nIndex = 0; nIndex < MAXCHANNEL / 2; nIndex++){
        hexStr = strCurrent.SubString(nIndex * 8 + 7, 2) + strCurrent.SubString(nIndex * 8 + 5, 2)
        	+ strCurrent.SubString(nIndex * 8 + 3, 2) + strCurrent.SubString(nIndex * 8 + 1, 2);

        uint32_t hexValue = strtoul(hexStr.c_str(), NULL, 16);
        float fVal;
        memcpy(&fVal, &hexValue, sizeof(float));
        double dVal = static_cast<double>(fVal) * 1000;

        ch = chMap[startOffset + nIndex + 1];
		real_data.curr[ch - 1] = FormatFloat("0.0", dVal);//FloatToStr(dVal);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetCapacity(AnsiString strCapacity)
{
    AnsiString hexStr = "";
    int ch = 0;
    for(int nIndex = 0; nIndex < MAXCHANNEL / 2; nIndex++){
        hexStr = strCapacity.SubString(nIndex * 8 + 7, 2) + strCapacity.SubString(nIndex * 8 + 5, 2)
        	+ strCapacity.SubString(nIndex * 8 + 3, 2) + strCapacity.SubString(nIndex * 8 + 1, 2);

        uint32_t hexValue = strtoul(hexStr.c_str(), NULL, 16);
        float fVal;
        memcpy(&fVal, &hexValue, sizeof(float));
        double dVal = static_cast<double>(fVal) * 1000;

        ch = chMap[startOffset + nIndex + 1];
        real_data.capa[ch - 1] = FormatFloat("0.0", dVal);
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetFinalResult(AnsiString strResult)
{
    //* 400 채널은 208 + 192 -> 200 + 192 + 8
    //* 576 채널은 288 * 2 -> 144 + 144
    //* 트레이 위치에 따라 288. (위치를 1번 이동시킴)
    int nResultIndex = 0;
    int MAX_SIZE;
    int channel;

    // "@BT1: "과 ";" 사이의 데이터 추출
    int startPos = 0;
    int endPos = 0;
    endPos = strResult.Pos(";");
    if(strResult.Pos("@BT1:") > 0) {
    	startPos = strResult.Pos("@BT1:");

        MAX_SIZE = 144;
        nResultIndex = 1;
        real_data.bBT1 = true;

        if (startPos > 0 && endPos > startPos) {
            // 데이터 추출 (공백 제거)
            String extracted = strResult.SubString(startPos + 5, endPos - (startPos + 5)).Trim();

            // `,`로 분리하여 배열에 저장
            int pos;
            while ((pos = extracted.Pos(",")) > 0 && nResultIndex < MAX_SIZE) {
                channel = chMap[startOffset + nResultIndex];

                real_data.final_result[channel - 1] = extracted.SubString(1, pos - 1).Trim();  // 배열에 추가
                extracted = extracted.SubString(pos + 1, extracted.Length() - pos);

                nResultIndex++;
            }
            if (nResultIndex < MAX_SIZE + 1) {
                channel = chMap[startOffset + nResultIndex];

                real_data.final_result[channel - 1] = extracted.Trim();  // 마지막 값 추가
            }
        }
    }
    else if(strResult.Pos("@BT2:") > 0) {
    	startPos = strResult.Pos("@BT2:");

        MAX_SIZE = 288;
        nResultIndex = 145;
        real_data.bBT2 = true;

        if (startPos > 0 && endPos > startPos) {
            // 데이터 추출 (공백 제거)
            String extracted = strResult.SubString(startPos + 5, endPos - (startPos + 5)).Trim();

            // `,`로 분리하여 배열에 저장
            int pos;
            while ((pos = extracted.Pos(",")) > 0 && nResultIndex < MAX_SIZE) {
                channel = chMap[startOffset + nResultIndex];

                real_data.final_result[channel - 1] = extracted.SubString(1, pos - 1).Trim();  // 배열에 추가
                extracted = extracted.SubString(pos + 1, extracted.Length() - pos);

                nResultIndex++;
            }
            if (nResultIndex < MAX_SIZE + 1) {
                channel = chMap[startOffset + nResultIndex];

                real_data.final_result[channel - 1] = extracted.Trim();  // 마지막 값 추가
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetFinalData()
{
    int channel;
    for(int nIndex = 0; nIndex < MAXCHANNEL / 2; nIndex++){
        channel = chMap[startOffset + nIndex + 1] - 1;
		if(real_data.status[channel] > -2 && StringToDouble(real_data.volt[channel], 0) > 100){
			real_data.final_status[channel] = real_data.status[channel];
            real_data.final_volt[channel] = real_data.volt[channel];
            real_data.final_curr[channel] = real_data.curr[channel];
            real_data.final_capa[channel] = real_data.capa[channel];
		}
		//* -2는 무시
		//* -2는 done 상태로 전압, 전류값이 점점 줄어든다. => 이 값은 final 데이터로 처리하면 안됨.
		else if(real_data.status[channel] < -2){
			if(StringToDouble(real_data.curr[channel], 0) < 100.0){
				real_data.final_curr[channel] = "0";
			}

			if(StringToDouble(real_data.curr[channel], 0) < 100.0
				&& StringToDouble(real_data.volt[channel], 0) < 100.0){
				real_data.final_volt[channel] = "0";
			}

			//* 2024 04 10 009 if(BaseForm->StringToDouble(real_data.volt[channel], 0) < 100.0) real_data.final_volt[channel] = "0";
			//* 2024 04 10 001 //else real_data.final_volt[channel] = real_data.volt[channel];

			//* 2024 04 10 009 if(BaseForm->StringToDouble(real_data.curr[channel], 0) < 100.0) real_data.final_curr[channel] = "0";
			//* 2024 04 10 001 //else real_data.final_curr[channel] = real_data.curr[channel];

            real_data.final_capa[channel] = real_data.capa[channel];
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetEquipStatus(AnsiString eqStatus)
{
    pnlStatus->Caption = eqStatus;

    if(eqStatus == "IDL"){
        if(stage.status == stage.oldstatus) stage.status = IDL;
        else if(stage.status != stage.oldstatus) stage.oldstatus = IDL;

        tray.channel_charging = false;
    }
    else if(eqStatus == "RUN"){
        if(stage.status == stage.oldstatus) stage.status = RUN;
        else if(stage.status != stage.oldstatus) stage.oldstatus = RUN;

        tray.channel_charging = true;
    }
    else if(eqStatus == "WDT"){
        if(stage.status == stage.oldstatus) stage.status = WDT;
        else if(stage.status != stage.oldstatus) stage.oldstatus = WDT;

        tray.channel_charging = false;
        //* reset
    }
    else if(eqStatus == "REC"){
        if(stage.status == stage.oldstatus) stage.status = REC;
        else if(stage.status != stage.oldstatus) stage.oldstatus = REC;

        tray.channel_charging = false;
        //* reboot
    }
}
//---------------------------------------------------------------------------
// SEN, MON, RPY 처리
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// 충전 셋팅
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ConfigBtn1Click(TObject *Sender)
{
    PreChargeSet();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTotalForm::convertCondition(AnsiString condition)
{
	int dCondition = condition.ToIntDef(0);
	double fCondition = dCondition / 1000.0;
	return FormatFloat("0.000", fCondition);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTotalForm::convertCondition2(int iCondition)
{
	double dCondition = iCondition / 1000.0;
	return FormatFloat("0.000", dCondition);
}
//---------------------------------------------------------------------------
void _fastcall TTotalForm::PreChargeSet()
{
    if(tray.ams == false)
	{
		int volt = editChargeVolt->Text.ToIntDef(4200);
        if(volt < 2000) {
        	ShowMessage("Please use Voltage more than 2000mV");
            editChargeVolt->Text = "2000";
        }

		int curr = editChargeCurrent->Text.ToIntDef(260);
        if(curr < 200) {
            ShowMessage("Please use Current more than 500mA");
            editChargeCurrent->Text = "500";
        }

		int time = editChargeTime->Text.ToIntDef(240);
        if(time < 30) {
            ShowMessage("Please use Time more than 30sec");
            editChargeTime->Text = "30";
        }

		int max_volt = editMaxChargeVolt->Text.ToIntDef(4200);
		int max_curr = editMaxChargeCurrent->Text.ToIntDef(2500);
		int max_time = editMaxChargeTime->Text.ToIntDef(300);

		if(volt >= max_volt) editChargeVolt->Text = FormatFloat("00", max_volt);
		if(curr >= max_curr) editChargeCurrent->Text = FormatFloat("00", max_curr);
		if(time >= max_time) editChargeTime->Text = FormatFloat("00", max_time);


		if(MessageBox(Handle, L"Are you sure want to save?", L"SAVE", MB_YESNO|MB_ICONQUESTION) == ID_YES){
			//CmdSetStep();
			WriteSystemInfo();
			ReadSystemInfo();
			WriteRemeasureInfo();

            //* SetStep
            MeasureInfoForm->SetStep();
		}
	}
	else ShowMessage("You can't set up while charging.");
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetSystemInfo()
{
    WriteSystemInfo();
    ReadSystemInfo();
}
//---------------------------------------------------------------------------
// 충전 셋팅
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  control event
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnMeasureInfoClick(TObject *Sender)
{
	//* test start
	//	for(int i = 0; i < 256; i++)
	//	{
	//		tray.cell[i] = 1;
	//	}
	//	DisplayTrayInfo();
	//* test end
    MeasureInfoForm->Left = 620;
	MeasureInfoForm->Top = 85;
	for(int i = 0; i < MAXCHANNEL; ++i){
        MeasureInfoForm->pvolt[i]->Caption = "-";
        MeasureInfoForm->pvolt[i]->ParentBackground = false;
        MeasureInfoForm->pcurr[i]->Caption = "-";
        MeasureInfoForm->pcurr[i]->Color = pnormal2->Color;
	}	// 모두 초기화

    MeasureInfoForm->SetChannelInfo();
	InitMeasureForm();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnConnectPRECHARGERClick(TObject *Sender)
{
    if(Client->Active == false){
        Client->Host = editPRECHARGERIPAddress->Text;
		config.recontact = true;
		this->ReContactTimerTimer(ReContactTimer);
	}
	else{
		Client->Active = false;
        Client->Host = editPRECHARGERIPAddress->Text;   // Client->Active == true  일때는 변경안됨
		config.recontact = true;
		this->ReContactTimerTimer(ReContactTimer);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnCloseConnConfigClick(TObject *Sender)
{
    pnlConfig->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnDisConnectPLCClick(TObject *Sender)
{
	Mod_PLC->DisConnect();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TTotalForm::lblChargingProcessDblClick(TObject *Sender)
{
	pnlCurrMin->Visible = !pnlCurrMin->Visible;
    editCurrMin->Visible = !editCurrMin->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TTotalForm::btnConfigClick(TObject *Sender)
{
	pnlConfig->Visible = !pnlConfig->Visible;
    pnlConfig->BringToFront();
	pnlConfig->Left = 20;
    pnlConfig->Top = 60;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnConnectPLCClick(TObject *Sender)
{
    WriteSystemInfo();
    ReadSystemInfo();
	Mod_PLC->Connect(PLC_IPADDRESS, PLC_PLCPORT, PLC_PCPORT);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnNgInfoClick(TObject *Sender)
{
    RemeasureForm->stage            = this->Tag;
	RemeasureForm->acc_remeasure 	= acc_remeasure;
	RemeasureForm->acc_init 		= &acc_init;
	RemeasureForm->acc_cnt			= &acc_cnt;

	RemeasureForm->pstage->Caption	= lblTitle->Caption;
	RemeasureForm->Visible = true;
}
//---------------------------------------------------------------------------
// 수동 전지 검사
// 트레이 보내기 - 재측정
void __fastcall TTotalForm::BitmapBtn5Click(TObject *Sender)
{
	CmdTrayOut();
	VisibleBox(GrpMain);
}
//---------------------------------------------------------------------------
// 트레이 보내기 - 메인폼
// 검사시작 - 메인폼
void __fastcall TTotalForm::MainBtnClick(TObject *Sender)
{
	switch(stage.arl){
		case nAuto:
			VisibleBox(GrpMain);
			break;
		case nLocal:
			VisibleBox(GrpLocal);
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::chkBypassMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(chkBypass->Checked && Button == mbLeft){
		if(MessageBox(Handle, L"Are you want to change BYPASS mode?", L"BYPASS", MB_YESNO|MB_ICONQUESTION) == ID_NO){
			chkBypass->Checked = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnManualClick(TObject *Sender)
{
	//if(nSection == STEP_WAIT &&	nStep == 0)
	//{
		stage.arl_reserve = nLocal;
		stage.arl = nLocal;
		this->CmdManualMod(true);
		VisibleBox(GrpLocal);

        InitMeasureForm();
		MeasureInfoForm->pLocal->Visible = true;
	//}
	//else ShowMessage("You cannot change it manually while charging.");
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnAutoClick(TObject *Sender)
{
	stage.arl_reserve = nAuto;
	stage.arl = nAuto;
    MeasureInfoForm->pLocal->Visible = false;
	this->CmdManualMod(false);
	VisibleBox(GrpMain);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnTrayOutClick(TObject *Sender)
{
	if(MessageBox(Handle, L"Are you sure you want to eject the tray?", L"", MB_YESNO|MB_ICONQUESTION) == ID_YES){
//		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  PC_D_PRE_PROB_CLOSE, 0);
//		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  PC_D_PRE_PROB_OPEN, 1);
//		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  PC_D_PRE_TRAY_OUT, 1);
        SetPcValue(PC_D_PRE_PROB_CLOSE, 0);
        SetPcValue(PC_D_PRE_PROB_OPEN, 1);
        SetPcValue(PC_D_PRE_TRAY_OUT, 1);

		nStep = 0;
		nSection = STEP_FINISH;
	}
}
// 그룹박스 보여주기
void __fastcall TTotalForm::VisibleBox(TGroupBox *grp)
{
	if(grp->Visible == false){

		grp->Left = GrpMain->Left;
		grp->Top = GrpMain->Top;

		if( (grp == GrpError) || (grp == GrpAlarm) ){
			if(grp->Visible == false)BaseForm->IncErrorCount();
			//Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  PC_D_PRE_ERROR, 1);
            SetPcValue(PC_D_PRE_ERROR, 1);
		}
		else {
        	//Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  PC_D_PRE_ERROR, 0);
            SetPcValue(PC_D_PRE_ERROR, 0);
        }
		if( (CurrentGrp == GrpError) || (CurrentGrp == GrpAlarm) ){
			BaseForm->DecErrorCount();
		}

		if(bconfig != true){
			if(CurrentGrp != NULL){
				CurrentGrp->Visible = false;
			}
			grp->Visible = true;
			OldGrp = CurrentGrp;
			CurrentGrp = grp;
		}
		else{
			OldGrp = grp;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::editTrayIdKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	UnicodeString str;
	if(Key == VK_RETURN){
		str = "[" + editTrayId->Text + "]" + " Are you sure you want inspection?";
		if(MessageBox(Handle, str.c_str(), L"", MB_YESNO|MB_ICONQUESTION) == ID_YES){
			pTrayid->Caption = editTrayId->Text;
			SetTrayID(pTrayid->Caption);
		}
		editTrayId->Visible = false;
	}
	if(Key == VK_ESCAPE){
		editTrayId->Visible = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    WriteRemeasureInfo();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnSaveConfigClick(TObject *Sender)
{
	//if(editMaxChargeCurrent->Text.ToDouble() <= 2500)
    if(StringToDouble(editMaxChargeCurrent->Text, 1200) <= 3000)
	{
		if(stage.arl == nLocal && nSection == STEP_WAIT && nStep == 0)
		{
			if(MessageBox(Handle, L"Are you sure want to save?", L"SAVE", MB_YESNO|MB_ICONQUESTION) == ID_YES){
				WriteSystemInfo();
				WriteRemeasureInfo();
				ReadSystemInfo();
			}
		}
		else ShowMessage("You can't set up while charging.");
	}
	else
	{
		ShowMessage("Current can't be set above 3000mA.");
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnInitClick(TObject *Sender)
{
	WritePLCLog("Init", "Initialization()");

	Initialization(1);
    Initialization(2);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  control event
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  기타함수
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ReadchannelMapping()
{
	AnsiString str, FileName;
	int file_handle;

	FileName = (AnsiString)BIN_PATH + "mapping.csv";

	TStringList *data;
	data = new TStringList;

	if (FileExists(FileName)) {

		data->LoadFromFile(FileName);

		for (int i = 1; i <= MAXCHANNEL; ++i) { // for (int i = 1; i <= 256; ++i) {
			str = data->Strings[i];
			str.Delete(1, str.Pos(",")); // 채널
			chMap[i] = str.ToIntDef(i);
			chReverseMap[str.ToIntDef(i)] = i;
		}
	}
	else {
		data->Add("변경전, 변경후");
		for (int i = 1; i <= MAXCHANNEL; ++i) { // for (int i = 1; i <= 256; ++i) {
			chMap[i] = i;
			chReverseMap[i] = i;
			data->Add(IntToStr(i) + "," + IntToStr(i));
		}
		data->SaveToFile(FileName);

	}
	delete data;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  기타함수
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ShowPLCSignal(TAdvSmoothPanel *advPanel, bool bOn)
{
    if(bOn)
	{
		advPanel->Fill->Color = BaseForm->pon->Color;
		advPanel->Fill->ColorMirror = BaseForm->pon->Color;
		advPanel->Fill->ColorMirrorTo = BaseForm->pon->Color;
		advPanel->Fill->ColorTo = BaseForm->pon->Color;
	}else{
		advPanel->Fill->Color = BaseForm->poff->Color;
		advPanel->Fill->ColorMirror = BaseForm->poff->Color;
		advPanel->Fill->ColorMirrorTo = BaseForm->poff->Color;
		advPanel->Fill->ColorTo = BaseForm->poff->Color;
	}
}







