//---------------------------------------------------------------------------

#pragma hdrstop
#include <DateUtils.hpp>

#include "Util.h"
#include "RVMO_main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

bool __fastcall WaitForMilliSeconds(int milliseconds)
{
    TDateTime startTime = Now();

    while (MilliSecondsBetween(Now(), startTime) < milliseconds)
    {
        Application->ProcessMessages();  // UI가 멈추지 않도록 함
    }
    return true;
}
AnsiString __fastcall getSettingValue(AnsiString str, int nIndex)
{
    AnsiString setval = "";
    TStringList *tokens = new TStringList;
    tokens->Delimiter = ' ';
    tokens->StrictDelimiter = true;   // 중요!
    tokens->DelimitedText = str;

    if(nIndex == 1) setval = tokens->Strings[0]; // "0mv"
    else if(nIndex == 2) setval = tokens->Strings[1]; // "0ma"
    else if(nIndex == 3) setval = tokens->Strings[2]; // "0s"

    delete tokens;

    return setval;
}
int __fastcall StringToInt(UnicodeString str, int def)
{
	int iVal;
	if(TryStrToInt(str, iVal) == true) return iVal;
	else return def;
}
//---------------------------------------------------------------------------
double __fastcall StringToDouble(UnicodeString str, double def)
{
	double dVal;
	if(TryStrToFloat(str, dVal) == true) return dVal;
	else return def;
}
//---------------------------------------------------------------------------
bool compareChannelInfoByMappedChannel(const CHANNELINFO& a, const CHANNELINFO& b)
{
    return a.mappedChannel < b.mappedChannel;
}
//---------------------------------------------------------------------------
// Channel Mapping
int GetChMap(int stage, int trayPos, int index)
{
    int offset = (trayPos - 1) * CHANNELCOUNT;
    return BaseForm->nForm[stage]->chMap[offset + index + 1];
}
//---------------------------------------------------------------------------
int GetChannel(int trayPos, int index)
{
    int offset = (trayPos - 1) * CHANNELCOUNT;
    return offset + index + 1;
}
//---------------------------------------------------------------------------
int __fastcall GetPaired(int stage, int nIndex)
{
    int rch = BaseForm->nForm[stage]->chReverseMap[nIndex + 1];
    //* chReverseMap을 사용해서 계산
    if(rch >= 289) rch  = rch - 288;
    else rch = rch + 288;
    int paired = BaseForm->nForm[stage]->chMap[rch];

    return paired;
}
//---------------------------------------------------------------------------
int __fastcall GetBoardNo(int stage, int nIndex)
{
    int reversechannel = BaseForm->nForm[stage]->chReverseMap[nIndex + 1];
    //* chReverseMap을 사용해서 계산
    if(reversechannel >= 289) reversechannel  = reversechannel - 288;
    int board_no = (reversechannel - 1) / LINECOUNT + 1;

    return board_no;
}
//---------------------------------------------------------------------------
AnsiString SetChannelHint(int reversechannel)
{
    AnsiString hint = "";
    //* chReverseMap을 사용해서 계산
    if(reversechannel >= 289) reversechannel  = reversechannel - 288;
    int board_no = (reversechannel - 1) / LINECOUNT + 1;
    //int channel_no = (reversechannel - 1) % LINECOUNT + 1;
    int channel_no = 24 - (reversechannel - 1) % LINECOUNT;
    hint = IntToStr(board_no) + " - " + IntToStr(channel_no);
    return hint;
}
AnsiString SetChannelHint2(int channel)
{
    AnsiString hint = "";

    //* 채널번호를 1-576으로 해서 계산
    int group = (channel % 24) + 1;
    int channel_no = channel / 24 + 1;

    int board_no;
    if (group % 4 == 3 || group % 4 == 0)
        board_no = group / 2;
    else if(group % 4 == 1)
        board_no = (group + 1) / 2;
    else if(group % 4 == 2)
        board_no = (group + 1) / 2 + 1;

    hint = IntToStr(board_no) + " - " + IntToStr(channel_no);
    return hint;
}
//---------------------------------------------------------------------------
// Reverse Channel Mapping
int GetChRMap(int stage, int trayPos, int index)
{
    int offset = (trayPos - 1) * CHANNELCOUNT;
    return BaseForm->nForm[stage]->chReverseMap[offset + index + 1];
}
//---------------------------------------------------------------------------
// Get Channel Position Front
AnsiString GetChPosF(int index)
{
    AnsiString hint = "";
    hint = IntToStr((index - 1) / LINECOUNT + 1);
    return hint;
}
//---------------------------------------------------------------------------
AnsiString GetChPosF(int stage, int index, AnsiString type)
{
    int fch = 0;
    AnsiString hint = "";
    fch = BaseForm->nForm[stage]->chReverseMap[index + 1];
   	if(fch >= 289) fch  = fch - 288;
    if(type == "3"){
	    hint = IntToStr((fch - 1) / LINECOUNT + 1);
    } else if(type == "4"){
	    hint = IntToStr((fch - 1) / LINECOUNT + 1);
    }
    return hint;
}
//---------------------------------------------------------------------------
AnsiString GetChPosF(int* chReverseMap, int index, AnsiString type)
{
    int fch = 0;
    AnsiString hint = "";
    fch = chReverseMap[index + 1];
    if(fch >= 289) fch  = fch - 288;
    if(type == "3"){
	    hint = IntToStr((fch - 1) / LINECOUNT + 1);
    } else if(type == "4"){
	    hint = IntToStr((fch - 1) / LINECOUNT + 1);
    }

    return hint;
}
//---------------------------------------------------------------------------
// Get Channel Position Rear
AnsiString GetChPosR(int index)
{
    return IntToStr((index - 1) % LINECOUNT + 1);
}
//---------------------------------------------------------------------------
AnsiString GetChPosR(int stage, int index, AnsiString type)
{
    int rch = BaseForm->nForm[stage]->chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) % LINECOUNT + 1);
}
//---------------------------------------------------------------------------
AnsiString GetChPosR(int* chReverseMap, int index, AnsiString type)
{
    int rch = chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) % LINECOUNT + 1);
}
//---------------------------------------------------------------------------
void __fastcall OpenFolder(UnicodeString path)
{
	 ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------