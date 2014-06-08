inherited FixedFilterFrame: TFixedFilterFrame
  Width = 193
  Height = 242
  ExplicitWidth = 193
  ExplicitHeight = 242
  object TopPanel: TsPanel [0]
    AlignWithMargins = True
    Left = 2
    Top = 0
    Width = 189
    Height = 28
    Margins.Left = 2
    Margins.Top = 0
    Margins.Right = 2
    Margins.Bottom = 0
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    SkinData.SkinSection = 'CHECKBOX'
    DesignSize = (
      189
      28)
    object LoadButton: TsSpeedButton
      Left = 79
      Top = 2
      Width = 23
      Height = 23
      Anchors = [akTop, akRight]
      OnClick = LoadButtonClick
      SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    end
    object SaveButton: TsSpeedButton
      Left = 103
      Top = 2
      Width = 23
      Height = 23
      Anchors = [akTop, akRight]
      OnClick = SaveButtonClick
      SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    end
    object ShadeButton: TsSpeedButton
      Left = 127
      Top = 2
      Width = 23
      Height = 23
      AllowAllUp = True
      Anchors = [akTop, akRight]
      GroupIndex = 1
      Down = True
      OnClick = ShadeButtonClick
      SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
      ImageIndex = 0
      ShowCaption = False
    end
    object OpModeButton: TsSpeedButton
      Left = 55
      Top = 2
      Width = 23
      Height = 23
      Anchors = [akTop, akRight]
      OnClick = OpModeButtonClick
      SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
      Images = InterfaceModule.MenuImageList
    end
    object NoFilterCheckBox: TsCheckBox
      Left = 3
      Top = 5
      Width = 33
      Height = 20
      Caption = 'No'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = NoFilterCheckBoxClick
      SkinData.SkinSection = 'CHECKBOX'
      ImgChecked = 0
      ImgUnchecked = 0
    end
    object NumFiltersEdit: TsSpinEdit
      Left = 153
      Top = 4
      Width = 33
      Height = 21
      Anchors = [akTop, akRight]
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = '1'
      OnChange = NumFiltersEditChange
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
      MaxValue = 9
      MinValue = 1
      Value = 1
    end
  end
  inherited sFrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'BARPANEL'
    Left = 136
    Top = 376
  end
  object FilterMenu: TPopupMenu
    Left = 136
    Top = 40
    object GeoMenu: TMenuItem
      Caption = 'Geo'
      object Organics_num: TMenuItem
        Caption = 'Organics'
        OnClick = FilterMenuClick
      end
      object Energy_num: TMenuItem
        Caption = 'Energy'
        OnClick = FilterMenuClick
      end
      object Minerals_num: TMenuItem
        Caption = 'Minerals'
        OnClick = FilterMenuClick
      end
      object Temperature_num: TMenuItem
        Caption = 'Temperature'
        OnClick = FilterMenuClick
      end
      object Size_num: TMenuItem
        Caption = 'Size'
        Hint = 'planet.S'
        OnClick = FilterMenuClick
      end
    end
    object PositionMenu: TMenuItem
      Caption = 'Position'
      object Sector_str: TMenuItem
        Caption = 'Sector'
        OnClick = FilterMenuClick
      end
      object Y_num: TMenuItem
        Caption = 'X'
        OnClick = FilterMenuClick
      end
      object X_num: TMenuItem
        Caption = 'Y'
        OnClick = FilterMenuClick
      end
    end
    object OwnerMenu: TMenuItem
      Caption = 'Owner'
      object planets_owner: TMenuItem
        Caption = 'Name'
        Hint = 'Player name'
        OnClick = FilterMenuClick
      end
      object players_dip: TMenuItem
        Caption = 'Dip'
        OnClick = FilterMenuClick
      end
      object players_organization: TMenuItem
        Caption = 'organization'
        OnClick = FilterMenuClick
      end
    end
    object PlanetMenu: TMenuItem
      Caption = 'Planets'
      object planets_caption: TMenuItem
        Caption = 'caption'
        OnClick = FilterMenuClick
      end
      object planets_notes: TMenuItem
        Caption = 'Notes'
        OnClick = FilterMenuClick
      end
      object planets_icon1: TMenuItem
        Caption = 'icon1'
        OnClick = FilterMenuClick
      end
      object planets_icon2: TMenuItem
        Caption = 'icon2'
        OnClick = FilterMenuClick
      end
      object open_players: TMenuItem
        Caption = 'open'
        RadioItem = True
        OnClick = FilterMenuClick
      end
    end
    object ValueMenu: TMenuItem
      Caption = 'Value'
      object Growth_num: TMenuItem
        Caption = 'Growth'
        OnClick = FilterMenuClick
      end
      object Credits_num: TMenuItem
        Caption = 'Credits'
        OnClick = FilterMenuClick
      end
      object MiningBase_num: TMenuItem
        Caption = 'MiningBase'
        OnClick = FilterMenuClick
      end
      object CreditsBase_num: TMenuItem
        Caption = 'CreditsBase'
        OnClick = FilterMenuClick
      end
      object MiningSecond_num: TMenuItem
        Caption = 'MiningSecond'
        OnClick = FilterMenuClick
      end
      object CreditsSecond_num: TMenuItem
        Caption = 'CreditsSecond'
        OnClick = FilterMenuClick
      end
    end
    object Expression_custom: TMenuItem
      Caption = 'Expression'
      OnClick = FilterMenuClick
    end
  end
  object DelayTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = DelayTimerTimer
    Left = 72
    Top = 104
  end
end
