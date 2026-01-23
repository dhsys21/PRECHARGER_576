#include "FormTotal.h"
#include "RVMO_main.h"


void __fastcall TTotalForm::ShowAlarm(AnsiString err1, AnsiString err2, AnsiString err3 , AnsiString err4)
{
	if(GrpAlarm->Visible == false){
		modAlarm1->Caption = err1 + " " + err2;
	//	modAlarm2->Caption = err2;
		modAlarm3->Caption = err3;
		modAlarm4->Caption = err4;
		AlarmTime->Caption = Now().FormatString("hh : nn : ss");
		VisibleBox(GrpAlarm);
	}
}
// 알람 보이기
void __fastcall TTotalForm::ProcessError(AnsiString err1, AnsiString err2,AnsiString err3,AnsiString err4)
{
	if(GrpError->Visible == false){
		error1->Caption = err1 + " " + err2;
	//	error2->Caption = err2;
		error3->Caption = err3;
		error4->Caption = err4;
		ErrorTime->Caption = Now().FormatString("hh : nn : ss");
		ErrorLog();
		//VisibleBox(GrpError);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::DisplayProcess(int status, AnsiString Status_Step, AnsiString msg, bool bError)
{
	for(int i = 0; i < 8; i++)
		pProcess[i]->Color = clSilver;

	PLCStatus = msg;
	pProcess[status]->Color = p0->Color;
	Panel_State->Caption = PLCStatus;

	if(bError == true)
	{
        Panel_State->Color = clRed;
		Panel_State->Font->Color = clWhite;
	}
	else
	{
        Panel_State->Color = clWhite;
		Panel_State->Font->Color = clBlack;
    }

	if(OldPLCStatus != PLCStatus) {
		OldPLCStatus = PLCStatus;
		WritePLCLog(Status_Step, PLCStatus);
		WriteCommLog(Status_Step, PLCStatus);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::DisplayError(AnsiString msg, bool bError)
{
    Panel_State->Caption = msg;

	if(bError == true)
	{
        Panel_State->Color = clRed;
		Panel_State->Font->Color = clWhite;
	}
	else
	{
        Panel_State->Color = clWhite;
		Panel_State->Font->Color = clBlack;
    }
}
//---------------------------------------------------------------------------
// 메인화면 검사 진행 표시
void __fastcall TTotalForm::DisplayStatus(int status)
{
	TColor blue, white, font_clr;
	blue = clBlue;
	white = clWhite;
	font_clr = clBlack;

	AnsiString img_path;
	AnsiString err1, err2, err3, err4;
	err1 = "";
	err2 = "";
	err3 = "";
	err4 = "";

	if(GrpError->Visible == false){
		if( (stage.alarm_status < 20) && (status > 20) )BaseForm->IncErrorCount();
		else if( (stage.alarm_status > 20) && (status < 20) )BaseForm->DecErrorCount();
	}

	if(stage.arl == nAuto){
		stage.alarm_status = status;
	}
	else if (status <= nManual){
		stage.alarm_status = nManual;
		return;
	}

	stage.alarm_status = status;

	if(status >=0 && status <=12){
		StatusImage->Picture = BaseForm->statusImage[status]->Picture;
	}

	if(GrpError->Visible){
		GrpError->BringToFront();
	}
	else if(stage.arl == nAuto || status >= 23)VisibleBox(GrpMain);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::DisplayTrayInfo(int traypos)
{
	TColor clr;
	AnsiString sCurr, sVolt;
    int channel;
	for(int index = 0; index < CHANNELCOUNT; index++){
        channel = GetChMap(this->Tag, traypos, index) - 1;
		if(tray.cell[channel] == 1){
			clr = clLine;
			sCurr = "";
            sVolt = "";
		}
		else
		{
			clr = clNoCell;
			sCurr = "CELL";
			sVolt = "NO";

			m_sTempVlot[channel] = "No";
			m_sTempCurr[channel] = "Cell";
		}

		panel[channel]->Color = clr;
		MeasureInfoForm->pcurr[channel]->Color = clr;
		MeasureInfoForm->pvolt[channel]->Color = clr;
		MeasureInfoForm->pcurr[channel]->Caption = sCurr;
		MeasureInfoForm->pvolt[channel]->Caption = sVolt;
	}
    //pnlPos->Caption = "DisplayTrayInfo()";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ErrorMsg(int err)
{
	AnsiString err1, err2, err3, err4;
    int nstatus = 0;
//	bool ErrorMode = true;   // false : 알람모드 true: 에러모드
	switch(err){
		case nRunningError:
			nstatus = sCharge;
			 err1 = "RUNNING";
			 err2 = "";
			 err3 = "Lapse of Over 120 Seconds";
			 err4 = "Restart PRECHARGER";
			break;
		case nReadyError:
			nstatus = sReady;
			 err1 = "READY";
			 err2 = "";
			 err3 = "Lapse of Over 100 Seconds";
			 err4 = "Restart PRECHARGER";
			 break;
		case nRedEnd:
			nstatus = sTrayIn;
			 err1 = "TrayIn";
			 err2 = "";
			 err3 = "Lapse of Over 100 Seconds";
			 err4 = "Restart PRECHARGER";
			break;
		case nBlueEnd:
			nstatus = sFinish;
			 err1 = "END";
			 err2 = "";
			 err3 = "Lapse of Over 100 Seconds";
			 err4 = "Please Tray Out Manually and Initialize PRECHARGER";
			break;
		case nFinishError:
			nstatus = sTrayOut;
			 err1 = "FINISH";
			 err2 = "";
			 err3 = "Lapse of Over 100 Seconds";
			 err4 = "Please Tray Out Manually and Initialize PRECHARGER";
			break;
		case nDefaultError:
            nstatus = 0;
			 err1 = "STAGE Status";
			 err2 = "";
			 err3 = "Lapse of Over 100 Seconds";
			 err4 = "Check the status of STAGE";
			break;

	}
	stage.err = err;
	if(GrpError->Visible == false){
		error1->Caption = err1 +" " +  err2;
//		error2->Caption = err2;
		error3->Caption = err3;
		error4->Caption = err4;
		ErrorTime->Caption = Now().FormatString("hh : nn : ss");
		ErrorLog();
		//* 2023 06 14 설비가 멈췄을 경우 에러
		DisplayProcess(nstatus, err1, err3, true);
        //SetPcValue(PC_D_PRE_ERROR, 1);
		//VisibleBox(GrpError);
	}
}
//---------------------------------------------------------------------------
