//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormRemeasure.h"
#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvSmoothButton"
#pragma resource "*.dfm"
TRemeasureForm *RemeasureForm;
//---------------------------------------------------------------------------

__fastcall TRemeasureForm::TRemeasureForm(TComponent* Owner)
	: TForm(Owner)
{
	stage = -1;
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::RefreshForm()
{

	for(int i=0; i<MAXCHANNEL; ++i){
		pre[i]->Caption = acc_remeasure[i];
		if(acc_remeasure[i] < pcolor2->Caption.ToIntDef(3)){
			pre[i]->Color = pcolor1->Color;
			pre[i]->ParentBackground = false;
		}
		else{
			pre[i]->Color = pcolor2->Color;
			pre[i]->ParentBackground = false;
		}
	}
	pAccCnt->Caption = IntToStr(*acc_cnt);
	pAccDate->Caption = *acc_init;

}
//---------------------------------------------------------------------------

void __fastcall TRemeasureForm::FormCreate(TObject *Sender)
{
	//MakeUIPanel();
	//MakePanel(BaseForm->lblLineNo->Caption);
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::DrawChannelTitle(AnsiString type)
{
	if(type == "1" || type == "2")
	{
		//* 세로 번호
		Panel3->Left = 3;
		Panel3->Top = 53;
		Panel3->Caption = "16";

		Panel4->Top = 101;
		Panel4->Caption = "15";

		Panel5->Top = 149;
		Panel5->Caption = "14";

		Panel6->Top = 197;
		Panel6->Caption = "13";

		Panel7->Top = 247;
		Panel7->Caption = "12";

		Panel8->Top = 295;
		Panel8->Caption = "11";

		Panel9->Top = 343;
		Panel9->Caption = "10";

		Panel10->Top = 391;
		Panel10->Caption = "9";

		Panel11->Top = 441;
		Panel11->Caption = "8";

		Panel12->Top = 489;
		Panel12->Caption = "7";

		Panel13->Top = 537;
		Panel13->Caption = "6";

		Panel14->Top = 585;
		Panel14->Caption = "5";

		Panel15->Top = 635;
		Panel15->Caption = "4";

		Panel16->Top = 683;
		Panel16->Caption = "3";

		Panel17->Top = 731;
		Panel17->Caption = "2";

		Panel18->Top = 779;
		Panel18->Caption = "1";


		//* 가로 번호
		Panel19->Left = 55;
		Panel19->Top = 3;
		Panel19->Caption = "1";

		Panel21->Left = 111;
		Panel21->Caption = "2";

		Panel22->Left = 169;
		Panel22->Caption = "3";

		Panel24->Left = 225;
		Panel24->Caption = "4";

		Panel25->Left = 283;
		Panel25->Caption = "5";

		Panel26->Left = 339;
		Panel26->Caption = "6";

		Panel27->Left = 397;
		Panel27->Caption = "7";

		Panel28->Left = 453;
		Panel28->Caption = "8";

		Panel29->Left = 511;
		Panel29->Caption = "9";

		Panel30->Left = 567;
		Panel30->Caption = "10";

		Panel31->Left = 625;
		Panel31->Caption = "11";

		Panel32->Left = 681;
		Panel32->Caption = "12";

		Panel33->Left = 739;
		Panel33->Caption = "13";

		Panel34->Left = 795;
		Panel34->Caption = "14";

		Panel23->Left = 853;
		Panel23->Caption = "15";

		Panel35->Left = 909;
		Panel35->Caption = "16";
	}
	else if(type == "3" || type == "4")
	{
        //* 세로 번호
		Panel3->Left = 3;
		Panel3->Top = 53;
		Panel3->Caption = "16";

		Panel4->Top = 101;
		Panel4->Caption = "15";

		Panel5->Top = 149;
		Panel5->Caption = "14";

		Panel6->Top = 197;
		Panel6->Caption = "13";

		Panel7->Top = 247;
		Panel7->Caption = "12";

		Panel8->Top = 295;
		Panel8->Caption = "11";

		Panel9->Top = 343;
		Panel9->Caption = "10";

		Panel10->Top = 391;
		Panel10->Caption = "9";

		Panel11->Top = 441;
		Panel11->Caption = "8";

		Panel12->Top = 489;
		Panel12->Caption = "7";

		Panel13->Top = 537;
		Panel13->Caption = "6";

		Panel14->Top = 585;
		Panel14->Caption = "5";

		Panel15->Top = 635;
		Panel15->Caption = "4";

		Panel16->Top = 683;
		Panel16->Caption = "3";

		Panel17->Top = 731;
		Panel17->Caption = "2";

		Panel18->Top = 779;
		Panel18->Caption = "1";


		//* 가로 번호
		Panel19->Left = 55;
		Panel19->Top = 3;
		Panel19->Caption = "16";

		Panel21->Left = 111;
		Panel21->Caption = "15";

		Panel22->Left = 169;
		Panel22->Caption = "14";

		Panel24->Left = 225;
		Panel24->Caption = "13";

		Panel25->Left = 283;
		Panel25->Caption = "12";

		Panel26->Left = 339;
		Panel26->Caption = "11";

		Panel27->Left = 397;
		Panel27->Caption = "10";

		Panel28->Left = 453;
		Panel28->Caption = "9";

		Panel29->Left = 511;
		Panel29->Caption = "8";

		Panel30->Left = 567;
		Panel30->Caption = "7";

		Panel31->Left = 625;
		Panel31->Caption = "6";

		Panel32->Left = 681;
		Panel32->Caption = "5";

		Panel33->Left = 739;
		Panel33->Caption = "4";

		Panel34->Left = 795;
		Panel34->Caption = "3";

		Panel23->Left = 853;
		Panel23->Caption = "2";

		Panel35->Left = 909;
		Panel35->Caption = "1";
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::MakePanel(AnsiString type)
{
	int nx, ny, nw, nh;
    nw = pnl_nw;
    nh = pnl_nh / 2;

	if(type == "3")
	{
		nx = nw + 4;
		ny = nh * 2 + 6;

		TColor clr;
		for(int index = 0; index < MAXCHANNEL;){
			pch[index] = new TPanel(this);
			pre[index] = new TPanel(this);

			SetOption(pch[index], nx, ny, nw, nh-1, index);
			pch[index]->Font->Color = clWhite;
			pch[index]->Caption = index+1;
			pch[index]->Color = clSkyBlue;
			pch[index]->ParentBackground = false;

			SetOption(pre[index], nx, ny+nh, nw, nh, index);
			pre[index]->Color = pcolor1->Color;
			pre[index]->ParentBackground = false;

			index += 1;
			nx += (nw + 1);
			if(index % 2 == 0) nx += 1;
			if(index % (LINECOUNT / 4) == 0) nx += 1;
			if(index % LINECOUNT == 0)
			{
				ny += nh * 2 + 2;
				nx = nw + 4;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny += 2;
			}
		}
	}
	else if(type == "4")
	{
		nx = pUIx[0]->Left;
		ny = nh * 2 + 6;

		TColor clr;
		for(int index = 0; index < MAXCHANNEL;){
			pch[index] = new TPanel(this);
			pre[index] = new TPanel(this);

			SetOption(pch[index], nx, ny, nw, nh-1, index);
			pch[index]->Font->Color = clWhite;
			pch[index]->Caption = index+1;
			pch[index]->Color = clSkyBlue;
			pch[index]->ParentBackground = false;

			SetOption(pre[index], nx, ny+nh, nw, nh, index);
			pre[index]->Color = pcolor1->Color;
			pre[index]->ParentBackground = false;

			index += 1;
			nx -= (nw + 1);
			if(index % 2 == 0) nx -= 1;
			if(index % (LINECOUNT / 4) == 0) nx -= 1;
			if(index % LINECOUNT == 0)
			{
				ny += nh * 2 + 2;
				nx = pUIx[0]->Left;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny += 2;
			}
		}
	}
	else if(type == "1")
	{
		nx = pUIx[0]->Left;
		ny = pUIy[0]->Top;

		TColor clr;
		for(int index = 0; index < MAXCHANNEL;){
			pch[index] = new TPanel(this);
			pre[index] = new TPanel(this);
			pch[index]->ParentBackground = false;
			pre[index]->ParentBackground = false;

			SetOption(pch[index], nx, ny, nw, nh, index);
			pch[index]->Font->Color = clWhite;
			pch[index]->Caption = index+1;
			pch[index]->Color = clSkyBlue;
			pch[index]->ParentBackground = false;

			SetOption(pre[index], nx, ny+nh+1, nw, nh, index);
			pre[index]->Color = pcolor1->Color;
			pre[index]->ParentBackground = false;

			index += 1;
			nx = nx + (nw + 1);
			if(index % 2 == 0) nx += 1;
			if(index % (LINECOUNT / 4) == 0) nx += 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - nh - 2;
				nx = nx = pUIx[0]->Left;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny -= 2;
			}
		}
    }
    else if(type == "2")
	{
		nx = pUIx[0]->Left;
		//ny = Panel2->Height - nh*2 - 5;
		ny = pUIy[0]->Top;

		TColor clr;
		for(int index = 0; index < MAXCHANNEL;){
			pch[index] = new TPanel(this);
			pre[index] = new TPanel(this);
			pch[index]->ParentBackground = false;
			pre[index]->ParentBackground = false;

			SetOption(pch[index], nx, ny, nw, nh, index);
			pch[index]->Font->Color = clWhite;
			pch[index]->Caption = index+1;
			pch[index]->Color = clSkyBlue;
			pch[index]->ParentBackground = false;

			SetOption(pre[index], nx, ny+nh+1, nw, nh, index);
			pre[index]->Color = pcolor1->Color;
			pre[index]->ParentBackground = false;

			index += 1;
			nx = nx - (nw + 1);
			if(index % 2 == 0) nx -= 1;
			if(index % (LINECOUNT / 4) == 0) nx -= 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - nh - 2;
				nx = pUIx[0]->Left;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny -= 2;
			}
		}
    }
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::MakeUIPanel(AnsiString type)
{
	int nx, ny, nw, nh;

	nw = pnl_nw;
	nh = pnl_nh + 1;

	if(type == "1"){
        ny = Panel2->Height - (nh) - 2;
        nx = nw + 4;

        for(int index = 0; index < LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, Panel35->Top, nw, nh, index);
            SetUIOption(pUIy[index], 2, ny-1, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx += (nw+1);
            if(index % 2 == 0) nx += 1;
            if(index % (LINECOUNT / 4) == 0) nx += 1;
            ny -= (nh+1);
            if( index % (LINECOUNT / 4) == 0) ny -= 2;
        }
    }
    else if(type == "2"){
        ny = Panel2->Height - (nh) - 2;
        nx = Panel2->Width - (nw + 2);

        for(int index = 0; index < LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, Panel35->Top, nw, nh, index);
            SetUIOption(pUIy[index], 2, ny-1, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx -= (nw+1);
            if(index % 2 == 0) nx -= 1;
            if(index % (LINECOUNT / 4) == 0) nx -= 1;
            ny -= (nh+1);
            if( index % (LINECOUNT / 4) == 0) ny -= 2;
        }
    }
    else if(type == "3"){
        ny = nh + 4;
        nx = nw + 4;
        for(int index = 0; index < LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, Panel35->Top, nw, nh, index);
            SetUIOption(pUIy[index], 2, ny, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx += (nw+1);
            if(index % 2 == 0) nx += 1;
            if(index % (LINECOUNT / 4) == 0) nx += 1;
            ny += (nh+1);
            if( index % (LINECOUNT / 4) == 0) ny += 2;
        }
    }
    else if(type == "4"){
        ny = nh + 4;
        nx = Panel2->Width - (nw + 2);;
        for(int index = 0; index < LINECOUNT;){
            pUIx[index] = new TPanel(this);
            pUIy[index] = new TPanel(this);

            SetUIOption(pUIx[index], nx, Panel35->Top, nw, nh, index);
            SetUIOption(pUIy[index], 2, ny, nw, nh, index);
            pUIx[index]->ParentBackground = false;
            pUIy[index]->ParentBackground = false;

            index ++;
            nx -= (nw+1);
            if(index % 2 == 0) nx -= 1;
            if(index % (LINECOUNT / 4) == 0) nx -= 1;
            ny += (nh+1);
            if( index % (LINECOUNT / 4) == 0) ny += 2;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::SetOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index)
{
	pnl->Parent = Panel2;
	pnl->Left =  nx;
	pnl->Top = ny;
	pnl->Width = nw;
	pnl->Height = nh;
	pnl->Alignment = taCenter;
	pnl->Font->Size = 10;
	pnl->Font->Color = clBlack;
	pnl->Font->Style = Font->Style << fsBold;

	pnl->BevelInner = bvNone;
	pnl->BevelKind = bkNone;
	pnl->BevelOuter = bvNone;
	pnl->Tag = index; // index + 16
	pnl->Hint = "POS : " + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1);

	pnl->ShowHint = true;
	pnl->OnDblClick = chInitdblClick;
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::SetUIOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index)
{
	pnl->Parent = Panel2;
    pnl->ParentBackground = false;
	pnl->Left = nx;
	pnl->Top = ny;
	pnl->Width = nw;
	pnl->Height = nh;
	pnl->Alignment = taCenter;
	pnl->Color = Panel35->Color;
	pnl->Caption = index+1;
    pnl->ShowCaption = true;
	pnl->Font->Size = 12;
	pnl->Font->Color = clBlack;
	pnl->Font->Style = Font->Style << fsBold;

	pnl->BevelInner = bvNone;
	pnl->BevelKind = bkNone;
	pnl->BevelOuter = bvRaised;
	pnl->BevelWidth =1;
	pnl->BiDiMode = bdLeftToRight;
	pnl->BorderStyle = bsNone;
	pnl->BorderWidth = 0;
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::FormShow(TObject *Sender)
{
    this->Left = 140;
	this->Top = 50;
	this->BringToFront();
	this->RefreshForm();
}
//---------------------------------------------------------------------------


void __fastcall TRemeasureForm::FormHide(TObject *Sender)
{
	stage = -1;	
}
//---------------------------------------------------------------------------


void __fastcall TRemeasureForm::chInitdblClick(TObject *Sender)
{
	TPanel *pnl;
	pnl = (TPanel*)Sender;
	int ch = pnl->Tag;
	int nRemeasureAlarmCount = 0;
	UnicodeString str;
	str = "Do you want to initialize the channel " + IntToStr(ch+1) +" record??";
   if(MessageBox(Handle, str.c_str(), L"", MB_YESNO|MB_ICONQUESTION) == ID_YES){
		acc_remeasure[ch] = 0;

		for(int index=0; index<MAXCHANNEL; ++index){
			if(acc_remeasure[index] >= pcolor2->Caption.ToIntDef(3))
				nRemeasureAlarmCount++;
		}
		BaseForm->nForm[stage]->RemeasureAlarm(nRemeasureAlarmCount);
		this->RefreshForm();
	}
}
//---------------------------------------------------------------------------

void __fastcall TRemeasureForm::AccInitBtnClick(TObject *Sender)
{
	if(MessageBox(Handle, L"Do you want to initialize?", L"", MB_YESNO|MB_ICONQUESTION) == ID_YES){
		for(int i=0; i<MAXCHANNEL; ++i)acc_remeasure[i] = 0;
		BaseForm->nForm[stage]->RemeasureAlarm(0);
		pAccDate->Caption = Now().FormatString("yyyy. m. d. hh:nn");
		pAccCnt->Caption = 0;
		*acc_cnt = 0;
		*acc_init = pAccDate->Caption;
		this->RefreshForm();
	}

}
//---------------------------------------------------------------------------

