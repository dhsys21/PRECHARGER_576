//---------------------------------------------------------------------------

#ifndef FormErrorH
#define FormErrorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_Error : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label_Title;
	TLabel *Label_Msg1;
	TLabel *Label_Msg2;
	TButton *btnRestart;
	TTimer *Timer_BringToFront;
	TButton *btnTrayOut;
	TTimer *timerErrorOff;
	TButton *btnOK;
	void __fastcall Timer_BringToFrontTimer(TObject *Sender);
	void __fastcall timerErrorOffTimer(TObject *Sender);
	void __fastcall btnTrayOutClick(TObject *Sender);
	void __fastcall btnRestartClick(TObject *Sender);
	void __fastcall btnOKClick(TObject *Sender);
private:	// User declarations
	void __fastcall SaveErrorLog(AnsiString title, AnsiString msg1, AnsiString msg2);
public:		// User declarations
	__fastcall TForm_Error(TComponent* Owner);
	int stage;
	void __fastcall DisplayErrorMessage(AnsiString title, AnsiString msg1, AnsiString msg2, int nStage);
    void __fastcall DisplayErrorMessage(int nStage, int nError);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Error *Form_Error;
//---------------------------------------------------------------------------
#endif
