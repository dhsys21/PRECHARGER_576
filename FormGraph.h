//---------------------------------------------------------------------------

#ifndef FormGraphH
#define FormGraphH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvSmoothButton.hpp"
#include "AdvSmoothPanel.hpp"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
//---------------------------------------------------------------------------
class TForm_Graph : public TForm
{
__published:	// IDE-managed Components
	TAdvSmoothPanel *AdvSmoothPanel2;
	TChart *tChart;
	TPanel *Panel1;
	TEdit *chartMinEdit;
	TDateTimePicker *PreDay;
	TStaticText *StaticText3;
	TDateTimePicker *PostDay;
	TPanel *Panel2;
	TAdvSmoothButton *AdvSmoothButton3;
	TAdvSmoothButton *AdvSmoothButton4;
	TAdvSmoothButton *SearchBtn;
	TFastLineSeries *Series1;
	TFastLineSeries *Series2;
	TFastLineSeries *Series3;
	TFastLineSeries *Series4;
	TFastLineSeries *Series5;
	TFastLineSeries *Series6;
	TFastLineSeries *Series7;
	TFastLineSeries *Series8;
	TFastLineSeries *Series9;
	TFastLineSeries *Series10;
	TFastLineSeries *Series11;
	TFastLineSeries *Series12;
	TFastLineSeries *Series13;
	TFastLineSeries *Series14;
	TFastLineSeries *Series15;
	TFastLineSeries *Series16;
	TFastLineSeries *Series17;
	TFastLineSeries *Series18;
	TFastLineSeries *Series19;
	TFastLineSeries *Series20;
	TFastLineSeries *Series21;
	TFastLineSeries *Series22;
	TFastLineSeries *Series23;
	TFastLineSeries *Series24;
	TFastLineSeries *Series25;
	TFastLineSeries *Series26;
	TFastLineSeries *Series27;
	TFastLineSeries *Series28;
	TFastLineSeries *Series29;
	TFastLineSeries *Series30;
	TFastLineSeries *Series31;
	TFastLineSeries *Series32;
	TFastLineSeries *Series33;
	TFastLineSeries *Series34;
	TFastLineSeries *Series35;
	TFastLineSeries *Series36;
	TFastLineSeries *Series37;
	TFastLineSeries *Series38;
	TFastLineSeries *Series39;
	TFastLineSeries *Series40;
	TFastLineSeries *Series41;
	TFastLineSeries *Series42;
	TFastLineSeries *Series43;
	TFastLineSeries *Series44;
	TFastLineSeries *Series45;
	TFastLineSeries *Series46;
	TFastLineSeries *Series47;
	TFastLineSeries *Series48;
	TFastLineSeries *Series49;
	TFastLineSeries *Series50;
	TFastLineSeries *Series51;
	TFastLineSeries *Series52;
	TFastLineSeries *Series53;
	TFastLineSeries *Series54;
	TFastLineSeries *Series55;
	TFastLineSeries *Series56;
	TFastLineSeries *Series57;
	TFastLineSeries *Series58;
	TFastLineSeries *Series59;
	TFastLineSeries *Series60;
	TFastLineSeries *Series61;
	TFastLineSeries *Series62;
	TFastLineSeries *Series63;
	TFastLineSeries *Series64;
	TFastLineSeries *Series65;
	TFastLineSeries *Series66;
	TFastLineSeries *Series67;
	TFastLineSeries *Series68;
	TFastLineSeries *Series69;
	TFastLineSeries *Series70;
	TFastLineSeries *Series71;
	TFastLineSeries *Series72;
	TFastLineSeries *Series73;
	TFastLineSeries *Series74;
	TFastLineSeries *Series75;
	TGroupBox *GroupBox2;
	TEdit *chartMaxEdit;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall chartMinEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall chartMaxEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall tChartMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
          TPoint &MousePos, bool &Handled);


private:	// User declarations
    int chart_min_value, chart_max_value;
public:		// User declarations
	__fastcall TForm_Graph(TComponent* Owner);

	void __fastcall ChartMinMax(int min, int max);
	void __fastcall WriteGraphInfo();
	void __fastcall ReadGraphInfo();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Graph *Form_Graph;
//---------------------------------------------------------------------------
#endif
