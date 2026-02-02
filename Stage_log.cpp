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

    if(StringToDouble(editMaxChargeVolt->Text, 0) > 4200) editMaxChargeVolt->Text = 4200;
    if(StringToDouble(editMaxChargeCurrent->Text, 0) > 5000) editMaxChargeCurrent->Text = 5000;

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

    ini->WriteString("CELLINFO", "MODELNAME", editModelName->Text);
    ini->WriteString("PASSWORD", "PWD", editPwd->Text);

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

    editModelName->Text = ini->ReadString("CELLINFO", "MODELNAME", "20PQ");
    editPwd->Text = ini->ReadString("PASSWORD", "PWD", "0000");

	delete ini;
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

	str = dir + "ERROR" + FormatFloat("000", this->Tag+1) + "_" + Now().FormatString("yymmdd") + ".log";

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
void __fastcall TTotalForm::ErrorLog(AnsiString msg)
{
    AnsiString str, dir;
	int file_handle;

	dir = (AnsiString)LOG_PATH + Now().FormatString("yyyymmdd") + "\\";
    ForceDirectories((AnsiString)dir);

	str = dir + "ERROR" + FormatFloat("000", this->Tag+1) + "_" + Now().FormatString("yymmdd") + ".log";

	if(FileExists(str))
		file_handle = FileOpen(str, fmOpenWrite);
	else{
		file_handle = FileCreate(str);
	}

	FileSeek(file_handle, 0, 2);

	str = Now().FormatString("yyyy-mm-dd hh:nn:ss> ") + msg + "\n";
	FileWrite(file_handle, str.c_str(), str.Length());

	FileClose(file_handle);
}
//---------------------------------------------------------------------------
// 로그
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Mon Data 저장, Final Result 저장
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//* 첫번째 트레이 위치에서는 절반만 결과 저장.
//* 두번째 트레이 위치에서는 모든 결과 저장.
void __fastcall TTotalForm::WriteResultFile()
{
	int file_handle;
	AnsiString filename;
	AnsiString dir;
	AnsiString cell, cell_id, volt, curr, ok_ng;

	dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	filename =  dir + tray.trayid +  "-" + Now().FormatString("yymmddhhnnss") + ".csv";

	if(FileExists(filename)){
		DeleteFile(filename);
	}

	file_handle = FileCreate(filename);
	FileSeek(file_handle, 0, 0);

	AnsiString file;
	file = "TRAY ID," + tray.trayid + "\r\n";
    file = "TRAY POSITION,ALL\r\n";
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
        channel = i;
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
void __fastcall TTotalForm::WriteResultFile(int traypos)
{
	int file_handle;
	AnsiString filename;
	AnsiString dir;
	AnsiString cell, cell_id, volt, curr, ok_ng;

	dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	filename =  dir + tray.trayid + "_TP" + IntToStr(traypos) + ".csv";

	if(FileExists(filename)){
		DeleteFile(filename);
	}

	file_handle = FileCreate(filename);
	FileSeek(file_handle, 0, 0);

	AnsiString file;
	file = "TRAY ID," + tray.trayid + "\r\n";
    file = "TRAY POSITION," + IntToStr(traypos) + "\r\n";
	file = file + "ARRIVE TIME," + m_dateTime.FormatString("yyyy/mm/dd hh:nn:ss") + "\r\n";
	file = file + "FINISH TIME," + Now().FormatString("yyyy/mm/dd hh:nn:ss") + "\r\n";
	file = file + "VOLTAGE," + FormatFloat("0", config.volt) + "\r\n";
	file = file + "CURRENT," + FormatFloat("0", config.curr) + "\r\n";
	file = file + "TIME," + FormatFloat("0", config.time) + "\r\n";

	file += "CH,CELL,CELL ID,VOLT,CURR,RESULT\r\n";

    int channel;
	for(int i = 0; i < MAXCHANNEL; ++i){
        channel = i;
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
bool __fastcall TTotalForm::ReadResultFile(int traypos)
{
    bool bExistFile = false;
	AnsiString filename;
    AnsiString dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\";
    filename = dir + tray.trayid + "_TP" + IntToStr(traypos) + ".csv";

    if (!FileExists(filename)) {
        WritePLCLog("ReadResultFile","File is not exist: " + filename);
        return false;
    }
    else
        bExistFile = true;

    TStringList *lines = new TStringList;
    try {
        lines->LoadFromFile(filename);

        // CSV에서 데이터 시작 인덱스: 헤더는 6줄 (TRAY ID ~ CH,CELL...) 이후부터 데이터
        int startLine = 6;
        int idx = 0;
        int channel;

        AnsiString volt, curr, result;
        for (int i = startLine; i < lines->Count; i++) {
            AnsiString line = lines->Strings[i];
            if (line.Trim().IsEmpty()) continue;

            // CH,CELL,CELL_ID,VOLT,CURR,RESULT
            TStringList *cols = new TStringList;
            cols->Delimiter = ',';
            cols->StrictDelimiter = true;
            cols->DelimitedText = line;

            channel = GetChMap(this->Tag, traypos, idx) - 1;
            if (cols->Count >= 6) {
                // VOLT, CURR는 3, 4번째 열
                //if(MeasureInfoForm->pcurr[channel]->Caption.Pos("-") > 1){
                    channel = StringToInt(cols->Strings[0].Trim(), 1) - 1;
                    volt = cols->Strings[3].Trim();
                    curr = cols->Strings[4].Trim();
                    result = cols->Strings[5].Trim();
                	MeasureInfoForm->pvolt[channel]->Caption = volt;
                	MeasureInfoForm->pcurr[channel]->Caption = curr;

                    real_data.final_volt[channel] = StringToDouble(volt, 0);
					real_data.final_curr[channel] = StringToDouble(curr, 0);

                    if(result == "OK") tray.measure_result[channel] = 0;
                    else tray.measure_result[channel] = 1;
                //}
                idx++;
            }
            delete cols;
        }
    }
    __finally {
        delete lines;
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteMonData(int traypos)
{
    AnsiString dir, filename;
	int file_handle;
    int channel;

	dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	filename =  dir + tray.trayid +  "_MONTP" + IntToStr(traypos) + ".csv";
    AnsiString file = "";

	if(FileExists(filename))
		file_handle = FileOpen(filename, fmOpenWrite);
	else{
		file_handle = FileCreate(filename);

    	file = "DATETIME,RUNCOUNT";
    	for(int nIndex = 0; nIndex < CHANNELCOUNT; nIndex++){
        	channel = (nTrayPos - 1) * 288 + nIndex;
        	file += ",CH" + IntToStr(channel + 1) + " STATUS,CH" + IntToStr(channel + 1) + " VOLTAGE,CH" + IntToStr(channel + 1) + " CURRENT";
        }
    	file += "\r\n";
	}

	FileSeek(file_handle, 0, 2);

    file += Now().FormatString("yyyy/mm/dd hh:nn:ss.zzz") + "," + stage.runcount;

	for(int i = 0; i < CHANNELCOUNT; ++i){
        channel = (nTrayPos - 1) * 288 + i;
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

	AnsiString strNg, strTotalUse, strConsNg, strPrevNg;
	AnsiString title = "REMEASURE" + IntToStr(this->Tag);

	int posNg = 0, posTotalNg = 0, posConsNg = 0, posPrevNg = 0;
	strNg = ini->ReadString(title, "NG", "-1");
    strTotalUse = ini->ReadString(title, "TOTAL_USE", "-1");
    strConsNg = ini->ReadString(title, "CONSECUTIVE_NG", "-1" );
    strPrevNg = ini->ReadString(title, "PREV_NG", "-1");

    int nRemeasureAlarmCount = 0;
	config.remeasure_alarm_cnt = ini->ReadInteger("MAIN", "REMEASURE_ALARM_COUNT", 3);
	editRemeasureAlarmCount->Text = config.remeasure_alarm_cnt;
	RemeasureForm->pcolor2->Caption = config.remeasure_alarm_cnt;

	//* 총 누적 불량
	if(strNg == "-1"){	// 파일이 존재하지 않으면
		for(int index = 0; index < MAXCHANNEL; ++index)
			acc_remeasure[index] = 0; 	// 모두 0으로
	}
	else{
		for(int index = 0; index < MAXCHANNEL; ++index){
			posNg = strNg.Pos("_");
			acc_remeasure[index] = strNg.SubString(1, posNg - 1).ToIntDef(0);
			strNg.Delete(1, posNg);
		}
	}

	//* 총 측정 횟수
	if(strTotalUse == "-1"){	// 파일이 존재하지 않으면
		for(int index = 0; index < MAXCHANNEL; ++index)
			acc_totaluse[index] = 0; 	// 모두 0으로
	}
	else{
		for(int index = 0; index < MAXCHANNEL; ++index){
			posTotalNg = strTotalUse.Pos("_");
			acc_totaluse[index] = strTotalUse.SubString(1, posTotalNg - 1).ToIntDef(0);
			strTotalUse.Delete(1, posTotalNg);
		}
	}

    //* 이전 측정에서 불량여부
    if(strPrevNg == "-1"){
        for(int index = 0; index < MAXCHANNEL; ++index)
            acc_prevng[index] = 0;
    } else{
        for(int index = 0; index < MAXCHANNEL; ++index){
            posPrevNg = strPrevNg.Pos("_");
            acc_prevng[index] = strPrevNg.SubString(1, posPrevNg - 1).ToIntDef(0);
            strPrevNg.Delete(1, posPrevNg);
        }
    }

    //* 연속불량 횟수
	if(strConsNg == "-1"){	// 파일이 존재하지 않으면
		for(int index = 0; index < MAXCHANNEL; ++index)
			acc_consng[index] = 0; 	// 모두 0으로
	}
	else{
		for(int index = 0; index < MAXCHANNEL; ++index){
			posConsNg = strConsNg.Pos("_");
			acc_consng[index] = strConsNg.SubString(1, posConsNg - 1).ToIntDef(0);
			if(acc_consng[index] >= config.remeasure_alarm_cnt)
				nRemeasureAlarmCount++;
			strConsNg.Delete(1, posConsNg);
		}
	}
    //* 연속불량이 일정횟수 이상이면 에러발생
    RemeasureAlarm(nRemeasureAlarmCount);

	acc_init = 	 ini->ReadString(title, "ACCMULATE_DAY", Now().FormatString("yyyy. m. d. hh:nn"));
	acc_cnt = ini->ReadInteger(title, "ACC_CNT", 0);

	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteRemeasureInfo()	// Tray가 Vacancy 상태일때 기록
{
	TIniFile *ini;
	ini = new TIniFile((AnsiString)BIN_PATH + "RemeasureInfo.inf");

	AnsiString strNg, strTotalUse, strConsNg, strPrevNg;
	AnsiString title = "REMEASURE" + IntToStr(this->Tag);
	strNg = "";
    strTotalUse = "";
    strConsNg = "";
    strPrevNg = "";
	int nRemeasureAlarmCount = 0;
	config.remeasure_alarm_cnt = StringToInt(RemeasureForm->pcolor2->Caption, 3);

	for(int index = 0; index < MAXCHANNEL; ++index){
		strNg = strNg + acc_remeasure[index] + "_";
        strTotalUse = strTotalUse + acc_totaluse[index] + "_";
        strConsNg = strConsNg + acc_consng[index] + "_";
        strPrevNg = strPrevNg + acc_prevng[index] + "_";

		if(acc_consng[index] >= config.remeasure_alarm_cnt)
			nRemeasureAlarmCount++;
	}
    RemeasureAlarm(nRemeasureAlarmCount);

	ini->WriteInteger(title, "REMEASURE_ALARM_COUNT", editRemeasureAlarmCount->Text.ToIntDef(3));
	ini->WriteString(title, "TOTAL_USE", strTotalUse);
	ini->WriteString(title, "NG", strNg);
    ini->WriteString(title, "CONSECUTIVE_NG", strConsNg);
    ini->WriteString(title, "PREV_NG", strPrevNg);
	ini->WriteString(title, "ACCMULATE_DAY", acc_init);
	ini->WriteInteger(title, "ACC_CNT", acc_cnt); //* 이 버전에서는 안쓰는 변수

	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::RemeasureAlarm(int remeasure_alarm_count)
{
	if(remeasure_alarm_count > 0) {
        Mod_PLC->SetPcValue(PC_D_PRE_NG_ALARM, 1);
		btnNgInfo->Color = clRed;
		lblRemeasureAlarmCheck->Visible = true;
	}
	else{
        Mod_PLC->SetPcValue(PC_D_PRE_NG_ALARM, 0);
		btnNgInfo->Color = clWhite;
		lblRemeasureAlarmCheck->Visible = false;
    }
}
//---------------------------------------------------------------------------
// 재측정 정보 읽고 쓰기
//---------------------------------------------------------------------------
int __fastcall TTotalForm::ReadCellSerial()
{
    int nCellSerial = 0;
    for(int i = 0; i < MAXCHANNEL; i++){
        tray.cell_serial[i] = Mod_PLC->GetCellSrial(PLC_D_PRE_CELL_SERIAL, i, 10);
        if(tray.cell_serial[i].IsEmpty() == false) nCellSerial++;
    }
    return nCellSerial;
}
//---------------------------------------------------------------------------
