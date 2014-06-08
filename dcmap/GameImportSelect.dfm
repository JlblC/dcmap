inherited GameImportSelectForm: TGameImportSelectForm
  ActiveControl = PlayersList
  BorderIcons = [biSystemMenu]
  Caption = 'GameImportSelectForm'
  ClientHeight = 168
  ClientWidth = 362
  Constraints.MinHeight = 200
  Constraints.MinWidth = 370
  ExplicitWidth = 370
  ExplicitHeight = 200
  PixelsPerInch = 96
  TextHeight = 13
  object PlayersList: TsListView [0]
    AlignWithMargins = True
    Left = 5
    Top = 40
    Width = 352
    Height = 88
    Margins.Left = 5
    Margins.Top = 40
    Margins.Right = 5
    Margins.Bottom = 40
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
    Align = alClient
    Checkboxes = True
    Columns = <
      item
        Caption = 'Player'
        Width = 290
      end
      item
        Alignment = taCenter
        Caption = 'Turn'
      end>
    HoverTime = 0
    TabOrder = 0
    ViewStyle = vsReport
    OnDblClick = PlayersListDblClick
    OnDeletion = PlayersListDeletion
  end
  object SelectAllButton: TsBitBtn [1]
    Left = 8
    Top = 8
    Width = 89
    Height = 25
    Caption = 'SelectAll'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 1
    OnClick = SelectAllButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object SelectNoneButton: TsBitBtn [2]
    Left = 112
    Top = 8
    Width = 90
    Height = 25
    Caption = 'SelectNone'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 2
    OnClick = SelectNoneButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object ProcessButton: TsBitBtn [3]
    Left = 22
    Top = 135
    Width = 99
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Process'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 3
    OnClick = ProcessButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object CloseButton: TsBitBtn [4]
    Left = 260
    Top = 135
    Width = 94
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Close'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 4
    OnClick = CloseButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object ListAllCheckBox: TsCheckBox [5]
    Left = 218
    Top = 11
    Width = 94
    Height = 20
    Caption = 'ListAllCheckBox'
    TabOrder = 5
    OnClick = ListAllCheckBoxClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  inherited SkinProvider: TsSkinProvider
    GripMode = gmRightBottom
    Left = 168
    Top = 136
  end
end
