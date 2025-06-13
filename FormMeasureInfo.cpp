//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMeasureInfo.h"
#include "RVMO_main.h"
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
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::FormShow(TObject *Sender)
{
    this->BringToFront();
	pLocal->Visible = false;
    GroupBox3->Height = 230;
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

	if(type == "3" || type == "4")
	{
//		nx = Panel19->Left;
//		ny = Panel18->Top;
//		nw = Panel35->Width;
//		nh = Panel18->Height / 2;

		nw = Panel2->Width/22;
		nh = Panel2->Height/22/2;
		nx = Panel2->Width - (nw+2);
		ny = nh*2+5;

		for(int index=0; index<MAXCHANNEL;){
			pvolt[index] = new TPanel(this);
			pcurr[index] = new TPanel(this);

			SetOption(pvolt[index], nx, ny, nw, nh-1, index);
			SetOption(pcurr[index], nx, ny+nh, nw, nh, index);
			pcurr[index]->Caption = IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index+LINECOUNT)%LINECOUNT);
			pcurr[index]->Color = pnormal2->Color;
			pcurr[index]->ParentBackground = false;
			pvolt[index]->ParentBackground = false;

			index += 1;
			nx -= (nw + 1);
			if(index % 2 == 0) nx -= 2;
			if(index % (LINECOUNT / 2) == 0) nx -= 2;
			if(index % LINECOUNT == 0)
			{
				ny += (nh+1)*2+1;
				nx = Panel2->Width - (nw+2);
				if( (index / LINECOUNT) % (LINECOUNT / 2) == 0) ny += 2;
			}
		}
	}
    //* 왼쪽 아래가 1번. 오른쪽 방향으로 1 -> 20
	else if(type == "1")
	{
		nw = Panel35->Width*0.8;
		nh = Panel2->Height/22/2;
		nx = pUIx[0]->Left;
		//ny = Panel2->Height - nh*2 - 5;
		ny = pUIy[0]->Top;

		for(int index=0; index<MAXCHANNEL;){
			pvolt[index] = new TPanel(this);
			pcurr[index] = new TPanel(this);

			SetOption(pvolt[index], nx, ny, nw, nh-1, index);
			SetOption(pcurr[index], nx, ny+nh, nw, nh, index);
			pcurr[index]->Caption = IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index+LINECOUNT)%LINECOUNT);
			pcurr[index]->Color = pnormal2->Color;
			pcurr[index]->ParentBackground = false;
			pvolt[index]->ParentBackground = false;

			index += 1;
			nx = nx + (nw + 1);
			if(index % 2 == 0) nx += 1;
			if(index % (LINECOUNT / 2) == 0) nx += 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - nh - 2;
				nx = nx = pUIx[0]->Left;
				if( (index / LINECOUNT) % (LINECOUNT / 2) == 0) ny -= 2;
			}
		}
    }
    //* 오른쪽 아래가 1번. 왼쪽 방향으로 20 <- 1
    else if(type == "2")
	{
		nw = Panel35->Width*0.8;
		nh = Panel2->Height/22/2;
		nx = pUIx[0]->Left;
		ny = pUIy[0]->Top;

		for(int index=0; index<MAXCHANNEL;){
			pvolt[index] = new TPanel(this);
			pcurr[index] = new TPanel(this);

			SetOption(pvolt[index], nx, ny, nw, nh-1, index);
			SetOption(pcurr[index], nx, ny+nh, nw, nh, index);
			pcurr[index]->Caption = IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index+LINECOUNT)%LINECOUNT);
			pcurr[index]->Color = pnormal2->Color;
			pcurr[index]->ParentBackground = false;
			pvolt[index]->ParentBackground = false;

			index += 1;
			nx = nx - (nw + 1);
			if(index % 2 == 0) nx -= 1;
			if(index % (LINECOUNT / 2) == 0) nx -= 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - nh - 2;
				nx = pUIx[0]->Left;
				if( (index / LINECOUNT) % (LINECOUNT / 2) == 0) ny -= 2;
			}
		}
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::MakeUIPanel(AnsiString type)
{
	int nx, ny, nw, nh;

	//nw = Panel2->Width/22;
	//nh = Panel2->Height/22 + 1;
    nw = Panel35->Width*0.8;
	nh = Panel35->Height*0.8+1;

	clir->Width = nw;
	clocv->Width = nw;
	clir->Height = nh/2;
	clocv->Height = nh/2-1;

	clir->Top = 2;
	clocv->Top = clir->Height +4;

    clir->Width = nw;
	clocv->Width = nw;
	clir->Height = nh/2;
	clocv->Height = nh/2-1;

	clir->Top = 2;
	clocv->Top = clir->Height +4;

    if(type == "1"){
        ny = Panel2->Height - (nh) ;
        nx = nw + 5;

        for(int index=0; index<LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, Panel35->Top, nw, nh, index);
            SetUIOption(pUIy[index], Panel35->Width-nw-9, ny-1, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx += (nw+1);
            if(index % 2 == 0) nx += 1;
            if(index % (LINECOUNT / 2) == 0) nx += 1;
            ny -= (nh+1);
            if( index % (LINECOUNT / 2) == 0) ny -= 2;
        }
    }
    else if(type == "2"){
        ny = Panel2->Height - (nh) ;
        nx = Panel2->Width - (nw + 2);

        for(int index=0; index<LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, Panel35->Top, nw, nh, index);
            SetUIOption(pUIy[index], Panel35->Width-nw-9, ny-1, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx -= (nw+1);
            if(index % 2 == 0) nx -= 1;
            if(index % (LINECOUNT / 2) == 0) nx -= 1;
            ny -= (nh+1);
            if( index % (LINECOUNT / 2) == 0) ny -= 2;
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
//	pnl->Hint = "채널 : " + IntToStr(index+1) + "(" + IntToStr((index%16)+1) + "-" + IntToStr((index+16)/16)+ ")";
	pnl->Hint = "CH : " + IntToStr(index+1) + "(" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
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
//	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_CLOSE, 1);
//	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_OPEN, 0);
    BaseForm->nForm[stage]->SetPcValue(PC_D_PRE_PROB_CLOSE, 1);
    BaseForm->nForm[stage]->SetPcValue(PC_D_PRE_PROB_OPEN, 0);
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::btnProbeOpenClick(TObject *Sender)
{
//	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_OPEN, 1);
//	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_CLOSE, 0);
    BaseForm->nForm[stage]->SetPcValue(PC_D_PRE_PROB_CLOSE, 0);
    BaseForm->nForm[stage]->SetPcValue(PC_D_PRE_PROB_OPEN, 1);
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::ChInfoMouseEnter(TObject *Sender)
{
	TPanel *pnl;
	pnl = (TPanel*)Sender;
	int index;
	index = pnl->Tag;
	pch->Caption = index + 1;
	ppos->Caption = IntToStr((index+LINECOUNT)/LINECOUNT) + "-" + IntToStr((index%LINECOUNT)+1);
}
//---------------------------------------------------------------------------
void __fastcall TMeasureInfoForm::ChInfoMouseLeave(TObject *Sender)
{
	pch->Caption = "";
	ppos->Caption = "";
}
//---------------------------------------------------------------------------

void __fastcall TMeasureInfoForm::Panel19Click(TObject *Sender)
{
	for(int i=0; i<MAXCHANNEL; ++i){
		pvolt[i]->Caption = IntToStr(i+1);
		pcurr[i]->Caption = IntToStr((i+LINECOUNT)/LINECOUNT) + "-" + IntToStr((i%LINECOUNT)+1);
	}
}
void __fastcall TMeasureInfoForm::Panel35Click(TObject *Sender)
{
	pLocal->Visible = true;
}
//---------------------------------------------------------------------------




void __fastcall TMeasureInfoForm::probeTimerTimer(TObject *Sender)
{
//    if(Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_PRE_PROB_CLOSE))
//        Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_CLOSE, 0);
//    if(Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PC_D_PRE_PROB_OPEN))
//        Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_OPEN, 0);

    if(BaseForm->nForm[stage]->GetPlcValue(PLC_D_PRE_PROB_CLOSE))
        BaseForm->nForm[stage]->SetPcValue(PC_D_PRE_PROB_CLOSE, 0);
    if(BaseForm->nForm[stage]->GetPlcValue(PLC_D_PRE_PROB_OPEN))
        BaseForm->nForm[stage]->SetPcValue(PC_D_PRE_PROB_OPEN, 0);
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
		for(int i=0; i<MAXCHANNEL; ++i){
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
	for(int i=0; i<MAXCHANNEL; ++i){
		pvolt[i]->Caption = IntToStr(i+1);
		pvolt[i]->Color = pnormal1->Color;
		pcurr[i]->Caption = IntToStr((i+LINECOUNT)/LINECOUNT) + "-" + IntToStr((i%LINECOUNT)+1);
		pcurr[i]->Color = pnormal2->Color;
	}
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
    if(BaseForm->WaitForMilliSeconds(1000) == true){
        BaseForm->nForm[stage]->CmdAutoStop();
	}

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

void __fastcall TMeasureInfoForm::GroupBox3Click(TObject *Sender)
{
    if(GroupBox3->Height == 456) GroupBox3->Height = 230;
    else GroupBox3->Height = 456;
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

//---------------------------------------------------------------------------
// 컨트롤러 명령어
//---------------------------------------------------------------------------


void __fastcall TMeasureInfoForm::btnDisChargeSetClick(TObject *Sender)
{
    BaseForm->nForm[stage]->SetSystemInfo();

    BaseForm->nForm[stage]->CmdDischargeSetStep();
    BaseForm->WaitForMilliSeconds(1000);
    BaseForm->nForm[stage]->CmdEna();
}
//---------------------------------------------------------------------------

void __fastcall TMeasureInfoForm::SetStep()
{
     nStep = 0;
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
    switch(nStep)
    {
        case 0:
            BaseForm->nForm[stage]->CmdSetStep();
            nStep = 99;
        	break;
        case 1:
            if(BaseForm->nForm[stage]->CmdCheckSet() == true){
                nStep = 2; // Ena
                nSetCount = 0;
            }

            if(nSetCount > 3)
                nStep = 0; // re-set
            break;
        case 2: // Ena
            BaseForm->nForm[stage]->CmdEna();
            break;
        case 3:
            BaseForm->nForm[stage]->CmdResetTimer();
            break;
        case 4:
            Timer_SetStep2->Enabled = false;
            nStep = 0;
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------

