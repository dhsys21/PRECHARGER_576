//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormGraph.h"
#include "RVMO_main.h"
#include "FormTotal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvSmoothButton"
#pragma link "AdvSmoothPanel"
#pragma resource "*.dfm"
TForm_Graph *Form_Graph;
//---------------------------------------------------------------------------
__fastcall TForm_Graph::TForm_Graph(TComponent* Owner)
	: TForm(Owner)
{
	this->Parent = BaseForm;

	this->Left = 50;
	this->Top = 120;
	this->Width = 1820;
    this->Height = 940;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Graph::FormCreate(TObject *Sender)
{
//	for(int index = 0; index < max_ch + 3; ++index)
//	{
//        this->tChart->Series[index]->XValues->Order = loNone;
//	}

	chart_min_value = 0;
    chart_max_value = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Graph::WriteGraphInfo()
{
	TIniFile *ini;
	ini = new TIniFile((AnsiString)BIN_PATH + "graph.ini");

	ini->WriteString("CHART", "MIN", chartMinEdit->Text);
    ini->WriteString("CHART", "MAX", chartMaxEdit->Text);
	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Graph::ReadGraphInfo()
{
	TIniFile *ini;
	ini = new TIniFile((AnsiString)BIN_PATH + "graph.ini");
	chartMinEdit->Text = ini->ReadString("CHART", "MIN", "2500");
	chartMaxEdit->Text = ini->ReadString("CHART", "MAX", "4000");

	chart_min_value = chartMinEdit->Text.ToIntDef(0);
	chart_max_value = chartMaxEdit->Text.ToIntDef(0);
	ChartMinMax(chart_min_value, chart_max_value);
	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Graph::ChartMinMax(int min, int max)
{
	tChart->LeftAxis->Maximum = max;
	tChart->LeftAxis->Minimum = min;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Graph::chartMinEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	TEdit *edit;
	edit = (TEdit*)Sender;

	chart_min_value = edit->Text.ToIntDef(0);
	if(Key == VK_RETURN)
	{
		ChartMinMax(chart_min_value, chart_max_value);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Graph::chartMaxEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    TEdit *edit;
	edit = (TEdit*)Sender;

	chart_max_value = edit->Text.ToIntDef(0);
	if(Key == VK_RETURN)
	{
		ChartMinMax(chart_min_value, chart_max_value);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Graph::tChartMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled)
{
    Handled = true;
}
//---------------------------------------------------------------------------

