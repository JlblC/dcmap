inherited ConsoleForm: TConsoleForm
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = 'ConsoleForm'
  ClientHeight = 426
  ClientWidth = 637
  PopupMode = pmAuto
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = TntFormShow
  ExplicitWidth = 645
  ExplicitHeight = 455
  PixelsPerInch = 96
  TextHeight = 13
  object sSplitter1: TsSplitter [0]
    Left = 0
    Top = 129
    Width = 637
    Height = 6
    Cursor = crVSplit
    Align = alTop
    SkinData.SkinSection = 'SPLITTER'
    ExplicitTop = 185
    ExplicitWidth = 263
  end
  object ResultMemo: TsMemo [1]
    Left = 0
    Top = 0
    Width = 637
    Height = 129
    Align = alTop
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
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
  object sPanel1: TsPanel [2]
    Left = 0
    Top = 385
    Width = 637
    Height = 41
    Align = alBottom
    TabOrder = 1
    SkinData.SkinSection = 'CHECKBOX'
    object ExecuteButton: TsButton
      Left = 16
      Top = 6
      Width = 92
      Height = 25
      Caption = 'ExecuteButton'
      TabOrder = 0
      OnClick = ExecuteButtonClick
      SkinData.SkinSection = 'BUTTON'
    end
  end
  inline ScriptEditFrame: TScriptEditFrame [3]
    Left = 0
    Top = 135
    Width = 637
    Height = 250
    HorzScrollBar.Visible = False
    VertScrollBar.Visible = False
    Align = alClient
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 2
    TabStop = True
    ExplicitTop = 135
    ExplicitWidth = 637
    ExplicitHeight = 250
    inherited Edit: TSynEdit
      Width = 637
      Height = 228
      ExplicitWidth = 637
      ExplicitHeight = 228
    end
    inherited sToolBar1: TsToolBar
      Width = 637
      ExplicitWidth = 637
    end
  end
end
