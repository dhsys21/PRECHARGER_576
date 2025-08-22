//---------------------------------------------------------------------------

#ifndef FormErrorResetH
#define FormErrorResetH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_ErrorReset : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label_Title;
	TLabel *Label_Msg1;
	TLabel *Label1;
	TButton *btnReset;
	TButton *btnCancel;
	TTimer *Timer_BringToFront;
	void __fastcall Timer_BringToFrontTimer(TObject *Sender);
	void __fastcall btnResetClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_ErrorReset(TComponent* Owner);
    int stage;
    void __fastcall DisplayErrorMessage(int nStage);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_ErrorReset *Form_ErrorReset;
//---------------------------------------------------------------------------
#endif
