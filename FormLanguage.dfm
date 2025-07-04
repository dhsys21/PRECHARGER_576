object Form_Language: TForm_Language
  Left = 0
  Top = 0
  Caption = 'Form_Language'
  ClientHeight = 805
  ClientWidth = 1011
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object VLE_KOREAN: TValueListEditor
    Left = 32
    Top = 24
    Width = 321
    Height = 265
    Strings.Strings = (
      'SAVE='#51200#51109
      'CANCEL='#52712#49548
      'CONFIG='#49444#51221
      'MANUAL='#49688#46041
      'AUTO='#51088#46041
      'RESET='#47532#49483
      'TRAYOUT='#53944#47112#51060#48176#52636
      'TRAYID='#53944#47112#51060#50500#51060#46356
      'STATUS='#49345#53468
      'PROCESS='#51652#54665
      'CHANNEL='#52292#45328
      'CONFIGURATION='#49444#51221
      'IR=IR'
      'OCV=OCV'
      'REMEAINFO='#48520#47049' '#51221#48372
      'INIT='#52488#44592#54868
      'OK='#54869#51064
      'CLOSE='#45803#44592
      'OPEN='#50676#44592
      'SET='#49483#54021
      'RESET='#47532#49483
      'START='#49884#51089
      'STOP='#51221#51648
      'SAVE='#51200#51109
      'PRECHARGER='#54532#47532#52264#51256
      'PLCCONNECTION=PLC '#50672#44208
      'ETC='#44592#53440
      'CHINFO='#52292#45328' '#51221#48372
      'CHPOS='#50948#52824
      'CONNECT='#50672#44208
      'DISCONNECT='#50672#44208#54644#51228
      'PROCESSINFO='#54532#47532#52264#51256' '#51652#54665' '#51221#48372
      'TRAYINFO='#53944#47112#51060' '#51221#48372
      'CHARGINGPROCESS='#52649#51204' '#54532#47196#49464#49828
      'msgRBT=Reboot'#51008' 60'#52488' '#51221#46020' '#49548#50836#46121#45768#45796'. Reboot '#54616#49884#44192#49845#45768#44620'?'
      'msgRST=Reset'#51008' 15'#52488' '#51221#46020' '#49548#50836#46121#45768#45796'. Reset '#54616#49884#44192#49845#45768#44620'?'
      'msgTooManyNG=NG '#49472#51060' '#45320#47924' '#47566#49845#45768#45796'. '#54869#51064#54644#51452#49464#50836
      'msgError='#50640#47084)
    TabOrder = 0
    ColWidths = (
      150
      148)
    RowHeights = (
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18)
  end
  object VLE_ENGLISH: TValueListEditor
    Left = 32
    Top = 312
    Width = 321
    Height = 265
    Strings.Strings = (
      'SAVE=SAVE'
      'CANCEL=CANCEL'
      'CONFIG=CONFIG'
      'MANUAL=MANUAL'
      'AUTO=AUTO'
      'RESET=RESET'
      'TRAYOUT=TRAY OUT'
      'TRAYID=TRAY ID'
      'STATUS=STATUS'
      'PROCESS=PROCESS'
      'CHANNEL=CHANNEL'
      'CONFIGURATION=Configuration'
      'IR=IR'
      'OCV=OCV'
      'REMEAINFO=NG INFO.'
      'INIT=INIT.'
      'OK=OK'
      'CLOSE=CLOSE'
      'OPEN=OPEN'
      'SET=SET'
      'RESET=RESET'
      'START=START'
      'STOP=STOP'
      'SAVE=SAVE'
      'PRECHARGER=PRECHARGER'
      'PLCCONNECTION=PLC CONNECTION'
      'ETC=ETC'
      'CHINFO=CHANNEL INFORMATION'
      'CHPOS=POS'
      'CONNECT=Connect'
      'PROCESSINFO=PRECHARGER PROCESS INFORMATION'
      'TRAYINFO=TRAY INFO.'
      'CHARGINGPROCESS=CHARGING PROCESS'
      'DISCONNECT=DisConnect'
      'msgRBT=Rebooting takes 60 seconds. Do you want to reboot?'
      'msgRST=Reset takes 15 seconds. Do you want to reset?'
      'msgTooManyNG=Too many NG cells. Please check it.'
      'msgError=Error')
    TabOrder = 1
    ColWidths = (
      150
      148)
    RowHeights = (
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18)
  end
  object VLE_CHINESE: TValueListEditor
    Left = 424
    Top = 24
    Width = 321
    Height = 265
    Strings.Strings = (
      'SAVE='#20445#23384
      'CANCEL='#21462#28040
      'CONFIG='#37197#32622
      'MANUAL= '#25163#21160
      'AUTO='#33258#21160
      'RESET='#37325#32622
      'TRAYOUT='#25176#30424#21462#20986
      'TRAYID='#25176#30424'ID'
      'STATUS='#29366#24577
      'PROCESS='#36807#31243
      'CHANNEL='#36890#36947
      'IR=IR'
      'OCV=OCV'
      'REMEAINFO='#19981#33391#20449#24687
      'INIT= '#21021#22987#21270
      'OK='#30830#23450
      'msgTooManyNG='#19981#33391#21697#36807#22810#65292#35831#26816#26597#12290)
    TabOrder = 2
    ColWidths = (
      150
      148)
    RowHeights = (
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18)
  end
end
