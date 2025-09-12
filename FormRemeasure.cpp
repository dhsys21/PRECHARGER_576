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
    pnl_nw = 40;
    pnl_nh = 34;
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::RefreshForm()
{

	for(int i = 0; i < MAXCHANNEL; ++i){
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
    Panel2->Caption = "RefreshForm";
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::FormCreate(TObject *Sender)
{

    Panel2->Caption = BaseForm->lblLineNo->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::FormShow(TObject *Sender)
{
    this->Left = 140;
	this->Top = 50;
	this->BringToFront();

    MakeUIPanel(BaseForm->lblLineNo->Caption);
	MakePanel(BaseForm->lblLineNo->Caption);
	this->RefreshForm();
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::FormHide(TObject *Sender)
{
	stage = -1;
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
            if(pch[index] != NULL && pre[index] != NULL) return;

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
            if(pch[index] != NULL && pre[index] != NULL) return;

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
            if(pch[index] != NULL && pre[index] != NULL) return;

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
            if(pch[index] != NULL && pre[index] != NULL) return;

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
            if(pUIx[index] != NULL && pUIy[index] != NULL) return;

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
            if(pUIy[index] != NULL && pUIy[index] != NULL) return;

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
            if(pUIy[index] != NULL && pUIy[index] != NULL) return;

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
            if(pUIy[index] != NULL && pUIy[index] != NULL) return;

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
	pnl->Tag = index;
	//pnl->Hint = "POS : " + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1);
    //* ä�� ��ġ -> �����̰� 12���̹Ƿ� ��ġ�� ����ؾ� ��
    //int ch = BaseForm->nForm[stage]->chReverseMap[index + 1];
    //if(ch >= 289) ch  = ch - 288;
    //pnl->Hint = "POS : " + IntToStr((ch - 1)/LINECOUNT + 1) + "-" + IntToStr((ch - 1)%LINECOUNT + 1);
    pnl->Hint = "POS : " + SetChannelHint(index);
    pnl->OnMouseEnter = ChInfoMouseEnter;
    pnl->OnMouseLeave = ChInfoMouseLeave;

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
void __fastcall TRemeasureForm::ChInfoMouseEnter(TObject *Sender)
{
	TPanel *pnl;
	pnl = (TPanel*)Sender;
	int index;
	index = pnl->Tag;
	pnlCh->Caption = index + 1;

    int ch = BaseForm->nForm[stage]->chReverseMap[index + 1];
    pnlPos->Caption = SetChannelHint(ch);
}
//---------------------------------------------------------------------------
void __fastcall TRemeasureForm::ChInfoMouseLeave(TObject *Sender)
{
	pnlCh->Caption = "";
	pnlPos->Caption = "";
}
//---------------------------------------------------------------------------s
