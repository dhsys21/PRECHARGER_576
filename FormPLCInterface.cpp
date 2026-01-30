#pragma link "AdvSmoothPanel"
#pragma link "AdvSmoothButton"
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
    const int count = chkAllData->Checked ? MAXCHANNEL : LINECOUNT;

    ListView_PLC->Items->BeginUpdate();
    try
    {
        ListView_PLC->Items->Clear();

        AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_HEART_BEAT), "PLC HEART BEAT");
        AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_AUTO_MANUAL), "PLC MANUAL/AUTO");
        AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_ERROR), "PLC ERROR");

        AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_TRAY_IN), "TRAY IN");
        AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_PROB_OPEN), "PROB OPEN");
        AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_PROB_CLOSE), "PROB CLOSE");
        AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_TRAY_POS), "TRAY POS");
        AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_PLC_AUTOMODE), "PLC AUTO MODE");
        AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_TRAY_ID), "IN TRAY BCR DATA");

        // CELL INFO => 1 : YES, 0 : NO
        //* 16(bit)개씩 36개
        for(int i = 0; i < 36; i++)
        {
            AddListView(nListView_PLC[nTag], "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_PRE_TRAY_CELL_DATA + i), "TRAY CELL DATA #" + IntToStr(i + 1));
        }

        if(chkAllData->Checked == false){
            for(int i = 0; i < LINECOUNT; i++)
               AddListView(ListView_PLC, "D" + IntToStr(PLC_D_CELL_SERIAL_NUM + PLC_D_PRE_CELL_SERIAL + (i * 10 * LINECOUNT)), "CELL SERIAL #" + IntToStr(i * LINECOUNT + 1));
        }else {
            for(int i = 0; i < MAXCHANNEL; i++)
               AddListView(ListView_PLC, "D" + IntToStr(PLC_D_CELL_SERIAL_NUM + PLC_D_PRE_CELL_SERIAL + (i * 10)), "CELL SERIAL #" + IntToStr(i + 1));
        }
    }
    __finally
    {
        ListView_PLC->Items->EndUpdate();
    }

    // 보통 EndUpdate로 충분하지만, 즉시 화면 반영이 필요하면:
    ListView_PLC->Invalidate();
    ListView_PLC->Update();
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::SetListViewPC(int nTag)
{
    const int count = chkAllData->Checked ? MAXCHANNEL : LINECOUNT;

    ListView_PC->Items->BeginUpdate();
    try
    {
        ListView_PC->Items->Clear();

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
        AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_DATA_WRITE), "DATA WRITE COMPLETE");

        // PRECHARGE RESULT OK/NG => 1 : NG, 0 : OK or NO CELL
        //* 16 bit * 36
        for(int i = 0; i < 36; i++)
            AddListView(nListView_PC[nTag], "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM + PC_D_PRE_MEASURE_OK_NG + i), "PRECHARGE OK/NG #" + IntToStr(i + 1));

        if(chkAllData->Checked == false){
            for(int i = 0; i < LINECOUNT; i++)
                AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_VOLTAGE + PC_D_PRE_VOLTAGE_VALUE + i * LINECOUNT), "VOLT VALUE #" + IntToStr(i * LINECOUNT + 1));

            for(int i = 0; i < LINECOUNT; i++)
                AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_CURRENT + PC_D_PRE_CURRENT_VALUE + i  * LINECOUNT), "CURR VALUE #" + IntToStr(i * LINECOUNT + 1));
        } else {
            for(int i = 0; i < MAXCHANNEL; i++)
                AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_VOLTAGE + PC_D_PRE_VOLTAGE_VALUE + i), "VOLT VALUE #" + IntToStr(i + 1));

            for(int i = 0; i < MAXCHANNEL; i++)
                AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_CURRENT + PC_D_PRE_CURRENT_VALUE + i), "CURR VALUE #" + IntToStr(i + 1));
        }
    }
    __finally
    {
        ListView_PC->Items->EndUpdate();
    }

    // 보통 EndUpdate로 충분하지만, 즉시 화면 반영이 필요하면:
    ListView_PC->Invalidate();
    ListView_PC->Update();
}
//---------------------------------------------------------------------------

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
            nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_PRE_PLC_AUTOMODE);
			nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetString(Mod_PLC->plc_Interface_Data, PLC_D_PRE_TRAY_ID, 10);

			AnsiString cellinfo;
            //* 16bit * 36
			for(int i = 0; i < 36; i++)
			{
				cellinfo = "";
				for(int j = 0; j < 16; j++)
				{
					cellinfo += Mod_PLC->GetData(Mod_PLC->plc_Interface_Data, PLC_D_PRE_TRAY_CELL_DATA + i, j);
				}
				nListView_PLC[nTag]->Items->Item[index++]->SubItems->Strings[1] = cellinfo;
			}

            if(chkAllData->Checked == false) {
                for(int i = 0; i < LINECOUNT; i++)
                    ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetCellSrial(PLC_D_PRE_CELL_SERIAL, i * LINECOUNT, 10);
            } else {
                for(int i = 0; i < MAXCHANNEL; i++)
                    ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetCellSrial(PLC_D_PRE_CELL_SERIAL, i, 10);
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
            nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_DATA_WRITE);
			// PRECHARGE RESULT OK/NG
			AnsiString okng_bin;
            //* 16 bit * 36
			for(int i = 0; i < 36; i++)
			{
				okng_bin = "";
				for(int j = 0; j < 16; j++)
					okng_bin += Mod_PLC->GetData(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + i, j);

				nListView_PC[nTag]->Items->Item[index++]->SubItems->Strings[1] = okng_bin;
			}

            if(chkAllData->Checked == false) {
                // PRECHARGE RESULT VOLTAGE VALUE
                for(int i = 0; i < LINECOUNT; i++)
                    ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetVoltValue(PC_D_PRE_VOLTAGE_VALUE, i * LINECOUNT);


                // PRECHARGE RESULT CURRENT VALUE
                for(int i = 0; i < LINECOUNT; i++)
                    ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetCurrValue(PC_D_PRE_CURRENT_VALUE, i * LINECOUNT);
            } else {
                // PRECHARGE RESULT VOLTAGE VALUE
                for(int i = 0; i < MAXCHANNEL; i++)
                    ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetVoltValue(PC_D_PRE_VOLTAGE_VALUE, i);


                // PRECHARGE RESULT CURRENT VALUE
                for(int i = 0; i < MAXCHANNEL; i++)
                    ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetCurrValue(PC_D_PRE_CURRENT_VALUE, i);
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
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, address - 34000, value);
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::btnWriteNgValueClick(TObject *Sender)
{
    AnsiString strIrocvNg = editIrOcvNg->Text;
	vector<int> ngchannels = BaseForm->StringToVector(strIrocvNg);

    int ngCount = 0;
    //* 16 bit * 36
	for(int i = 0; i < 36; ++i){
		for(int j = 0; j < 16; j++)
		{
			int nChannel = i * 16 + j + 1;
			if(find(ngchannels.begin(), ngchannels.end(), nChannel) != ngchannels.end())
			{
				//Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + i, j, true);
                Mod_PLC->SetPcData(PC_D_PRE_MEASURE_OK_NG + i, j, true);
				ngCount++;
			}
			else
			{
				//Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_PRE_MEASURE_OK_NG + i, j, false);
                Mod_PLC->SetPcData(PC_D_PRE_MEASURE_OK_NG + i, j, false);
			}
		}
	}

	//Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_PRE_NG_COUNT, ngCount);
    Mod_PLC->SetPcValue(PC_D_PRE_NG_COUNT, ngCount);
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
		int32_t voltage_int = static_cast<int32_t>(voltage_base * 10.0) + i;  // signed 32-bit int
        Mod_PLC->SetVoltValue(PC_D_PRE_VOLTAGE_VALUE, i, voltage_int);
	}

	for(int i = 0; i < MAXCHANNEL; i++)
	{
		double current = static_cast<int32_t>(current_base * 10.0) + i;
		Mod_PLC->SetCurrValue(PC_D_PRE_CURRENT_VALUE, i, current);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_PLCInterface::chkAllDataClick(TObject *Sender)
{
    //BaseForm->FormCnt
	for(int i = 0; i < 1; i++){
		SetListViewPLC(i);
		SetListViewPC(i);
	}
}
//---------------------------------------------------------------------------

