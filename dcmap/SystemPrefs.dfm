inherited SystemPrefsFrame: TSystemPrefsFrame
  Width = 296
  Height = 153
  ExplicitWidth = 296
  ExplicitHeight = 153
  object ShowTrayIcon: TsCheckBox [0]
    Tag = 2
    Left = 16
    Top = 88
    Width = 89
    Height = 20
    Caption = 'ShowTrayIcon'
    BiDiMode = bdLeftToRight
    ParentBiDiMode = False
    TabOrder = 0
    OnClick = OnChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object MinimizeToTray: TsCheckBox [1]
    Tag = 2
    Left = 16
    Top = 111
    Width = 93
    Height = 20
    Caption = 'MinimizeToTray'
    BiDiMode = bdLeftToRight
    ParentBiDiMode = False
    TabOrder = 1
    OnClick = OnChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object AutoCheckUpdates: TsCheckBox [2]
    Tag = 1
    Left = 16
    Top = 40
    Width = 112
    Height = 20
    Caption = 'AutoCheckUpdates'
    TabOrder = 2
    OnClick = OnChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object WatchClipbrdForCmd: TsCheckBox [3]
    Tag = 1
    Left = 16
    Top = 16
    Width = 121
    Height = 20
    Caption = 'WatchClipbrdForCmd'
    TabOrder = 3
    OnClick = OnChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 232
  end
end
