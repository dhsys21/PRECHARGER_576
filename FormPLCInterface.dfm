object Form_PLCInterface: TForm_PLCInterface
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'PLC Interface'
  ClientHeight = 951
  ClientWidth = 1899
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
    Left = 4
    Top = 8
    Width = 1886
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
      Left = 950
      Top = 47
      Width = 920
      Height = 870
      Columns = <
        item
          Caption = 'PC_Address'
          Width = 200
        end
        item
          Caption = 'PC_Name'
          Width = 300
        end
        item
          Caption = 'PC_Value'
          Width = 400
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
      Left = 15
      Top = 47
      Width = 920
      Height = 870
      Columns = <
        item
          Caption = 'PLC_Address'
          Width = 200
        end
        item
          Caption = 'PLC_Name'
          Width = 300
        end
        item
          Caption = 'PLC_Value'
          Width = 400
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
  end
  object Panel1: TPanel
    Left = 544
    Top = 192
    Width = 809
    Height = 313
    BorderStyle = bsSingle
    TabOrder = 1
    Visible = False
    object Label1: TLabel
      Left = 372
      Top = 8
      Width = 70
      Height = 19
      Caption = 'PLC TEST'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object GroupBox1: TGroupBox
      Left = 496
      Top = 47
      Width = 257
      Height = 250
      Caption = 'IR/OCV Value'
      TabOrder = 0
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
        Text = '3100'
      end
      object editCurrent: TEdit
        Left = 39
        Top = 126
        Width = 178
        Height = 21
        TabOrder = 1
        Text = '1000'
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
    object GroupBox3: TGroupBox
      Left = 31
      Top = 188
      Width = 434
      Height = 112
      Caption = 'IROCV NG Value'
      TabOrder = 1
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
    object GroupBox4: TGroupBox
      Left = 31
      Top = 47
      Width = 434
      Height = 135
      Caption = 'Write Value'
      TabOrder = 2
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
        Text = '41000'
        Items.Strings = (
          '41000'
          '41001'
          '41002'
          '41003'
          '41004'
          '41005'
          '41006'
          '41007'
          '41010'
          '41011'
          '41013'
          '41015'
          '41017')
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
  end
  object Timer_Update: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer_UpdateTimer
    Left = 390
    Top = 8
  end
end
