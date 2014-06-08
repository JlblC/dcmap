inherited DBFrefsFrame: TDBFrefsFrame
  Width = 330
  Height = 168
  ExplicitWidth = 330
  ExplicitHeight = 168
  object BackupsGroupBox: TsGroupBox [0]
    Left = 16
    Top = 18
    Width = 297
    Height = 98
    Caption = 'BackupsGroupBox'
    TabOrder = 0
    SkinData.SkinSection = 'GROUPBOX'
    object UseHardlinks: TsCheckBox
      Tag = 1
      Left = 10
      Top = 69
      Width = 81
      Height = 20
      Caption = 'UseHardlinks'
      TabOrder = 0
      OnClick = OnChange
      SkinData.SkinSection = 'CHECKBOX'
      ImgChecked = 0
      ImgUnchecked = 0
    end
    object UseBackups: TsCheckBox
      Tag = 1
      Left = 10
      Top = 22
      Width = 77
      Height = 20
      Caption = 'UseBackups'
      TabOrder = 1
      OnClick = UseBackupsClick
      SkinData.SkinSection = 'CHECKBOX'
      ImgChecked = 0
      ImgUnchecked = 0
    end
    object NumBackups: TsSpinEdit
      Tag = 1
      Left = 125
      Top = 45
      Width = 65
      Height = 21
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = '3'
      OnChange = OnChange
      SkinData.SkinSection = 'EDIT'
      BoundLabel.Active = True
      BoundLabel.Caption = 'NumBackups'
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = DEFAULT_CHARSET
      BoundLabel.Font.Color = clBlack
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      MaxValue = 100
      MinValue = 1
      Value = 3
    end
  end
  object CompressDatabase: TsCheckBox [1]
    Tag = 1
    Left = 16
    Top = 122
    Width = 113
    Height = 20
    Caption = 'CompressDatabase'
    TabOrder = 1
    OnClick = OnChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 280
  end
end
