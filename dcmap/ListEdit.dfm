inherited ListEditForm: TListEditForm
  Left = 305
  Top = 198
  Caption = 'ListEditForm'
  ClientHeight = 287
  ClientWidth = 516
  Constraints.MinHeight = 317
  Constraints.MinWidth = 520
  Font.Charset = RUSSIAN_CHARSET
  ExplicitWidth = 522
  ExplicitHeight = 317
  DesignSize = (
    516
    287)
  PixelsPerInch = 96
  TextHeight = 13
  object DownBtn: TsSpeedButton [0]
    Left = 139
    Top = 257
    Width = 23
    Height = 22
    Anchors = [akLeft, akBottom]
    Flat = True
    ParentShowHint = False
    ShowHint = True
    OnClick = DownBtnClick
    SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    ExplicitTop = 260
  end
  object UpBtn: TsSpeedButton [1]
    Left = 110
    Top = 257
    Width = 23
    Height = 22
    Anchors = [akLeft, akBottom]
    Flat = True
    ParentShowHint = False
    ShowHint = True
    OnClick = UpBtnClick
    SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    ExplicitTop = 260
  end
  inherited DataCopyButton: TsSpeedButton
    Left = 401
    Top = 12
    ExplicitLeft = 401
    ExplicitTop = 12
  end
  inherited ContextButton: TsSpeedButton
    Left = 468
    Top = 12
    Anchors = [akTop, akRight]
    ExplicitLeft = 496
    ExplicitTop = 12
  end
  object AddButton: TsButton [4]
    Left = 172
    Top = 12
    Width = 81
    Height = 27
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100
    TabOrder = 1
    OnClick = AddButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object DeleteButton: TsButton [5]
    Left = 270
    Top = 12
    Width = 75
    Height = 27
    Caption = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 2
    OnClick = DeleteButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object ApplyButton: TsButton [6]
    Left = 172
    Top = 254
    Width = 81
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100
    ParentShowHint = False
    ShowHint = True
    TabOrder = 3
    OnClick = ApplyButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object CloseButton: TsButton [7]
    Left = 434
    Top = 254
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 6
    SkinData.SkinSection = 'BUTTON'
  end
  object CanselButton: TsButton [8]
    Left = 264
    Top = 254
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = #1054#1090#1084#1077#1085#1080#1090#1100
    TabOrder = 4
    OnClick = CanselButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object OkButton: TsButton [9]
    Left = 348
    Top = 254
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Ok'
    Default = True
    ModalResult = 1
    TabOrder = 5
    SkinData.SkinSection = 'BUTTON'
  end
  object MaskEdit: TsEdit [10]
    Left = 8
    Top = 18
    Width = 154
    Height = 21
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnChange = MaskEditChange
    OnKeyDown = MaskEditKeyDown
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1055#1086#1080#1089#1082
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 2171169
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object SortCheck: TsCheckBox [11]
    Left = 8
    Top = 257
    Width = 87
    Height = 20
    Caption = #1057#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100
    Anchors = [akLeft, akBottom]
    Checked = True
    State = cbChecked
    TabOrder = 7
    OnClick = SortCheckClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object DataList: TsListView [12]
    Left = 8
    Top = 45
    Width = 154
    Height = 199
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
    Anchors = [akLeft, akTop, akBottom]
    Color = clWhite
    Columns = <
      item
        MaxWidth = 140
        Width = 135
      end>
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    HideSelection = False
    ReadOnly = True
    RowSelect = True
    ParentFont = False
    ShowColumnHeaders = False
    TabOrder = 10
    ViewStyle = vsReport
    OnClick = _PlayerListBoxClick
    OnKeyDown = _PlayerListBoxKeyDown
    OnSelectItem = DataListSelectItem
  end
  inherited DataPasteButton: TsBitBtn
    Left = 357
    Top = 12
    TabOrder = 8
    ExplicitLeft = 357
    ExplicitTop = 12
  end
  inherited PropPageFrame: TPropPageFrame
    Left = 172
    Width = 337
    Height = 187
    TabOrder = 9
    ExplicitLeft = 172
    ExplicitWidth = 337
    ExplicitHeight = 187
    inherited Tabs: TsTabControl
      Width = 337
      Height = 187
      ExplicitWidth = 337
      ExplicitHeight = 187
      inherited ClientPanel: TsPanel
        Width = 329
        Height = 174
        ExplicitWidth = 329
        ExplicitHeight = 174
      end
    end
  end
  object DelayTimer: TTimer
    Enabled = False
    Interval = 800
    OnTimer = DelayTimerTimer
    Left = 120
    Top = 8
  end
end
