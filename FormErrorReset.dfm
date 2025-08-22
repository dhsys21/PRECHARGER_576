object Form_ErrorReset: TForm_ErrorReset
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Form_ErrorReset'
  ClientHeight = 338
  ClientWidth = 600
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label_Title: TLabel
    Left = 50
    Top = 37
    Width = 487
    Height = 48
    Caption = 'Please reset equipment '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -40
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label_Msg1: TLabel
    Left = 50
    Top = 116
    Width = 381
    Height = 35
    Caption = 'Pressing the reset button '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -29
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label1: TLabel
    Left = 50
    Top = 164
    Width = 359
    Height = 35
    Caption = 'will reset the qeuipment'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -29
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object btnReset: TButton
    Left = 125
    Top = 250
    Width = 150
    Height = 50
    Caption = 'RESET'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = btnResetClick
  end
  object btnCancel: TButton
    Left = 325
    Top = 250
    Width = 150
    Height = 50
    Caption = 'CANCEL'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btnCancelClick
  end
  object Timer_BringToFront: TTimer
    Enabled = False
    OnTimer = Timer_BringToFrontTimer
    Left = 496
    Top = 160
  end
end
