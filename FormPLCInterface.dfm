object Form_PLCInterface: TForm_PLCInterface
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'PLC Interface'
  ClientHeight = 951
  ClientWidth = 1304
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox2: TGroupBox
    Left = 6
    Top = 8
    Width = 1295
    Height = 935
    Caption = 'PreCharger 1 (PLC - PC INTERFACE)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = GroupBox2Click
    object ListView_PC: TListView
      Left = 652
      Top = 47
      Width = 635
      Height = 870
      Columns = <
        item
          Caption = 'PC_Address'
          Width = 120
        end
        item
          Caption = 'PC_Name'
          Width = 200
        end
        item
          Caption = 'PC_Value'
          Width = 290
        end>
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      GridLines = True
      ReadOnly = True
      RowSelect = True
      ParentFont = False
      TabOrder = 0
      ViewStyle = vsReport
    end
    object ListView_PLC: TListView
      Left = 16
      Top = 47
      Width = 630
      Height = 870
      Columns = <
        item
          Caption = 'PLC_Address'
          Width = 120
        end
        item
          Caption = 'PLC_Name'
          Width = 200
        end
        item
          Caption = 'PLC_Value'
          Width = 290
        end>
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      GridLines = True
      ReadOnly = True
      RowSelect = True
      ParentFont = False
      TabOrder = 1
      ViewStyle = vsReport
    end
    object chkAllData: TCheckBox
      Left = 1198
      Top = 16
      Width = 89
      Height = 17
      Caption = 'All Data'
      TabOrder = 2
      OnClick = chkAllDataClick
    end
  end
  object Panel1: TAdvSmoothPanel
    Left = 288
    Top = 183
    Width = 809
    Height = 330
    Cursor = crDefault
    CanMove = True
    Caption.Location = plCenterCenter
    Caption.HTMLFont.Charset = DEFAULT_CHARSET
    Caption.HTMLFont.Color = clWindowText
    Caption.HTMLFont.Height = -11
    Caption.HTMLFont.Name = 'Tahoma'
    Caption.HTMLFont.Style = []
    Caption.HTMLURLColor = clBlack
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clWindowText
    Caption.Font.Height = -24
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = [fsBold]
    Caption.ColorStart = clWhite
    Caption.ColorEnd = clWhite
    Caption.LineColor = clBlack
    Caption.Line = False
    Caption.TextAlignment = taCenter
    Fill.Color = clWhite
    Fill.ColorTo = clWhite
    Fill.ColorMirror = clWhite
    Fill.ColorMirrorTo = clWhite
    Fill.GradientType = gtVertical
    Fill.GradientMirrorType = gtVertical
    Fill.BorderColor = 14922381
    Fill.Rounding = 5
    Fill.ShadowOffset = 0
    Fill.Glow = gmNone
    Version = '1.5.2.1'
    Visible = False
    TabOrder = 1
    TMSStyle = 0
    object lblConfiguration: TLabel
      Left = 7
      Top = 7
      Width = 78
      Height = 19
      Caption = 'PLC TEST'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold, fsUnderline]
      ParentFont = False
    end
    object btnSaveConfig: TAdvSmoothButton
      Left = 323
      Top = 492
      Width = 80
      Height = 40
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -17
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.Spacing = 0
      Appearance.Rounding = 3
      Appearance.WordWrapping = False
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 0
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      BevelColor = clMedGray
      Caption = 'Save '
      Color = clWhite
      ParentFont = False
      TabOrder = 0
      Visible = False
      Version = '2.1.1.5'
      TMSStyle = 8
    end
    object pnlCurrMin: TPanel
      Left = 13
      Top = 518
      Width = 184
      Height = 24
      Margins.Top = 5
      Margins.Bottom = 5
      BevelKind = bkFlat
      BevelOuter = bvNone
      Caption = 'PreCharging NG - Min. (mA)'
      Color = 15658734
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Padding.Top = 5
      ParentBackground = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 1
      Visible = False
    end
    object editCurrMin: TEdit
      Left = 199
      Top = 518
      Width = 108
      Height = 24
      Margins.Bottom = 0
      Alignment = taCenter
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      Text = '50'
      Visible = False
    end
    object GroupBox4: TGroupBox
      Left = 39
      Top = 47
      Width = 434
      Height = 135
      Caption = 'Write Value'
      TabOrder = 3
      object Label2: TLabel
        Left = 16
        Top = 22
        Width = 55
        Height = 16
        Caption = 'ADDRESS'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label3: TLabel
        Left = 16
        Top = 75
        Width = 37
        Height = 16
        Caption = 'VALUE'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object cbAddress: TComboBox
        Left = 16
        Top = 44
        Width = 225
        Height = 21
        TabOrder = 0
        Text = '34000'
        Items.Strings = (
          '34000'
          '34001'
          '34002'
          '34003'
          '34004'
          '34005'
          '34006'
          '34007'
          '34010'
          '34011'
          '34013'
          '34015'
          '34017')
      end
      object editValue: TEdit
        Left = 16
        Top = 98
        Width = 225
        Height = 21
        TabOrder = 1
        Text = '1'
      end
      object btnWriteValue: TButton
        Left = 247
        Top = 56
        Width = 83
        Height = 54
        Caption = 'Write Value'
        TabOrder = 2
        OnClick = btnWriteValueClick
      end
    end
    object GroupBox3: TGroupBox
      Left = 39
      Top = 188
      Width = 434
      Height = 112
      Caption = 'IROCV NG Value'
      TabOrder = 4
      object editIrOcvNg: TEdit
        Left = 16
        Top = 30
        Width = 401
        Height = 21
        TabOrder = 0
        Text = 
          '1,22,43,64,85,106,127,148,169,190,211,232,253,274,295,316,337,35' +
          '8,379,400'
      end
      object btnWriteNgValue: TButton
        Left = 304
        Top = 65
        Width = 113
        Height = 40
        Caption = 'Write NG Value'
        TabOrder = 1
        OnClick = btnWriteNgValueClick
      end
    end
    object GroupBox1: TGroupBox
      Left = 504
      Top = 47
      Width = 257
      Height = 250
      Caption = 'IR/OCV Value'
      TabOrder = 5
      object Label5: TLabel
        Left = 137
        Top = 40
        Width = 79
        Height = 16
        Caption = 'Voltage Value'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 134
        Top = 104
        Width = 79
        Height = 16
        Caption = 'Current Value'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object editVoltage: TEdit
        Left = 39
        Top = 62
        Width = 178
        Height = 21
        TabOrder = 0
        Text = '3600.5'
      end
      object editCurrent: TEdit
        Left = 39
        Top = 126
        Width = 178
        Height = 21
        TabOrder = 1
        Text = '3000'
      end
      object btnWriteIrOcvValue: TButton
        Left = 104
        Top = 161
        Width = 113
        Height = 40
        Caption = 'Write Value'
        TabOrder = 2
        OnClick = btnWriteIrOcvValueClick
      end
    end
  end
  object Timer_Update: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer_UpdateTimer
    Left = 390
    Top = 8
  end
end
