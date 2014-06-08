object MainForm2: TMainForm2
  Left = 0
  Top = 0
  Caption = 'MainForm2'
  ClientHeight = 402
  ClientWidth = 648
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object SkinProvider: TsSkinProvider
    SkinData.SkinSection = 'FORM'
    TitleButtons = <>
    Left = 280
    Top = 216
  end
  object MainMenu: TMainMenu
    Images = InterfaceModule.MenuImageList
    OwnerDraw = True
    Left = 400
    Top = 136
    object ViewMenu: TMenuItem
      Caption = #1042#1080#1076
      GroupIndex = 5
      object NewTabMenuItem: TMenuItem
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1074#1082#1083#1072#1076#1082#1091
      end
      object CreateTabMenu: TMenuItem
        Caption = #1057#1086#1079#1076#1072#1090#1100' '#1080#1079' '#1096#1072#1073#1083#1086#1085#1072
        object StandartTabMenuItem: TMenuItem
          Caption = #1057#1090#1072#1085#1076#1072#1088#1090#1085#1099#1081
        end
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object RenameMenuItem: TMenuItem
        Caption = 'Rename'
      end
      object CloseTabMenuItem: TMenuItem
        Caption = #1047#1072#1082#1088#1099#1090#1100' '#1074#1082#1083#1072#1076#1082#1091
      end
      object N22: TMenuItem
        Caption = '-'
      end
      object SetupTabMenuItem: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1080#1090#1100' '#1074#1082#1083#1072#1076#1082#1091'...'
      end
      object SaveTemplateMenuItem: TMenuItem
        Caption = 'Save As Template'
      end
    end
  end
end
