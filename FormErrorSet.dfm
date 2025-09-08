object Form_ErrorSet: TForm_ErrorSet
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Form_ErrorSet'
  ClientHeight = 327
  ClientWidth = 640
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
    Left = 70
    Top = 12
    Width = 440
    Height = 64
    AutoSize = False
    Caption = 'SETTING ERROR'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -53
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object btnOK: TButton
    Left = 215
    Top = 258
    Width = 150
    Height = 50
    Caption = 'OK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = btnOKClick
  end
  object Label_Msg1: TStaticText
    Left = 20
    Top = 96
    Width = 600
    Height = 149
    Margins.Left = 10
    Margins.Top = 10
    Margins.Right = 10
    Margins.Bottom = 10
    AutoSize = False
    Caption = 'The current settings are invalid.  Please reset and try again.'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    TabOrder = 1
    Transparent = False
  end
  object Timer_BringToFront: TTimer
    Enabled = False
    OnTimer = Timer_BringToFrontTimer
    Left = 576
    Top = 224
  end
  object timerErrorOff: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerErrorOffTimer
    Left = 575
    Top = 280
  end
end
