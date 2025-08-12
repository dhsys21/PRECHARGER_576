//---------------------------------------------------------------------------

#ifndef UtilH
#define UtilH
//---------------------------------------------------------------------------
#endif

#include <System.hpp>
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
#include "define.h"

int __fastcall StringToInt(UnicodeString str, int def);
double __fastcall StringToDouble(UnicodeString str, double def);
bool compareChannelInfoByMappedChannel(const CHANNELINFO& a, const CHANNELINFO& b);
int GetChannel(int trayPos, int index);
AnsiString SetChannelHint(int traypos);
AnsiString SetChannelHint();
int GetChMap(int stage, int trayPos, int index);
int GetChRMap(int stage, int trayPos, int index);
AnsiString GetChPosF(int index);
AnsiString GetChPosF(int stage, int index, AnsiString type);
AnsiString GetChPosF(int* chReverseMap, int index, AnsiString type);
AnsiString GetChPosR(int index);
AnsiString GetChPosR(int stage, int index, AnsiString type);
AnsiString GetChPosR(int* chReverseMap, int index, AnsiString type);
void OpenFolder(UnicodeString path);
