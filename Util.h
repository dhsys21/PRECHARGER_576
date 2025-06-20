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
int GetChannel(int* chMap, int trayPos, int index);
int GetRChannel(int* chReverseMap, int trayPos, int index);
int GetChPosF(int* chReverseMap, int index);
int GetChPosR(int* chReverseMap, int index);
