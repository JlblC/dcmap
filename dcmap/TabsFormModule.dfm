inherited TabsFrameForm: TTabsFrameForm
  Caption = 'TabsFrameForm'
  ClientHeight = 250
  ExplicitHeight = 302
  PixelsPerInch = 96
  TextHeight = 13
  object FrameTabs: TsTabControl [0]
    Left = 0
    Top = 0
    Width = 420
    Height = 250
    Align = alClient
    TabOrder = 0
    OnChange = FrameTabsChange
    OnMouseDown = FrameTabsMouseDown
    OnMouseMove = FrameTabsMouseMove
    OnMouseUp = FrameTabsMouseUp
    SkinData.SkinSection = 'PAGECONTROL'
  end
  inherited HintPanel: TsPanel
    Left = 128
    Top = 36
    TabOrder = 1
    ExplicitLeft = 128
    ExplicitTop = 36
  end
  inherited FrameMenu: TMainMenu
    Left = 240
    Top = 32
    object ViewMenu: TMenuItem
      Caption = 'View'
      GroupIndex = 80
      object NewTabMenu: TMenuItem
        Caption = 'Add Tab'
        OnClick = AddTabMenuClick
      end
      object CreateTabMenu: TMenuItem
        Caption = 'Create From Template'
        OnClick = CreateTabClick
        object StandartTabMenu: TMenuItem
          Caption = 'Default'
          OnClick = AddTabMenuClick
        end
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object CloseTabMenu: TMenuItem
        Caption = 'Close Tab'
        OnClick = CntCloseMenuItemClick
      end
      object RenameTabMenu: TMenuItem
        Caption = 'Rename Tab'
        OnClick = CntRenameMenuItemClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object SaveTabAsTemplateMenu: TMenuItem
        Caption = 'Save As Template'
        OnClick = CntSaveTemplateMenuItemClick
      end
    end
  end
  object TabMenu: TPopupMenu
    Images = InterfaceModule.MenuImageList
    Left = 56
    Top = 96
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
