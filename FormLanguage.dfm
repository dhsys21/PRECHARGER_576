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
      'OK='#54869#51064
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
      'READY='#45824#44592
      'CHARGING='#52649#51204
      'COMPLETE='#50756#47308
      'FAIL='#48520#47049
      'OUTFLOW='#49472' '#50976#52636
      'NOCELL='#49472' '#50630#51020
      'msgWDTMode=PRECHARGER'#44032' WDT '#47784#46300#51077#45768#45796'. RESET '#54616#49464#50836'.'
      'msgNoSetValues='#49483#54021#44050#51060' '#50630#49845#45768#45796'.'
      'msgPLCConnFail=PLC - PC '#50672#44208' '#49892#54056'.'
      'msgPREConnFail=PRECHARGER '#50672#44208' '#49892#54056'.'
      'msgBTConnFail=BT '#50672#44208' '#49892#54056'.'
      'msgRBT=Reboot'#51008' 60'#52488' '#51221#46020' '#49548#50836#46121#45768#45796'. Reboot '#54616#49884#44192#49845#45768#44620'?'
      'msgRST=Reset'#51008' 15'#52488' '#51221#46020' '#49548#50836#46121#45768#45796'. Reset '#54616#49884#44192#49845#45768#44620'?'
      'msgTooManyNG=NG '#49472#51060' '#45320#47924' '#47566#49845#45768#45796'. '#54869#51064#54644#51452#49464#50836
      'msgErrorSet='#54788#51116' '#49444#51221' '#44050#51060' '#50732#48148#47476#51648' '#50506#49845#45768#45796'. \r\n\r\n'#45796#49884' '#49444#51221#54644' '#51452#49464#50836'.'
      'msgDelEachChRecord='#54644#45817' '#52292#45328' '#44592#47197#51012' '#49325#51228' '#54624#44620#50836'?'
      'msgDelAllChRecord='#47784#46304' '#52292#45328' '#44592#47197#51012' '#52488#44592#54868' '#54624#44620#50836'?'
      'msgVoltLimit='#51204#50517#51012' 2000mV '#51060#49345' '#51077#47141#54616#49464#50836'.'
      'msgCurrLimit='#51204#47448#47484' 260mA '#51060#49345' '#51077#47141#54616#49464#50836'.'
      'msgTimeLimit='#49884#44036#51012' 30sec '#51060#49345' '#51077#47141#54616#49464#50836'.'
      'msgSave='#51200#51109' '#54616#49884#44192#49845#45768#44620'?')
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
      'OK=OK'
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
      'READY=READY'
      'CHARGING=CHARGING'
      'COMPLETE=COMPLETE'
      'FAIL=FAIL'
      'OUTFLOW=OUTFLOW'
      'NOCELL=NO CELL'
      'msgWDTMode=PRECHARGER is WDT Mode. Please [RESET] it.'
      'msgNoSetValues=No Setting Values.'
      'msgPLCConnFail=PLC - PC Connection Fail.'
      'msgPREConnFail=PRECHARGER Connection Fail.'
      'msgBTConnFail=BT Connection Fail.'
      'msgRBT=Rebooting takes 60 seconds. Do you want to reboot?'
      'msgRST=Reset takes 15 seconds. Do you want to reset?'
      'msgTooManyNG=Too many NG cells. Please check it.'
      
        'msgErrorSet=The current settings are invalid. \r\n\r\nPlease res' +
        'et and try again.'
      'msgDelEachChRecord=Do you want to initialize the channel record?'
      'msgDelAllChRecord=Do you want to initialize all channels?'
      'msgVoltLimit=Please use Voltage more than 2000mV'
      'msgCurrLimit=Please use Current more than 260mA'
      'msgTimeLimit=Please use Time more than 30sec'
      'msgSave=Are you sure want to save?')
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
    Height = 553
    Strings.Strings = (
      'OK='#30906#23450
      'SAVE='#20445#23384
      'CANCEL='#30830#35748
      'CONFIG='#37197#32622
      'MANUAL='#25163#21160
      'AUTO='#33258#21160
      'RESET='#37325#32622
      'TRAYOUT='#25176#30424#21462#20986
      'TRAYID='#25176#30424'ID'
      'STATUS='#29366#24577
      'PROCESS='#36807#31243
      'CHANNEL='#36890#36947
      'CONFIGURATION='#37197#32622
      'IR=IR'
      'OCV=OCV'
      'REMEAINFO='#19981#33391#20449#24687
      'INIT='#21021#22987#21270
      'OK='#30830#23450
      'CLOSE='#20851#38381
      'OPEN='#25171#24320
      'SET='#35774#32622
      'RESET='#37325#32622
      'START='#24320#22987
      'STOP='#20572#27490
      'SAVE='#20445#23384
      'PRECHARGER=PRECHARGER'
      'PLCCONNECTION=PLC '#36830#25509
      'ETC='#31561#31561
      'CHINFO='#36890#36947#24773#22577
      'CHPOS='#20301#32622
      'CONNECT='#36830#25509
      'PROCESSINFO=PRECHARGER '#36807#31243#24773#22577
      'TRAYINFO='#25176#30424#24773#22577
      'CHARGINGPROCESS='#20805#30005' '#36807#31243
      'DISCONNECT='#26029#24320
      'READY='#31561#24453
      'CHARGING='#27491#22312#20805#30005
      'COMPLETE='#23436#25104
      'FAIL='#38169#35823
      'OUTFLOW=CELL'#27969#20986
      'NOCELL='#26080' CELL'
      'msgWDTMode=PRECHARGER '#22788#20110' WDT '#27169#24335#12290#35831#12304#37325#32622#12305#12290
      'msgNoSetValues='#27809#26377#35774#32622#20540#12290
      'msgPLCConnFail=PLC '#19982' PC '#36830#25509#22833#36133#12290
      'msgPREConnFail=PRECHARGER '#36830#25509#22833#36133#12290
      'msgBTConnFail=BT '#36830#25509#22833#36133#12290
      'msgRBT='#37325#21551#38656#35201' 60 '#31186#12290#26159#21542#35201#37325#21551#65311
      'msgRST='#37325#32622#38656#35201' 15 '#31186#12290#26159#21542#35201#37325#32622#65311
      'msgTooManyNG=NG '#30005#33455#25968#37327#36807#22810#12290#35831#26816#26597#12290
      'msgErrorSet='#24403#21069#35774#32622#26080#25928#12290'\r\n\r\n'#35831#37325#32622#21518#37325#35797#12290
      'msgDelEachChRecord='#26159#21542#35201#21021#22987#21270#35813#36890#36947#30340#35760#24405#65311
      'msgDelAllChRecord='#26159#21542#35201#21021#22987#21270#25152#26377#36890#36947#65311
      'msgVoltLimit='#35831#20351#29992#39640#20110' 2000mV '#30340#30005#21387#12290
      'msgCurrLimit='#35831#20351#29992#39640#20110' 260mA '#30340#30005#27969#12290
      'msgTimeLimit='#35831#20351#29992#19981#23569#20110' 30 '#31186#30340#26102#38388#12290
      'msgSave='#30830#23450#35201#20445#23384#21527#65311)
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
      18
      18)
  end
end
