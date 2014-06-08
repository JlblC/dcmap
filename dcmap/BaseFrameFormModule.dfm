inherited BaseFrameForm: TBaseFrameForm
  Caption = 'DCMap'
  ClientHeight = 296
  ClientWidth = 420
  Menu = FrameMenu
  Position = poDefault
  OnActivate = FormActivate
  OnClose = FormClose
  OnShow = FormShow
  ExplicitWidth = 428
  ExplicitHeight = 328
  PixelsPerInch = 96
  TextHeight = 13
  object HintPanel: TsPanel [0]
    Left = 184
    Top = 84
    Width = 32
    Height = 31
    BevelOuter = bvNone
    TabOrder = 0
    Visible = False
    SkinData.SkinSection = 'CHECKBOX'
  end
  inherited SkinProvider: TsSkinProvider
    MakeSkinMenu = True
  end
  object FrameMenu: TMainMenu
    Left = 328
    Top = 56
  end
end
