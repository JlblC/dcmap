object FixedFilterDBFrame: TFixedFilterDBFrame
  AlignWithMargins = True
  Left = 0
  Top = 0
  Width = 200
  Height = 54
  Margins.Left = 2
  Margins.Top = 0
  Margins.Right = 2
  Margins.Bottom = 0
  ParentBackground = False
  TabOrder = 0
  TabStop = True
  DesignSize = (
    200
    54)
  object ResourceSelBtn: TsSpeedButton
    Left = 66
    Top = 2
    Width = 131
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    OnMouseActivate = ResourceSelBtnMouseActivate
    Alignment = taLeftJustify
    ButtonStyle = tbsDropDown
    SkinData.SkinSection = 'COMBOBOX'
    TextOffset = 4
    TextAlignment = taLeftJustify
  end
  object ModeButton: TsSpeedButton
    Left = 45
    Top = 2
    Width = 20
    Height = 20
    OnClick = ModeButtonClick
    SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    Images = InterfaceModule.MenuImageList
  end
  object ComboEdit: TsComboEdit
    Left = 3
    Top = 26
    Width = 194
    Height = 22
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Visible = False
    OnChange = DataChange
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
    OnButtonClick = ComboEditButtonClick
    GlyphMode.Blend = 0
    GlyphMode.Grayed = False
  end
  object DBValueComboBox: TsComboBox
    Left = 52
    Top = 27
    Width = 146
    Height = 21
    AutoCloseUp = True
    AutoDropDown = True
    Anchors = [akLeft, akTop, akRight]
    Alignment = taLeftJustify
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Style = csDropDownList
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 0
    ItemIndex = -1
    ParentFont = False
    Sorted = True
    TabOrder = 0
    Visible = False
    OnChange = DataChange
  end
  object OperatorComboBox: TsComboBox
    Left = 3
    Top = 26
    Width = 45
    Height = 21
    Alignment = taLeftJustify
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Style = csDropDownList
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 13
    ItemIndex = -1
    ParentFont = False
    TabOrder = 1
    OnChange = DataChange
    Items.Strings = (
      '='
      '<>'
      '>='
      '<='
      '<'
      '>')
  end
  object UseCheck: TsCheckBox
    Left = 1
    Top = 3
    Width = 45
    Height = 20
    Caption = #1055#1086#1083#1077
    AutoSize = False
    TabOrder = 2
    OnClick = UseCheckClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object NumEdit: TsDecimalSpinEdit
    Left = 52
    Top = 26
    Width = 146
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = '0,00'
    Visible = False
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Increment = 1.000000000000000000
  end
  object StringEdit: TsEdit
    Left = 52
    Top = 26
    Width = 146
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Visible = False
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object sFrameAdapter1: TsFrameAdapter
    SkinData.SkinSection = 'CHECKBOX'
    Left = 56
    Top = 120
  end
end
