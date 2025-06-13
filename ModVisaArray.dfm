object Mod_VisaArray: TMod_VisaArray
  OldCreateOrder = False
  Height = 502
  Width = 771
  object Timer_CheckConnection: TTimer
    Enabled = False
    Interval = 2000
    OnTimer = Timer_CheckConnectionTimer
    Left = 71
    Top = 40
  end
  object Timer_EnableCharge: TTimer
    Enabled = False
    Interval = 100
    OnTimer = Timer_EnableChargeTimer
    Left = 255
    Top = 128
  end
  object Timer_InitCharge: TTimer
    Enabled = False
    Interval = 800
    OnTimer = Timer_InitChargeTimer
    Left = 367
    Top = 128
  end
  object Timer_GetMeas: TTimer
    Enabled = False
    Interval = 150
    Left = 255
    Top = 200
  end
  object Timer_GetReport: TTimer
    Enabled = False
    Interval = 50
    OnTimer = Timer_GetReportTimer
    Left = 367
    Top = 200
  end
  object Timer_SetCharge: TTimer
    Enabled = False
    Interval = 200
    OnTimer = Timer_SetChargeTimer
    Left = 71
    Top = 136
  end
  object Timer_GetInfo: TTimer
    Enabled = False
    Interval = 150
    OnTimer = Timer_GetInfoTimer
    Left = 71
    Top = 208
  end
  object Timer_EnableCharge2: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer_EnableCharge2Timer
    Left = 271
    Top = 56
  end
  object Timer_InitCharge2: TTimer
    Enabled = False
    OnTimer = Timer_InitCharge2Timer
    Left = 407
    Top = 56
  end
  object Timer_CheckConnection2: TTimer
    Enabled = False
    Interval = 20000
    OnTimer = Timer_CheckConnection2Timer
    Left = 167
    Top = 24
  end
end
