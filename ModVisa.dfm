object Mod_Visa: TMod_Visa
  OldCreateOrder = False
  Height = 337
  Width = 678
  object Timer_CheckConnection: TTimer
    Enabled = False
    Interval = 3000
    OnTimer = Timer_CheckConnectionTimer
    Left = 71
    Top = 40
  end
  object Timer_EnableCharge: TTimer
    Enabled = False
    Interval = 100
    OnTimer = Timer_EnableChargeTimer
    Left = 175
    Top = 136
  end
  object Timer_InitCharge: TTimer
    Enabled = False
    Interval = 800
    OnTimer = Timer_InitChargeTimer
    Left = 287
    Top = 136
  end
  object Timer_GetMeas: TTimer
    Enabled = False
    Interval = 150
    OnTimer = Timer_GetMeasTimer
    Left = 191
    Top = 208
  end
  object Timer_GetReport: TTimer
    Enabled = False
    Interval = 50
    OnTimer = Timer_GetReportTimer
    Left = 303
    Top = 208
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
end
