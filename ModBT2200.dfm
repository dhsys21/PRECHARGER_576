object Mod_BT2200: TMod_BT2200
  OldCreateOrder = False
  Height = 357
  Width = 658
  object ClientSocket_BT2200: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    Left = 144
    Top = 32
  end
  object SendTimer: TTimer
    Enabled = False
    Interval = 500
    Left = 145
    Top = 121
  end
end
