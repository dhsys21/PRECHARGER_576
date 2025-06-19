#include "FormTotal.h"
#include "RVMO_main.h"
#include "Util.h"
//---------------------------------------------------------------------------
// 환경설정 파일 저장 / 읽기
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteSystemInfo()
{
	TIniFile *ini;

	AnsiString file;
	file = (AnsiString)BIN_PATH + "SystemInfo_"+ IntToStr(this->Tag) + ".inf";
	ini = new TIniFile(file);

	ini->WriteString("PRECHARGE", "MAX_VOLTAGE", editMaxChargeVolt->Text);
	ini->WriteString("PRECHARGE", "MAX_CURRENT", editMaxChargeCurrent->Text);
	ini->WriteString("PRECHARGE", "MAX_TIME", editMaxChargeTime->Text);

	ini->WriteString("PRECHARGE", "VOLTAGE", editChargeVolt->Text);
	ini->WriteString("PRECHARGE", "CURRENT", editChargeCurrent->Text);
	ini->WriteString("PRECHARGE", "TIME", editChargeTime->Text);

	ini->WriteString("MINCURRENT", "CURRENT", editCurrMin->Text);
	ini->WriteString("NG_ALARM_COUNT", "COUNT", editNGAlarmCount->Text);

	ini->WriteInteger("MAIN", "REMEASURE_ALARM_COUNT", editRemeasureAlarmCount->Text.ToIntDef(3));
	config.remeasure_alarm_cnt = editRemeasureAlarmCount->Text.ToIntDef(3);
	RemeasureForm->pcolor2->Caption = config.remeasure_alarm_cnt;

	ini->WriteString("PRECHARGER_PLC", "IP", editPLCIPAddress->Text);
	ini->WriteString("PRECHARGER_PLC", "PORT1", editPLCPortPC->Text);
	ini->WriteString("PRECHARGER_PLC", "PORT2", editPLCPortPLC->Text);

	ini->WriteString("PRECHARGER", "IP", editPRECHARGERIPAddress->Text);
	ini->WriteString("PRECHARGER", "PORT", editPRECHARGERPort->Text);

	delete ini;
}
//---------------------------------------------------------------------------
bool __fastcall TTotalForm::ReadSystemInfo()
{
	TIniFile *ini;

	AnsiString file;
	file = (AnsiString)BIN_PATH + "SystemInfo_"+ IntToStr(this->Tag) + ".inf";

	ini = new TIniFile(file);
    config.remeasure_alarm_cnt = ini->ReadInteger("MAIN", "REMEASURE_ALARM_COUNT", 3);

	editMaxChargeVolt->Text = ini->ReadString("PRECHARGE", "MAX_VOLTAGE", "4200");
	editMaxChargeCurrent->Text = ini->ReadString("PRECHARGE", "MAX_CURRENT", "2500");
	editMaxChargeTime->Text = ini->ReadString("PRECHARGE", "MAX_TIME", "300");

	editChargeVolt->Text = ini->ReadString("PRECHARGE", "VOLTAGE", "4200");
	editChargeCurrent->Text = ini->ReadString("PRECHARGE", "CURRENT", "1000");
	editChargeTime->Text = ini->ReadString("PRECHARGE", "TIME", "60");

	config.volt = StringToDouble(editChargeVolt->Text, 4200);
	config.curr = StringToDouble(editChargeCurrent->Text, 1000);
    config.time = StringToInt(editChargeTime->Text, 60);

	editCurrMin->Text = ini->ReadString("MINCURRENT", "CURRENT", 100);

	editNGAlarmCount->Text = ini->ReadString("NG_ALARM_COUNT", "COUNT", "20");

	editPLCIPAddress->Text = ini->ReadString("PRECHARGER_PLC", "IP", "17.91.80.220");
	editPLCPortPC->Text = ini->ReadString("PRECHARGER_PLC", "PORT1", "5007");
	editPLCPortPLC->Text = ini->ReadString("PRECHARGER_PLC", "PORT2", "5008");

    PLC_IPADDRESS = editPLCIPAddress->Text;
	PLC_PCPORT = editPLCPortPC->Text.ToIntDef(5007);
	PLC_PLCPORT = editPLCPortPLC->Text.ToIntDef(5008);

	editPRECHARGERIPAddress->Text = ini->ReadString("PRECHARGER", "IP", "192.168.10.231");
	editPRECHARGERPort->Text = ini->ReadString("PRECHARGER", "PORT", "50000");

    if(Client->Host != editPRECHARGERIPAddress->Text
    	|| Client->Port != editPRECHARGERPort->Text.ToIntDef(50000)){
        Client->Active = false;
        Client->Host = editPRECHARGERIPAddress->Text;
        Client->Port = editPRECHARGERPort->Text.ToIntDef(50000);
    }

    //btnConnectPRECHARGERClick(this);

	delete ini;
}
//---------------------------------------------------------------------------
bool __fastcall TTotalForm::ReadCellInfo()
{
	TIniFile *ini;

	AnsiString file;
	file = (AnsiString)BIN_PATH + "SystemInfo_"+ IntToStr(this->Tag) + ".inf";

	ini = new TIniFile(file);

	tray.cell_model = ini->ReadString("CELL_INFO", "CELL_MODEL", "-");
	tray.lot_number = ini->ReadString("CELL_INFO", "LOT_NUMBER", "-");

	delete ini;
}
//---------------------------------------------------------------------------
bool __fastcall TTotalForm::LoadTrayInfo(AnsiString trayID)
{
	AnsiString filename;
	filename = (UnicodeString)TRAY_PATH2 + trayID + ".Tray";
    WritePLCLog("Read CELL SERIAL Filename", filename);
	if(FileExists(filename)){
		TIniFile *ini;

		ini = new TIniFile(filename);
		for(int i = 0; i < MAXCHANNEL; i++)
		{
			tray.cell_serial[i] = ini->ReadString(i + 1, "CELL_SERIAL", "-");
		}

		delete ini;
	}
	else return false;

	return true;
}
//---------------------------------------------------------------------------
// 환경설정 파일 저장 / 읽기
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// 로그
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteCommLog(AnsiString Type, AnsiString Msg)
{
	AnsiString str, dir;
	int file_handle;

	dir = (AnsiString)LOG_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	str = dir + "STAGE" + FormatFloat("000", this->Tag+1) + "_" + Now().FormatString("yymmdd-hh") + ".log";

	if(FileExists(str))
		file_handle = FileOpen(str, fmOpenWrite);
	else{
		file_handle = FileCreate(str);
	}

	FileSeek(file_handle, 0, 2);

	if(Msg.Pos("2SENIDL") < 1 || Msg.Pos("1SEN001") < 1){
		str = Now().FormatString("yyyy-mm-dd hh:nn:ss.zzz> ") + "[" + IntToStr(nSection) + ", "
			+ IntToStr(nStep) + "] " + Type +  "\t" + Msg + "\n";
		FileWrite(file_handle, str.c_str(), str.Length());
	}

	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WritePLCLog(AnsiString Type, AnsiString Msg)
{
	AnsiString str, dir;
	int file_handle;

	dir = (AnsiString)LOG_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	str = dir + "STAGE" + FormatFloat("000", this->Tag+1) + "_PLC_" + Now().FormatString("yymmdd-hh") + ".log";

	if(FileExists(str))
		file_handle = FileOpen(str, fmOpenWrite);
	else{
		file_handle = FileCreate(str);
	}

	FileSeek(file_handle, 0, 2);

	str = Now().FormatString("yyyy-mm-dd hh:nn:ss ") +Type +  "\t" + Msg + "\n";
	FileWrite(file_handle, str.c_str(), str.Length());

	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ErrorLog()
{
    AnsiString str, dir;
	int file_handle;

	dir = (AnsiString)LOG_PATH + Now().FormatString("yyyymmdd") + "\\";
    ForceDirectories((AnsiString)dir);

	str = dir + "ERROR" + FormatFloat("000", this->Tag+1) + "_" + Now().FormatString("yymmdd-hh") + ".log";

	if(FileExists(str))
		file_handle = FileOpen(str, fmOpenWrite);
	else{
		file_handle = FileCreate(str);
	}

	FileSeek(file_handle, 0, 2);

	str = Now().FormatString("yyyy-mm-dd hh:nn:ss> ") + error1->Caption + ", " + error3->Caption + ", " + error4->Caption + "\n";
	FileWrite(file_handle, str.c_str(), str.Length());

	FileClose(file_handle);
}
//---------------------------------------------------------------------------
// 로그
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// Tray Info저장, Mon Data 저장, Final Result 저장
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteTrayInfo()
{
	int file_handle;
	AnsiString filename;
	AnsiString dir;
	AnsiString ok_ng;

	dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\" + lblTitle->Caption + "\\";
	ForceDirectories((AnsiString)dir);

	filename =  dir + tray.trayid +  "-" + Now().FormatString("yymmddhhnnss") + "-TRAYINFO.csv";

	if(FileExists(filename)){
		DeleteFile(filename);
	}

	file_handle = FileCreate(filename);
	FileSeek(file_handle, 0, 0);

	AnsiString file;
//	file = "TRAY ID," + tray.trayid + "\r\n";
//	file += "CH,OK/NG\r\n";
	file = "CELL 유무 (1 : OK/ 0 : NG)\r\n";
	for(int i = 0; i < MAXCHANNEL; ++i)
	{
		if(tray.cell[i] == 1) ok_ng = "1";
		else ok_ng = "0";
		file = file + IntToStr(i+1) + "," + ok_ng + "\r\n";
	}

	FileWrite(file_handle, file.c_str(), file.Length());
	FileClose(file_handle);
}

//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteResultFile(int traypos)
{
	int file_handle;
	AnsiString filename;
	AnsiString dir;
	AnsiString cell, cell_id, volt, curr, ok_ng;

	dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\" + lblTitle->Caption + "\\";
	ForceDirectories((AnsiString)dir);

	filename =  dir + tray.trayid +  "-" + Now().FormatString("yymmddhhnnss") + "_TP" + IntToStr(traypos) + ".csv";

	if(FileExists(filename)){
		DeleteFile(filename);
	}

	file_handle = FileCreate(filename);
	FileSeek(file_handle, 0, 0);

	AnsiString file;
	file = "TRAY ID," + tray.trayid + "\r\n";
    file = "TRAY POSITION," + IntToStr(traypos) + "\r\n";
//	file = file + "CELL MODEL," + tray.cell_model + "\r\n";
//	file = file + "LOT NUMBER," + tray.lot_number + "\r\n";
	file = file + "ARRIVE TIME," + m_dateTime.FormatString("yyyy/mm/dd hh:nn:ss") + "\r\n";
	file = file + "FINISH TIME," + Now().FormatString("yyyy/mm/dd hh:nn:ss") + "\r\n";
	file = file + "VOLTAGE," + FormatFloat("0", config.volt) + "\r\n";
	file = file + "CURRENT," + FormatFloat("0", config.curr) + "\r\n";
	file = file + "TIME," + FormatFloat("0", config.time) + "\r\n";

	file += "CH,CELL,CELL ID,VOLT,CURR,RESULT\r\n";

    int channel;
	for(int i = 0; i < MAXCHANNEL; ++i){
        channel = chMap[(nTrayPos - 1) * (MAXCHANNEL / 2) + i] - 1;
        cell_id = tray.cell_serial[channel];
		volt = real_data.final_volt[channel];
		curr = real_data.final_curr[channel];

		if(tray.cell[channel] == 1)
		{
			if(tray.measure_result[channel] == 0) ok_ng = "OK";
			else ok_ng = "NG";

			cell = "O";
		}
		else if(tray.cell[channel] == 0)
		{
			if(tray.measure_result[channel] == 0) ok_ng = "OUT FLOW";
			else ok_ng = "NG (No Cell)";

			cell = "X";
		}

		file = file + IntToStr(channel + 1) + "," + cell + "," + cell_id + "," + volt + "," + curr + "," + ok_ng + "\r\n";
	}

	FileWrite(file_handle, file.c_str(), file.Length());
	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteMonData(int traypos)
{
    AnsiString dir, filename;
	int file_handle;
    int channel;

	dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\" + lblTitle->Caption + "\\";
	ForceDirectories((AnsiString)dir);

	filename =  dir + tray.trayid +  "_MONTP" + IntToStr(traypos) + ".csv";
    AnsiString file = "";

	if(FileExists(filename))
		file_handle = FileOpen(filename, fmOpenWrite);
	else{
		file_handle = FileCreate(filename);

    file = "DATETIME,RUNCOUNT";
    for(int nIndex = 0; nIndex < MAXCHANNEL / 2; nIndex++)
        channel = chMap[(nTrayPos - 1) * (MAXCHANNEL / 2) + nIndex];
        file += ",CH" + IntToStr(channel) + " STATUS,CH" + IntToStr(channel) + " VOLTAGE,CH" + IntToStr(channel) + " CURRENT";
    	file += "\r\n";
	}

	FileSeek(file_handle, 0, 2);

    file += Now().FormatString("yyyy/mm/dd hh:nn:ss.zzz") + "," + stage.runcount;
	for(int i = 0; i < MAXCHANNEL / 2; ++i){
        channel = chMap[(nTrayPos - 1) * (MAXCHANNEL / 2) + i] - 1;
        file += "," + IntToStr(real_data.status[channel]);
        file += "," + FormatFloat("0.0", StringToDouble(real_data.volt[channel], 0.0));
        file += "," + FormatFloat("0.0", StringToDouble(real_data.curr[channel], 0.0));
	}
    file += "\r\n";

    FileWrite(file_handle, file.c_str(), file.Length());
	FileClose(file_handle);
}
//---------------------------------------------------------------------------
// Tray Info저장, Mon Data 저장, Final Result 저장
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// 재측정 정보 읽고 쓰기
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ReadRemeasureInfo()
{
	TIniFile *ini;

	ini = new TIniFile((AnsiString)BIN_PATH + "RemeasureInfo.inf");

	AnsiString retest_info;
	AnsiString title = "REMEASURE" + IntToStr(this->Tag);

	int pos = 0;

	retest_info = ini->ReadString(title, "ACCMULATE", "-1");
    int nRemeasureAlarmCount = 0;
	config.remeasure_alarm_cnt = ini->ReadInteger("MAIN", "REMEASURE_ALARM_COUNT", 3);
	editRemeasureAlarmCount->Text = config.remeasure_alarm_cnt;
	RemeasureForm->pcolor2->Caption = config.remeasure_alarm_cnt;

	if(retest_info == "-1"){	// 파일이 존재하지 않으면
		for(int index=0; index<MAXCHANNEL; ++index){
			acc_remeasure[index] = 0; 	// 모두 0으로
		}
	}
	else{
		for(int index=0; index<MAXCHANNEL; ++index){
			pos = retest_info.Pos("_");
			acc_remeasure[index] = retest_info.SubString(1,pos-1).ToIntDef(0);
            if(acc_remeasure[index] >= config.remeasure_alarm_cnt)
				nRemeasureAlarmCount++;
			retest_info.Delete(1, pos);
		}
	}
    RemeasureAlarm(nRemeasureAlarmCount);

	retest_info = "";
	acc_init = 	 ini->ReadString(title, "ACCMULATE_DAY", Now().FormatString("yyyy. m. d. hh:nn"));
	acc_cnt = ini->ReadInteger(title, "ACC_CNT", 0);

	delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TTotalForm::WriteRemeasureInfo()	// Tray가 Vacancy 상태일때 기록
{
	TIniFile *ini;

	ini = new TIniFile((AnsiString)BIN_PATH + "RemeasureInfo.inf");

	AnsiString retest_info;
	AnsiString title = "REMEASURE" + IntToStr(this->Tag);
	retest_info = "";
	int nRemeasureAlarmCount = 0;
	config.remeasure_alarm_cnt = StringToInt(RemeasureForm->pcolor2->Caption, 3);

	retest_info = "";
	for(int index=0; index<MAXCHANNEL; ++index){
		retest_info =  retest_info + acc_remeasure[index] + "_";

		if(acc_remeasure[index] >= config.remeasure_alarm_cnt)
			nRemeasureAlarmCount++;
	}
    RemeasureAlarm(nRemeasureAlarmCount);

	ini->WriteInteger(title, "REMEASURE_ALARM_COUNT", editRemeasureAlarmCount->Text.ToIntDef(3));
	ini->WriteString(title, "ACCMULATE", retest_info);
	ini->WriteString(title, "ACCMULATE_DAY", acc_init);
	ini->WriteInteger(title, "ACC_CNT", acc_cnt);


	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::RemeasureAlarm(int remeasure_alarm_count)
{
	if(remeasure_alarm_count > 0) {
		//Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  PC_D_PRE_NG_ALARM, 1);
        SetPcValue(PC_D_PRE_NG_ALARM, 1);
		btnNgInfo->Color = clRed;
		lblRemeasureAlarmCheck->Visible = true;
	}
	else{
        //Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  PC_D_PRE_NG_ALARM, 0);
        SetPcValue(PC_D_PRE_NG_ALARM, 0);
		btnNgInfo->Color = clWhite;
		lblRemeasureAlarmCheck->Visible = false;
    }
}
//---------------------------------------------------------------------------
// 재측정 정보 읽고 쓰기
//---------------------------------------------------------------------------
