//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMeasureInfo.h"
#include "RVMO_main.h"
#include "Util.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvSmoothButton"
#pragma link "AdvSmoothButton"
#pragma resource "*.dfm"
TMeasureInfoForm *MeasureInfoForm;
//---------------------------------------------------------------------------
__fastcall TMeasureInfoForm::TMeasureInfoForm(TComponent* Owner)
	: TForm(Owner)
{
	n_bManualStart = false;

    this->Parent = BaseForm;
	stage = 0;

    this->Left = 0;
    this->Top = 0;
    pnl_nw = 40;
    pnl_nh = 34;
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::FormShow(TObject *Sender)
{
    this->BringToFront();
	pLocal->Visible = false;
    GroupBox3->Height = 230;

    chartVoltage->Width = 998;
    chartVoltage->Height = 433;
    chartVoltage->Left = 4;
    chartVoltage->Top = pnl_nh + 6;

    chartCurrent->Width = 998;
    chartCurrent->Height = 433;
    chartCurrent->Left = 4;
    chartCurrent->Top = chartVoltage->Height + pnl_nh + 8;

    //* 기본 색상
//    pnormal1->Color = (TColor)0x00FFE8D9;
//    pnormal2->Color = (TColor)0x00FFE6FF;
//
//    pnormal3->Color = clWhite;
//    pnormal31->Color = (TColor)0x00EBEBEB;
//    pnormal4->Color = (TColor)0x00BCBCBC;
//    pnormal41->Color = (TColor)0x00CCCCCC;

    //* 2025 09 16 - 검수 요청 색상
    pnormal3->Color = clWhite;
    pnormal31->Color = clWhite;
    pnormal4->Color = (TColor)0x00FFE8D9;
    pnormal41->Color = (TColor)0x00FFE8D9;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::FormCreate(TObject *Sender)
{
	chartVoltage->Series[0]->Clear();
	chartCurrent->Series[0]->Clear();
	for (int i = 0; i < MAXCHANNEL; ++i) {
		chartVoltage->Series[0]->AddXY(i + 1, 0);
		chartCurrent->Series[0]->AddXY(i + 1, 0);
	}

	MakeUIPanel(BaseForm->lblLineNo->Caption);
	MakePanel(BaseForm->lblLineNo->Caption);
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::MakePanel(AnsiString type)
{
	int nx, ny, nw, nh;
    nw = pnl_nw;
    nh = pnl_nh / 2;

	if(type == "3") //* 왼쪽 위가 1번, 오른쪽 방향으로 1 -> 24
	{
		nx = nw + 4;
		ny = nh * 2 + 6;

		for(int index = 0; index < MAXCHANNEL;){
			pvolt[index] = new TPanel(this);
			pcurr[index] = new TPanel(this);

			SetOption(pvolt[index], nx, ny, nw, nh-1, index);
			SetOption(pcurr[index], nx, ny+nh, nw, nh, index);
			//pcurr[index]->Caption = IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index+LINECOUNT)%LINECOUNT);
			pcurr[index]->Color = pnormal2->Color;
			pcurr[index]->ParentBackground = false;
			pvolt[index]->ParentBackground = false;

			index += 1;
			nx += (nw + 1);
			if(index % 2 == 0) nx += 1;
			if(index % (LINECOUNT / 6) == 0) nx += 1;
			if(index % LINECOUNT == 0)
			{
				ny += nh * 2 + 2;
				nx = nw + 4;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny += 2;
			}
		}
	}
    else if(type == "4") //* 오른쪽 위가 1번, 왼쪽 방향으로 24 <- 1
	{
		nx = pUIx[0]->Left;
		ny = nh * 2 + 6;

		for(int index = 0; index < MAXCHANNEL;){
			pvolt[index] = new TPanel(this);
			pcurr[index] = new TPanel(this);

			SetOption(pvolt[index], nx, ny, nw, nh-1, index);
			SetOption(pcurr[index], nx, ny+nh, nw, nh, index);
			//pcurr[index]->Caption = IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index+LINECOUNT)%LINECOUNT);
			pcurr[index]->Color = pnormal2->Color;
			pcurr[index]->ParentBackground = false;
			pvolt[index]->ParentBackground = false;

			index += 1;
			nx -= (nw + 1);
			if(index % 2 == 0) nx -= 1;
			if(index % (LINECOUNT / 6) == 0) nx -= 1;
			if(index % LINECOUNT == 0)
			{
				ny += nh * 2 + 2;
				nx = pUIx[0]->Left;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny += 2;
			}
		}
	}
    //* 왼쪽 아래가 1번. 오른쪽 방향으로 1 -> 24
	else if(type == "1")
	{
		nx = pUIx[0]->Left;
		ny = pUIy[0]->Top;

		for(int index=0; index<MAXCHANNEL;){
			pvolt[index] = new TPanel(this);
			pcurr[index] = new TPanel(this);

			SetOption(pvolt[index], nx, ny, nw, nh, index);
			SetOption(pcurr[index], nx, ny + nh + 1, nw, nh, index);
			//pcurr[index]->Caption = IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index+LINECOUNT)%LINECOUNT);
			pcurr[index]->Color = pnormal2->Color;
			pcurr[index]->ParentBackground = false;
			pvolt[index]->ParentBackground = false;

			index += 1;
			nx = nx + (nw + 1);
			if(index % 2 == 0) nx += 1;
			if(index % (LINECOUNT / 4) == 0) nx += 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - nh - 2;
				nx = nx = pUIx[0]->Left;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny -= 2;
			}
		}
    }
    //* 오른쪽 아래가 1번. 왼쪽 방향으로 24 <- 1
    else if(type == "2")
	{
		nx = pUIx[0]->Left;
		ny = pUIy[0]->Top;

		for(int index=0; index<MAXCHANNEL;){
			pvolt[index] = new TPanel(this);
			pcurr[index] = new TPanel(this);

			SetOption(pvolt[index], nx, ny, nw, nh, index);
			SetOption(pcurr[index], nx, ny + nh + 1, nw, nh, index);
			//pcurr[index]->Caption = IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index+LINECOUNT)%LINECOUNT);
			pcurr[index]->Color = pnormal2->Color;
			pcurr[index]->ParentBackground = false;
			pvolt[index]->ParentBackground = false;

			index += 1;
			nx = nx - (nw + 1);
			if(index % 2 == 0) nx -= 1;
			if(index % (LINECOUNT / 4) == 0) nx -= 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - nh - 2;
				nx = pUIx[0]->Left;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny -= 2;
			}
		}
    }
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::SetChannelInfo()
{
    SetChannelInfo(1);
    SetChannelInfo(2);
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::SetChannelInfo(int traypos)
{
    //* 채널 위치 -> 릴레이가 12줄이므로 위치를 계산해야 함
    int channel, rchannel;
    for(int index = 0; index < MAXCHANNEL / 2; index++){
        channel = GetChMap(stage, traypos, index) - 1;

        pvolt[channel]->Caption = IntToStr(channel + 1);

        int ch = BaseForm->nForm[stage]->chReverseMap[channel + 1];
        pcurr[channel]->Caption = SetChannelHint(ch);
        pcurr[channel]->Hint = "CH " + SetChannelHint(ch);
        pcurr[channel]->Refresh();

        if(channel % 4 == 0 || channel % 4 == 1) {
            pvolt[channel]->Color = pnormal3->Color;
        	pcurr[channel]->Color = pnormal31->Color;
        }
        else {
            pvolt[channel]->Color = pnormal4->Color;
        	pcurr[channel]->Color = pnormal41->Color;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::MakeUIPanel(AnsiString type)
{
	int nx, ny, nw, nh;
    int pUIy_nx;
    int pUIx_ny;

    nw = pnl_nw;
	nh = pnl_nh + 1;

    clir->Width = nw - 1;
	clocv->Width = nw - 1;
	clir->Height = nh/2;
	clocv->Height = nh/2;

	clir->Top = 2;
	clocv->Top = clir->Height + 4;

    if(type == "1"){
        ny = Panel2->Height - (nh) - 2;
        nx = nw + 4;

        for(int index = 0; index < LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, Panel35->Top, nw, nh, index);
            SetUIOption(pUIy[index], 2, ny-1, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx += (nw+1);
            if(index % 2 == 0) nx += 1;
            if(index % (LINECOUNT / 4) == 0) nx += 1;
            ny -= (nh+1);
            if( index % (LINECOUNT / 4) == 0) ny -= 2;
        }
    }
    else if(type == "2"){
        ny = Panel2->Height - (nh) - 2;
        nx = Panel2->Width - (nw + 2);

        for(int index = 0; index < LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, Panel35->Top, nw, nh, index);
            SetUIOption(pUIy[index], 2, ny-1, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx -= (nw+1);
            if(index % 2 == 0) nx -= 1;
            if(index % (LINECOUNT / 4) == 0) nx -= 1;
            ny -= (nh+1);
            if( index % (LINECOUNT / 4) == 0) ny -= 2;
        }
    }
    else if(type == "3"){
        ny = nh + 4;
        nx = nw + 4;
        for(int index = 0; index < LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, Panel35->Top, nw, nh, index);
            SetUIOption(pUIy[index], 2, ny, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx += (nw+1);
            if(index % 2 == 0) nx += 1;
            if(index % (LINECOUNT / 4) == 0) nx += 1;
            ny += (nh+1);
            if( index % (LINECOUNT / 4) == 0) ny += 2;
        }
    }
    else if(type == "4"){
        clir->Left = Panel2->Width - (clir->Width + 4);
        clocv->Left = Panel2->Width - (clocv->Width + 4);

        //* 가로열의 y값 - 가로는 nx가 바뀜
        pUIx_ny = Panel35->Top;
        //* 세로열의 x값 - 세로는 ny가 바뀜
        pUIy_nx = clir->Left;

        ny = nh + 5;
        nx = clir->Left - (clir->Width + 4);
        for(int index = 0; index < LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, pUIx_ny, nw, nh, index);
            SetUIOption(pUIy[index], pUIy_nx, ny, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx -= (nw+1);
            if(index % 2 == 0) nx -= 1;
            if(index % (LINECOUNT / 6) == 0) nx -= 1;
            ny += (nh+1);
            if( index % (LINECOUNT / 4) == 0) ny += 2;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::SetOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index)
{
	pnl->Parent = Panel2;
	pnl->Left =  nx;
	pnl->Top = ny;
	pnl->Width = nw;
	pnl->Height = nh;
	pnl->Alignment = taCenter;
	pnl->Font->Size = 8;
	pnl->Font->Color = clBlack;
	pnl->Font->Style = Font->Style << fsBold;
	pnl->Color = pnormal1->Color;
	pnl->ParentBackground = false;
	pnl->OnMouseEnter = ChInfoMouseEnter;
	pnl->OnMouseLeave = ChInfoMouseLeave;

	pnl->BevelInner = bvNone;
	pnl->BevelKind = bkNone;
	pnl->BevelOuter = bvNone;
	pnl->Tag = index;
	//pnl->Hint = "CH : " + IntToStr(index+1) + "(" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
	pnl->ShowHint = true;
	pnl->Caption = index+1;
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::SetUIOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index)
{
	pnl->Parent = Panel2;
	pnl->ParentBackground = false;
	pnl->Left = nx;
	pnl->Top = ny;
	pnl->Width = nw;
	pnl->Height = nh;
	pnl->Alignment = taCenter;
	pnl->Color = Panel35->Color;
	pnl->Caption = index+1;
	pnl->ShowCaption = true;
	pnl->Font->Size = 12;
	pnl->Font->Color = clBlack;
	pnl->Font->Style = Font->Style << fsBold;

	pnl->BevelInner = bvNone;
	pnl->BevelKind = bkNone;
	pnl->BevelOuter = bvRaised;
	pnl->BevelWidth =1;
	pnl->BiDiMode = bdLeftToRight;
	pnl->BorderStyle = bsNone;
	pnl->BorderWidth = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMeasureInfoForm::btnProbeClick(TObject *Sender)
{
    Mod_PLC->SetPcValue(PC_D_PRE_PROB_CLOSE, 1);
    Mod_PLC->SetPcValue(PC_D_PRE_PROB_OPEN, 0);
    probeTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnProbeOpenClick(TObject *Sender)
{
    Mod_PLC->SetPcValue(PC_D_PRE_PROB_CLOSE, 0);
    Mod_PLC->SetPcValue(PC_D_PRE_PROB_OPEN, 1);
    probeTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::ChInfoMouseEnter(TObject *Sender)
{
	TPanel *pnl;
	pnl = (TPanel*)Sender;
	int index;
	index = pnl->Tag;
	pch->Caption = index + 1;
    int ch = BaseForm->nForm[stage]->chReverseMap[index + 1];
    ppos->Caption = SetChannelHint(ch);
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::ChInfoMouseLeave(TObject *Sender)
{
	pch->Caption = "";
	ppos->Caption = "";
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::Panel35Click(TObject *Sender)
{
	pLocal->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::probeTimerTimer(TObject *Sender)
{
    if(Mod_PLC->GetPlcValue(PLC_D_PRE_PROB_CLOSE)){
        Mod_PLC->SetPcValue(PC_D_PRE_PROB_CLOSE, 0);
        probeTimer->Enabled = false;
    }
    if(Mod_PLC->GetPlcValue(PLC_D_PRE_PROB_OPEN)){
        Mod_PLC->SetPcValue(PC_D_PRE_PROB_OPEN, 0);
        probeTimer->Enabled = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnSaveClick(TObject *Sender)
{
    AnsiString str, FileName;
	int file_handle;

	if(SaveDialog->Execute()){
		FileName = SaveDialog->FileName;
		if(FileExists(FileName)){
			if(MessageBox(Handle, L"Would you like to overwrite files?", L"SAVE", MB_YESNO|MB_ICONQUESTION) == ID_NO){
				return;
			}
			else{
				DeleteFile(FileName);
			}
		}
		file_handle = FileCreate(FileName);
        FileSeek(file_handle, 0, 0);
		str = "No,Voltage,Current\n";
		for(int i = 0; i < MAXCHANNEL; ++i){
			str = str + IntToStr(i+1) + "," ;
			str = str + pvolt[i]->Caption + ",";//TotalForm->real_data.volt[i] + ","; //FormatFloat("0.0", TotalForm->real_data.volt[i]) + ",";
			str = str + pcurr[i]->Caption + "\n";//TotalForm->real_data.curr[i] + "\n"; //FormatFloat("0.0", TotalForm->real_data.curr[i]) + "\n";
		}
		FileWrite(file_handle, str.c_str(), str.Length());
		FileClose(file_handle);
		}
		else return;
}
//---------------------------------------------------------------------------

void __fastcall TMeasureInfoForm::btnInitClick(TObject *Sender)
{
    SetChannelInfo();
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::chkGraphClick(TObject *Sender)
{
    if(chkGraph->Checked){
		chartVoltage->Visible = true;
		chartVoltage->BringToFront();

		chartCurrent->Visible = true;
		chartCurrent->BringToFront();
	}
	else{
		chartVoltage->Visible = false;
        chartCurrent->Visible = false;
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// 컨트롤러 명령어
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnAutoClick(TObject *Sender)
{
	BaseForm->nForm[stage]->SetTrayID("");
	n_bManualStart = true;
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnStopClick(TObject *Sender)
{
	BaseForm->nForm[stage]->Timer_ManualInspection->Enabled = false;
    BaseForm->nForm[stage]->CmdStop();
//    if(BaseForm->WaitForMilliSeconds(1000) == true){
//        BaseForm->nForm[stage]->CmdAutoStop(BaseForm->nForm[stage]->nTrayPos);
//	}
    WaitForMilliSeconds(1000);
    BaseForm->nForm[stage]->CmdAutoStop(BaseForm->nForm[stage]->nTrayPos);

	if(BaseForm->nForm[stage]->Timer_Reset->Enabled == true)
		BaseForm->nForm[stage]->Timer_Reset->Enabled = false;

	if(BaseForm->nForm[stage]->Timer_Reboot->Enabled == true)
        BaseForm->nForm[stage]->Timer_Reboot->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnResetClick(TObject *Sender)
{
	if(MessageBox(Handle, Form_Language->msgRst.c_str(), L"", MB_YESNO|MB_ICONQUESTION) == ID_YES){

        BaseForm->nForm[stage]->CmdReset();
		Sleep(1000);
		BaseForm->nForm[stage]->CmdResetTimer();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnSetPrechargerClick(TObject *Sender)
{
    //SetStep();
    BaseForm->nForm[stage]->PreChargeSet();
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnRBTClick(TObject *Sender)
{
    if(MessageBox(Handle, Form_Language->msgRbt.c_str(), L"", MB_YESNO|MB_ICONQUESTION) == ID_YES){
        BaseForm->nForm[stage]->CmdReboot();
        Sleep(1000);
        BaseForm->nForm[stage]->CmdRebootTimer();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnReportClick(TObject *Sender)
{
    BaseForm->nForm[stage]->InitMeasureForm();
    BaseForm->nForm[stage]->CmdReport();
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnCALClick(TObject *Sender)
{
    BaseForm->nForm[stage]->CmdCAL();
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnSTCClick(TObject *Sender)
{
    BaseForm->nForm[stage]->CmdSTC();
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnENAClick(TObject *Sender)
{
    BaseForm->nForm[stage]->CmdEna();
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnDSTClick(TObject *Sender)
{
    BaseForm->nForm[stage]->CmdDST();
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnIDNClick(TObject *Sender)
{
    //* IDN
    BaseForm->nForm[stage]->CmdBID();
}
//---------------------------------------------------------------------------

void __fastcall TMeasureInfoForm::btnCLSClick(TObject *Sender)
{
    //* CLS (BT 메모리 데이터 삭제)
    BaseForm->nForm[stage]->CmdCLS();
}
//---------------------------------------------------------------------------

void __fastcall TMeasureInfoForm::btnCheckStepClick(TObject *Sender)
{
    BaseForm->nForm[stage]->CmdCheckStep(1);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// 컨트롤러 명령어
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnDisChargeSetClick(TObject *Sender)
{
    //* Discharge Setting
    BaseForm->nForm[stage]->SetSystemInfo();

    BaseForm->nForm[stage]->CmdDischargeSetStep();
    WaitForMilliSeconds(1000);
    BaseForm->nForm[stage]->CmdEna();
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::SetStep()
{
     nSetStep = 0;
     nSetCount = 0;
     MeasureInfoForm->Timer_SetStep2->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::Timer_SetStepTimer(TObject *Sender)
{
    nSetCount++;
    switch(nStep)
    {
        case 0:
            BaseForm->nForm[stage]->CmdSetStep();
            nStep = 99;
        	break;
        case 1:
            if(BaseForm->nForm[stage]->CmdCheckSet() == true)
                nStep = 2; // Ena
            else
                nStep = 0; // re-set
            break;
        case 2: // Ena
            BaseForm->nForm[stage]->CmdEna();
            break;
        case 3:
            //BaseForm->nForm[stage]->CmdResetTimer();
            //* error display
            Form_Error->DisplayErrorMessage(this->Tag, nResetErr);
            Form_Error->Tag = this->Tag;
            break;
        case 4:
            Timer_SetStep->Enabled = false;
            nStep = 0;
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::Timer_SetStep2Timer(TObject *Sender)
{
    nSetCount++;
    switch(nSetStep)
    {
        case 0:
            BaseForm->nForm[stage]->CmdSetStep();
            nStep = 99;
        	break;
        case 1:
            if(BaseForm->nForm[stage]->CmdCheckSet() == true){
                nSetStep = 2; // Ena
                nSetCount = 0;
            }

            if(nSetCount > 3){
                ShowMessage("PreCharger setting error! Please re-set it");//nSetStep = 0; // re-set
                nSetStep = 3;
            }
            break;
        case 2: // Ena
            BaseForm->nForm[stage]->CmdEna();
            break;
        case 3:
            //BaseForm->nForm[stage]->CmdResetTimer();
            Timer_SetStep2->Enabled = false;
            nSetStep = 0;
            break;
        case 4:
            Timer_SetStep2->Enabled = false;
            nSetStep = 0;
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnInit1Click(TObject *Sender)
{
    SetChannelInfo(1);
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnInit2Click(TObject *Sender)
{
    SetChannelInfo(2);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// CHART
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::initChart(int volt, int curr)
{
	for(int i=0; i<3; ++i){
		chartVoltage->Series[i]->Clear();
		chartCurrent->Series[i]->Clear();
	}

	for (int i = 0; i < MAXCHANNEL; ++i) {
		chartVoltage->Series[0]->AddXY(i + 1, 0);
		chartCurrent->Series[0]->AddXY(i + 1, 0);
	}

	chartVoltage->LeftAxis->Maximum = 4200;
	chartVoltage->LeftAxis->Minimum = 0;

	chartCurrent->LeftAxis->Maximum = curr + 500;
	chartCurrent->LeftAxis->Minimum = 0;

	chartVoltage->Series[1]->AddXY(1, volt - 200);
	chartVoltage->Series[1]->AddXY(400, volt - 200);
	chartCurrent->Series[1]->AddXY(1, curr);
	chartCurrent->Series[1]->AddXY(400, curr);

	chartVoltage->Series[2]->AddXY(1, volt - 2000);
	chartVoltage->Series[2]->AddXY(400, volt - 2000);
	chartCurrent->Series[2]->AddXY(1, curr - 1000);
	chartCurrent->Series[2]->AddXY(400, curr - 1000);
}
//---------------------------------------------------------------------------


void __fastcall TMeasureInfoForm::GroupBox3DblClick(TObject *Sender)
{
    if(GroupBox3->Height == 460) GroupBox3->Height = 230;
    else GroupBox3->Height = 460;
}
//---------------------------------------------------------------------------

