inherited RegExpImpPrefsFrame: TRegExpImpPrefsFrame
  Width = 385
  Height = 397
  ExplicitWidth = 385
  ExplicitHeight = 397
  object DeleteButton: TsButton [0]
    Left = 294
    Top = 38
    Width = 75
    Height = 25
    Caption = 'Button'
    TabOrder = 4
    OnClick = DeleteButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object NameEdit: TsEdit [1]
    Left = 200
    Top = 100
    Width = 169
    Height = 21
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = 'NameEdit'
    OnChange = NameEditChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'Name'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  inline ScriptEditFrame: TScriptEditFrame [2]
    Left = 16
    Top = 151
    Width = 353
    Height = 234
    HorzScrollBar.Visible = False
    VertScrollBar.Visible = False
    Anchors = [akLeft, akTop, akBottom]
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 5
    TabStop = True
    OnResize = FrameResize
    ExplicitLeft = 16
    ExplicitTop = 151
    ExplicitWidth = 353
    ExplicitHeight = 234
    inherited Edit: TSynEdit
      Width = 353
      Height = 212
      ExplicitWidth = 353
      ExplicitHeight = 212
    end
    inherited sToolBar1: TsToolBar
      Width = 353
      ExplicitWidth = 353
    end
  end
  object AddButton: TsButton
    Left = 200
    Top = 38
    Width = 73
    Height = 25
    Caption = 'Add'
    TabOrder = 3
    OnClick = AddButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object BlockComboBox: TsComboBox
    Left = 88
    Top = 11
    Width = 209
    Height = 21
    Alignment = taLeftJustify
    BoundLabel.Active = True
    BoundLabel.Caption = 'BlockComboBox'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Style = csDropDownList
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 0
    ItemIndex = -1
    ParentFont = False
    TabOrder = 0
    OnChange = BlockComboBoxChange
  end
  object ConfListBox: TsListBox
    Left = 16
    Top = 38
    Width = 169
    Height = 99
    AutoCompleteDelay = 500
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
    ItemHeight = 13
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = ConfListBoxClick
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 336
    Top = 8
  end
end
