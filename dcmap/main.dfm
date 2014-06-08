inherited MainForm: TMainForm
  Left = 60
  Top = 131
  Caption = 'MainForm'
  ClientHeight = 223
  ClientWidth = 357
  Menu = MainMenu
  OldCreateOrder = True
  OnCreate = FormCreate
  OnDeactivate = FormDeactivate
  OnHide = FormHide
  ExplicitWidth = 365
  ExplicitHeight = 277
  PixelsPerInch = 96
  TextHeight = 13
  object sTabs: TsTabControl [0]
    Left = 0
    Top = 0
    Width = 357
    Height = 223
    Align = alClient
    TabOrder = 1
    OnChange = sTabsChange
    OnMouseDown = sTabsMouseDown
    OnMouseMove = sTabsMouseMove
    OnMouseUp = sTabsMouseUp
    OnStartDrag = sTabsStartDrag
    SkinData.SkinSection = 'PAGECONTROL'
    object HintPlace: TsPanel
      Left = 208
      Top = 28
      Width = 32
      Height = 31
      BevelOuter = bvNone
      TabOrder = 0
      SkinData.SkinSection = 'CHECKBOX'
    end
  end
  inherited SkinProvider: TsSkinProvider
    Left = 32
  end
  inherited FrameMenu: TMainMenu
    Left = 224
    Top = 24
  end
  object MainMenu: TMainMenu
    Left = 144
    Top = 24
    object ViewMenu: TMenuItem
      Caption = #1042#1080#1076
      GroupIndex = 80
      object NewTabMenuItem: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1074#1082#1083#1072#1076#1082#1091
        OnClick = NewTabMenuItemClick
      end
      object CreateTabMenu: TMenuItem
        Caption = #1057#1086#1079#1076#1072#1090#1100' '#1080#1079' '#1096#1072#1073#1083#1086#1085#1072
        OnClick = CreateTabMenuClick
        object StandartTabMenuItem: TMenuItem
          Caption = #1057#1090#1072#1085#1076#1072#1088#1090#1085#1099#1081
          OnClick = NewTabMenuItemClick
        end
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object RenameMenuItem: TMenuItem
        Caption = 'Rename'
        OnClick = CntRenameMenuItemClick
      end
      object CloseTabMenuItem: TMenuItem
        Caption = #1047#1072#1082#1088#1099#1090#1100' '#1074#1082#1083#1072#1076#1082#1091
        OnClick = CloseTabMenuItemClick
      end
      object N22: TMenuItem
        Caption = '-'
      end
      object SetupTabMenuItem: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1080#1090#1100' '#1074#1082#1083#1072#1076#1082#1091'...'
        OnClick = SetupTabMenuItemClick
      end
      object SaveTemplateMenuItem: TMenuItem
        Caption = 'Save As Template'
        OnClick = CntSaveTemplateMenuItemClick
      end
    end
  end
  object TabMenu: TPopupMenu
    Images = InterfaceModule.MenuImageList
    Left = 88
    Top = 24
    object CntCloseMenuItem: TMenuItem
      Caption = #1047#1072#1082#1088#1099#1090#1100
      OnClick = CntCloseMenuItemClick
    end
    object CntRenameMenuItem: TMenuItem
      Caption = #1055#1077#1088#1077#1080#1084#1077#1085#1086#1074#1072#1090#1100
      OnClick = CntRenameMenuItemClick
    end
    object CntSaveTemplateMenuItem: TMenuItem
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1082#1072#1082' '#1096#1072#1073#1083#1086#1085'...'
      OnClick = CntSaveTemplateMenuItemClick
    end
  end
end
