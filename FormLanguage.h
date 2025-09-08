//---------------------------------------------------------------------------

#ifndef FormLanguageH
#define FormLanguageH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>

#include "FormMeasureInfo.h"
#include "FormRemeasure.h"
//---------------------------------------------------------------------------
class TForm_Language : public TForm
{
__published:	// IDE-managed Components
	TValueListEditor *VLE_KOREAN;
	TValueListEditor *VLE_ENGLISH;
	TValueListEditor *VLE_CHINESE;
private:	// User declarations
public:		// User declarations
	__fastcall TForm_Language(TComponent* Owner);
	void __fastcall LanguageChange(int LangIndex);

	//* Message º¯¼ö
	UnicodeString msgRst;
    UnicodeString msgRbt;
    UnicodeString msgTooManyNG;
    UnicodeString msgErrorSet;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Language *Form_Language;
//---------------------------------------------------------------------------
#endif
