inherited PreferencencesForm: TPreferencencesForm
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #1059#1089#1090#1072#1085#1086#1074#1082#1080
  ClientHeight = 370
  ClientWidth = 574
  Position = poOwnerFormCenter
  OnCloseQuery = FormCloseQuery
  ExplicitWidth = 580
  ExplicitHeight = 400
  DesignSize = (
    574
    370)
  PixelsPerInch = 96
  TextHeight = 13
  object TreeView: TsTreeView [0]
    AlignWithMargins = True
    Left = 8
    Top = 8
    Width = 162
    Height = 354
    Margins.Left = 8
    Margins.Top = 8
    Margins.Right = 0
    Margins.Bottom = 8
    Align = alLeft
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Indent = 19
    MultiSelectStyle = []
    ParentFont = False
    ReadOnly = True
    ShowButtons = False
    ShowRoot = False
    TabOrder = 0
    OnChange = TreeViewChange
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'EDIT'
  end
  object ClientPanel: TsPanel [1]
    AlignWithMargins = True
    Left = 178
    Top = 8
    Width = 388
    Height = 317
    Margins.Left = 8
    Margins.Top = 8
    Margins.Right = 8
    Margins.Bottom = 45
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    SkinData.SkinSection = 'CHECKBOX'
  end
  object ApplyButton: TsButton [2]
    Left = 319
    Top = 337
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100
    TabOrder = 2
    OnClick = ApplyButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object OkButton: TsButton [3]
    Left = 410
    Top = 337
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = #1054#1082
    Default = True
    ModalResult = 1
    TabOrder = 3
    SkinData.SkinSection = 'BUTTON'
  end
  object CloseButton: TsButton [4]
    Left = 491
    Top = 337
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 4
    SkinData.SkinSection = 'BUTTON'
  end
end
