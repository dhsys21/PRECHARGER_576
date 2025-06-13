object Form_Error: TForm_Error
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Form_Error'
  ClientHeight = 359
  ClientWidth = 772
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label_Title: TLabel
    Left = 173
    Top = 29
    Width = 272
    Height = 48
    Caption = 'ERROR TITLE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -40
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label_Msg1: TLabel
    Left = 26
    Top = 132
    Width = 161
    Height = 35
    Caption = 'Error Msg1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -29
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label_Msg2: TLabel
    Left = 26
    Top = 197
    Width = 161
    Height = 35
    Caption = 'Error Msg2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -29
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object btnRestart: TButton
    Left = 396
    Top = 273
    Width = 150
    Height = 50
    Caption = 'RESTART'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = btnRestartClick
  end
  object btnTrayOut: TButton
    Left = 194
    Top = 273
    Width = 150
    Height = 50
    Caption = 'TRAY OUT'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btnTrayOutClick
  end
  object btnOK: TButton
    Left = 295
    Top = 273
    Width = 150
    Height = 50
    Caption = 'OK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    Visible = False
    OnClick = btnOKClick
  end
  object Timer_BringToFront: TTimer
    Enabled = False
    OnTimer = Timer_BringToFrontTimer
    Left = 496
    Top = 208
  end
  object timerErrorOff: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerErrorOffTimer
    Left = 648
    Top = 208
  end
end
