//---------------------------------------------------------------------------

#ifndef ModBT2200H
#define ModBT2200H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Win.ScktComp.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMod_BT2200 : public TDataModule
{
__published:	// IDE-managed Components
	TClientSocket *ClientSocket_BT2200;
	TTimer *SendTimer;
private:	// User declarations
	TCustomWinSocket *sock1;	// BT2200 에 전송을 위한 소켓
public:		// User declarations
	__fastcall TMod_BT2200(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMod_BT2200 *Mod_BT2200;
//---------------------------------------------------------------------------
#endif
