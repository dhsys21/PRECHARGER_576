//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Language *Form_Language;
//---------------------------------------------------------------------------
__fastcall TForm_Language::TForm_Language(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm_Language::LanguageChange(int LangIndex)
{
    TValueListEditor *vle;
    if(LangIndex == 0) vle = VLE_ENGLISH;
    else if(LangIndex == 1) vle = VLE_KOREAN;
    else if(LangIndex == 2) vle = VLE_CHINESE;

	//-------------------------------------------------------------------------
	// 				COMPONENT
	//-------------------------------------------------------------------------
	for(int i = 0; i < BaseForm->FormCnt; i++){
        BaseForm->nForm[i]->btnSaveConnConfig->Caption = vle->Values["SAVE"];
        BaseForm->nForm[i]->btnCloseConnConfig->Caption = vle->Values["CANCEL"];
        BaseForm->nForm[i]->btnConfig->Caption = vle->Values["CONFIG"];
        BaseForm->nForm[i]->btnManual->Caption = vle->Values["MANUAL"];
        BaseForm->nForm[i]->btnAuto->Caption = vle->Values["AUTO"];
        BaseForm->nForm[i]->btnInit->Caption = vle->Values["INIT"];
        BaseForm->nForm[i]->pnlTrayId->Caption = vle->Values["TRAYID"];
        BaseForm->nForm[i]->pnlEqStatus->Caption = vle->Values["STATUS"];
        BaseForm->nForm[i]->btnTrayOut->Caption = vle->Values["TRAYOUT"];
        BaseForm->nForm[i]->btnNgInfo->Caption = vle->Values["REMEAINFO"];
        BaseForm->nForm[i]->btnConnectPRECHARGER->Caption = vle->Values["CONNECT"];
        BaseForm->nForm[i]->btnDisconnPRECHARGER->Caption = vle->Values["DISCONNECT"];
        BaseForm->nForm[i]->btnConnectPLC->Caption = vle->Values["CONNECT"];
        BaseForm->nForm[i]->btnDisConnectPLC->Caption = vle->Values["DISCONNECT"];
        BaseForm->nForm[i]->ConfigBtn1->Caption = vle->Values["SET"];
        BaseForm->nForm[i]->lblConfiguration->Caption = vle->Values["CONFIGURATION"];
        BaseForm->nForm[i]->gbPreCharger->Caption = vle->Values["PRECHARGER"];
        BaseForm->nForm[i]->gbPlcConnection->Caption = vle->Values["PLCCONNECTION"];
        BaseForm->nForm[i]->gbEtc->Caption = vle->Values["ETC"];
        BaseForm->nForm[i]->lblProcess->Caption = vle->Values["PROCESSINFO"];
        BaseForm->nForm[i]->lblTrayInfo->Caption = vle->Values["TRAYINFO"];
        BaseForm->nForm[i]->lblChargingProcess->Caption = vle->Values["CHARGINGPROCESS"];
        BaseForm->nForm[i]->grbChInfo->Caption = vle->Values["CHINFO"];

        BaseForm->nForm[i]->cl_line->Caption = vle->Values["READY"];
        BaseForm->nForm[i]->cl_charge->Caption = vle->Values["CHARGING"];
        BaseForm->nForm[i]->cl_end->Caption = vle->Values["COMPLETE"];
        BaseForm->nForm[i]->cl_error->Caption = vle->Values["FAIL"];
        BaseForm->nForm[i]->cl_outflow->Caption = vle->Values["OUTFLOW"];
        BaseForm->nForm[i]->cl_no->Caption = vle->Values["NOCELL"];
    }

    MeasureInfoForm->btnProbeOpen->Caption = vle->Values["OPEN"];
    MeasureInfoForm->btnProbeClose->Caption = vle->Values["CLOSE"];
    MeasureInfoForm->btnSetPrecharger->Caption = vle->Values["SET"];
    MeasureInfoForm->btnReset->Caption = vle->Values["RESET"];
    MeasureInfoForm->btnAuto->Caption = vle->Values["START"];
	MeasureInfoForm->btnStop->Caption = vle->Values["STOP"];
    MeasureInfoForm->btnInit->Caption = vle->Values["INIT"];
    MeasureInfoForm->btnSave->Caption = vle->Values["SAVE"];
    MeasureInfoForm->grbChInfo->Caption = vle->Values["CHINFO"];
    MeasureInfoForm->pnlChannel->Caption = vle->Values["CHANNEL"];
    MeasureInfoForm->pnlPos->Caption = vle->Values["CHPOS"];


    //-------------------------------------------------------------------------
	// 				MESSAGE
	//-------------------------------------------------------------------------
	msgRbt = vle->Values["msgRBT"];
    msgRst = vle->Values["msgRST"];
    msgTooManyNG = vle->Values["msgTooManyNG"];
}
//---------------------------------------------------------------------------
