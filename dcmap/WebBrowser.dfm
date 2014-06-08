inherited WebBrowserFrame: TWebBrowserFrame
  Width = 451
  Height = 304
  Align = alClient
  ExplicitWidth = 451
  ExplicitHeight = 304
  object StatusBar: TsStatusBar [0]
    Left = 0
    Top = 285
    Width = 451
    Height = 19
    Panels = <
      item
        Bevel = pbNone
        Width = 180
      end
      item
        Width = 50
      end>
    SkinData.SkinSection = 'STATUSBAR'
    object ProgressGauge: TsGauge
      AlignWithMargins = True
      Left = 2
      Top = 2
      Width = 175
      Height = 15
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Align = alLeft
      Visible = False
      SkinData.SkinSection = 'GAUGE'
      BackColor = clBtnFace
      ForeColor = clBtnShadow
      Suffix = '%'
    end
  end
  object sToolBar1: TsToolBar [1]
    Left = 0
    Top = 0
    Width = 451
    Height = 22
    ButtonHeight = 18
    Caption = 'sToolBar1'
    TabOrder = 1
    SkinData.SkinSection = 'CHECKBOX'
    object BackBtn: TToolButton
      Left = 0
      Top = 0
      Caption = 'BackBtn'
      OnClick = BackBtnClick
    end
    object ForwardBtn: TToolButton
      Left = 23
      Top = 0
      Caption = 'ForwardBtn'
      OnClick = ForwardBtnClick
    end
    object ReloadBtn: TToolButton
      Left = 46
      Top = 0
      Caption = 'ReloadBtn'
      OnClick = ReloadBtnClick
    end
    object LoginBtn: TToolButton
      Left = 69
      Top = 0
      Caption = 'LoginBtn'
      DropdownMenu = LoginMenu
      ImageIndex = 0
    end
    object UrlBox: TsComboBox
      Left = 92
      Top = 0
      Width = 323
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
      ItemHeight = 13
      ItemIndex = -1
      TabOrder = 0
      Text = 'UrlBox'
      OnKeyDown = UrlBoxKeyDown
    end
    object GoBtn: TToolButton
      Left = 415
      Top = 0
      Caption = 'GoBtn'
      OnClick = GoBtnClick
    end
    object UserNameLabel: TsLabel
      Left = 438
      Top = 0
      Width = 119
      Height = 18
      Caption = 'UserNameLabel'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
  end
  object sPanel1: TsPanel [2]
    Left = 0
    Top = 22
    Width = 451
    Height = 263
    Align = alClient
    BevelOuter = bvLowered
    Caption = 'sPanel1'
    TabOrder = 2
    SkinData.SkinSection = 'PANEL_LOW'
    object WB: TEmbeddedWB
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 445
      Height = 257
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Align = alClient
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      Silent = False
      OnStatusTextChange = WBStatusTextChange
      OnProgressChange = WBProgressChange
      OnBeforeNavigate2 = WBBeforeNavigate2
      OnNavigateComplete2 = WBNavigateComplete2
      DisableCtrlShortcuts = 'N'
      UserInterfaceOptions = [DontUse3DBorders, ForceFlatScrollBars, EnablesFormsAutoComplete, EnableThemes, DisableLocalFileAccess]
      OnEvaluateNewWindow = WBEvaluateNewWindow
      About = ' EmbeddedWB http://bsalsa.com/'
      PrintOptions.Margins.Left = 19.050000000000000000
      PrintOptions.Margins.Right = 19.050000000000000000
      PrintOptions.Margins.Top = 19.050000000000000000
      PrintOptions.Margins.Bottom = 19.050000000000000000
      PrintOptions.Header = '&w&bPage &p of &P'
      PrintOptions.HTMLHeader.Strings = (
        '<HTML></HTML>')
      PrintOptions.Footer = '&u&b&d'
      PrintOptions.Orientation = poPortrait
      UserAgent = 'DCMap/1.29'
      UserAgentMode = uaInternal
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 451
      ExplicitHeight = 261
      ControlData = {
        4C000000AF2D0000E81B00000000000000000000000000000000000000000000
        000000004C000000000000000000000001000000E0D057007335CF11AE690800
        2B2E126208000000000000004C0000000114020000000000C000000000000046
        8000000000000000000000000000000000000000000000000000000000000000
        00000000000000000100000000000000000000000000000000000000}
    end
  end
  inherited FrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'CHECKBOX'
    Left = 352
    Top = 144
  end
end
