﻿#pragma link "AdvSmoothButton"
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
#include "Util.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_PLCInterface *Form_PLCInterface;
//---------------------------------------------------------------------------
__fastcall TForm_PLCInterface::TForm_PLCInterface(TComponent* Owner)
	: TForm(Owner)
{
	nListView_PLC[0] = ListView_PLC;

	nListView_PC[0] = ListView_PC;

	//BaseForm->FormCnt
	for(int i = 0; i < 1; i++){
		SetListViewPLC(i);
		SetListViewPC(i);
	}

	for(int nIndex = 0; nIndex < 1; nIndex++){
		auto_manual_plc[nIndex] = 0;
		auto_manual_plc_old[nIndex] = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::SetListViewPLC(int nTag)
{
	AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_HEART_BEAT), "PLC HEART BEAT");
	AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_AUTO_MANUAL), "PLC MANUAL/AUTO");
	AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_ERROR), "PLC ERROR");

	AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_TRAY_IN), "TRAY IN");
	AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_PROB_OPEN), "PROB OPEN");
	AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_PROB_CLOSE), "PROB CLOSE");
    AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_TRAY_POS), "TRAY POS");
	AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_TRAY_ID), "IN TRAY BCR DATA");

	// CELL INFO => 1 : YES, 0 : NO
	for(int i = 0; i < LINECOUNT; i++)
	{
		AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_TRAY_CELL_DATA + (i * 2)), "TRAY CELL DATA #" + IntToStr(i + 1));
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::SetListViewPC(int nTag)
{
	// PC - CELL PP
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_HEART_BEAT), "PC HEART BEAT");
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_STAGE_AUTO_READY), "STAGE AUTO READY");
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_ERROR), "PC ERROR");

	// PC - PRE CHARGER
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_TRAY_OUT), "TRAY OUT");
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_PROB_OPEN), "PROB OPEN");
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_PROB_CLOSE), "PROB CLOSE");
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_CHARGING), "CHARGING");
    AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_COMPLETE1), "COMPLETE1");
    AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_COMPLETE2), "COMPLETE2");
    AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_TRAY_POS_MOVE), "TRAY POS MOVE");

	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_NG_COUNT), "PRECHARGER NG COUNT");
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_CURRENT_MIN), "CURRENT MIN. VALUE");
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_CHARGE_VOLTAGE), "CHARGE VOLTAGE");
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_CHARGE_CURRENT), "CHARGE CURRENT");
	AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_CHARGE_TIME), "CHARGE TIME");
    AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_NG_ALARM), "NG ALARM");

	// PRECHARGE RESULT OK/NG => 1 : NG, 0 : OK or NO CELL
	for(int i = 0; i < LINECOUNT; i++)
		AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_MEASURE_OK_NG + (i * 2)), "PRECHARGE OK/NG #" + IntToStr(i + 1));

	for(int i = 0; i < MAXCHANNEL; i++)
		AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_VOLTAGE_VALUE + i), "PRECHARGE VOLTAGE #" + IntToStr(i + 1));

	for(int i = 0; i < MAXCHANNEL; i++)
		AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_CURRENT + PC_D_PRE_CURRENT_VALUE + i), "PRECHARGE CURRENT #" + IntToStr(i + 1));
}//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::AddListView(TListView *list, AnsiString address, AnsiString name)
{
	tempListItem = list->Items->Add();
	tempListItem->Caption = address;
	tempListItem->SubItems->Add(name);
	tempListItem->SubItems->Add("");
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::FormShow(TObject *Sender)
{
	Timer_Update->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer_Update->Enabled = false;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::Timer_UpdateTimer(TObject *Sender)
{
	int index;
	if(Mod_PLC->ClientSocket_PLC->Active)	//	PLC 상태
	{
		for(int nTag = 0; nTag < BaseForm->FormCnt; nTag++){
			index = 0;
			nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_HEART_BEAT);
			nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_AUTO_MANUAL);
			nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_PRE_ERROR);

			nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_PRE_TRAY_IN);
			nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_PRE_PROB_OPEN);
			nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_PRE_PROB_CLOSE);
            nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_PRE_TRAY_POS);
			nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetString(Mod_PLC->plc_Interface_Data, PLC_D_PRE_TRAY_ID, 10);

			AnsiString cellinfo;
			for(int i = 0; i < LINECOUNT; i++)
			{
				cellinfo = "";
				for(int j = 0; j < LINECOUNT; j++)
				{
					cellinfo += Mod_PLC->GetData(Mod_PLC->plc_Interface_Data, PLC_D_PRE_TRAY_CELL_DATA + (i * 2), j);
				}
				nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = cellinfo;
			}
		}
	}

	if(Mod_PLC->ClientSocket_PC->Active)	//	PC 상태
	{
		for(int nTag =0; nTag < BaseForm->FormCnt; nTag++)
		{
			index = 0;
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_HEART_BEAT);
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_STAGE_AUTO_READY);
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_ERROR);

			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_TRAY_OUT);
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_OPEN);
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_PROB_CLOSE);
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CHARGING);
            nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_COMPLETE1);
            nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_COMPLETE2);
            nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_TRAY_POS_MOVE);

			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_NG_COUNT);
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CURRENT_MIN);
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CHARGE_VOLTAGE);
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CHARGE_CURRENT);
			nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_CHARGE_TIME);
            nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_NG_ALARM);
			// PRECHARGE RESULT OK/NG
			AnsiString okng_bin;
			for(int i = 0; i < LINECOUNT; i++)
			{
				okng_bin = "";
				for(int j = 0; j < LINECOUNT; j++)
					okng_bin += Mod_PLC->GetData(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + (i * 2), j);

				nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = okng_bin;
			}

			double vol1 = 0.0, vol2 = 0.0;
			// PRECHARGE RESULT VOLTAGE VALUE
			for(int i = 0; i < MAXCHANNEL; i++)
			{
				vol1 = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_VOLTAGE_VALUE + i);
                nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = vol1;
			}

			// PRECHARGE RESULT CURRENT VALUE
			for(int i = 0; i < MAXCHANNEL; i++)
			{
				nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Curr_Data, PC_D_PRE_CURRENT_VALUE + i);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::GroupBox2Click(TObject *Sender)
{
    Panel1->Visible = !Panel1->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::btnWriteValueClick(TObject *Sender)
{
	int address = cbAddress->Text.ToIntDef(41000);
	int value = editValue->Text.ToIntDef(1);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, address - 41000, value);
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::btnWriteNgValueClick(TObject *Sender)
{
    AnsiString strIrocvNg = editIrOcvNg->Text;
	vector<int> ngchannels = BaseForm->StringToVector(strIrocvNg);

    int ngCount = 0;
	for(int i = 0; i < LINECOUNT; ++i){
		for(int j = 0; j < LINECOUNT; j++)
		{
			int nChannel = i * LINECOUNT + j + 1;
			if(find(ngchannels.begin(), ngchannels.end(), nChannel) != ngchannels.end())
			{
				Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + i, j, true);
				ngCount++;
			}
			else
			{
				Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + i, j, false);
			}
		}
	}

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_NG_COUNT, ngCount);
}
//---------------------------------------------------------------------------

void __fastcall TForm_PLCInterface::btnWriteIrOcvValueClick(TObject *Sender)
{
	double voltage_base = StringToDouble(editVoltage->Text, 1);
	double current_base = StringToDouble(editCurrent->Text, 1);

	// ir value 2 Word
	// 2 Word :  value / (65536 / 2) => 윗 주소에 쓰기, value % (65536 /2 ) => 아래 주소에 쓰기 // herald 2017 11 30
	for(int i = 0; i < MAXCHANNEL; i++)
	{
		double voltage = voltage_base + (i / 100.0);
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_VOLTAGE_VALUE + i, FormatFloat("0000", voltage * 10.0) % (256 * 256));
	}

	for(int i = 0; i < MAXCHANNEL; i++)
	{
		double current = current_base + (i / 10.0);
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Curr_Data, PC_D_PRE_CURRENT_VALUE + i, FormatFloat("00000", current * 10.0) % (256 * 256));
	}
}
//---------------------------------------------------------------------------

