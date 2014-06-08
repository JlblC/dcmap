inherited SecurityPrefsFrame: TSecurityPrefsFrame
  Width = 359
  Height = 275
  ExplicitWidth = 359
  ExplicitHeight = 275
  object MinutesLabel: TsLabel [0]
    Left = 220
    Top = 187
    Width = 62
    Height = 13
    Caption = 'MinutesLabel'
  end
  object NewPasswordCheck: TsEdit [1]
    Left = 10
    Top = 110
    Width = 159
    Height = 21
    PasswordChar = '*'
    TabOrder = 2
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'sEdit1'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object NewPassword: TsEdit [2]
    Left = 10
    Top = 71
    Width = 159
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'sEdit1'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object OldPassword: TsEdit [3]
    Left = 10
    Top = 24
    Width = 159
    Height = 21
    PasswordChar = '*'
    TabOrder = 0
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'sEdit1'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object ChangePassword: TsButton [4]
    Left = 62
    Top = 137
    Width = 107
    Height = 25
    Caption = 'ChangePassword'
    TabOrder = 3
    OnClick = ChangePasswordClick
    SkinData.SkinSection = 'BUTTON'
  end
  object RememberPasswordTime: TsSpinEdit [5]
    Tag = 1
    Left = 168
    Top = 184
    Width = 49
    Height = 21
    TabOrder = 4
    Text = '1'
    OnChange = OnChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'sSpinEdit1'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    MaxValue = 0
    MinValue = 0
    Value = 1
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 248
    Top = 72
  end
end
