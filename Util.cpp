//---------------------------------------------------------------------------

#pragma hdrstop

#include "Util.h"
#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
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
// Channel Mapping
int GetChMap(int stage, int trayPos, int index)
{
    int offset = (trayPos - 1) * (MAXCHANNEL / 2);
    return BaseForm->nForm[stage]->chMap[offset + index + 1];
}
//---------------------------------------------------------------------------
// Reverse Channel Mapping
int GetChRMap(int stage, int trayPos, int index)
{
    int offset = (trayPos - 1) * (MAXCHANNEL / 2);
    return BaseForm->nForm[stage]->chReverseMap[offset + index + 1];
}
//---------------------------------------------------------------------------
// Get Channel Position Front
AnsiString GetChPosF(int stage, int index)
{
    int rch = BaseForm->nForm[stage]->chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) / LINECOUNT + 1);
}
//---------------------------------------------------------------------------
AnsiString GetChPosF(int* chReverseMap, int index)
{
    int rch = chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) / LINECOUNT + 1);
}
//---------------------------------------------------------------------------
// Get Channel Position Rear
AnsiString GetChPosR(int stage, int index)
{
    int rch = BaseForm->nForm[stage]->chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) % LINECOUNT + 1);
}
//---------------------------------------------------------------------------
AnsiString GetChPosR(int* chReverseMap, int index)
{
    int rch = chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) % LINECOUNT + 1);
}
//---------------------------------------------------------------------------