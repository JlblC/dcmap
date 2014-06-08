inherited ScriptEditFrame: TScriptEditFrame
  Width = 523
  Height = 246
  ExplicitWidth = 523
  ExplicitHeight = 246
  object Edit: TSynEdit [0]
    Left = 0
    Top = 22
    Width = 523
    Height = 224
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    PopupMenu = EditMenu
    TabOrder = 0
    Gutter.Font.Charset = DEFAULT_CHARSET
    Gutter.Font.Color = clWindowText
    Gutter.Font.Height = -11
    Gutter.Font.Name = 'Courier New'
    Gutter.Font.Style = []
    Gutter.Visible = False
    Gutter.Width = 0
    Highlighter = SynDcmSyn1
    Options = [eoAutoIndent, eoDragDropEditing, eoEnhanceEndKey, eoGroupUndo, eoHideShowScrollbars, eoSmartTabDelete, eoSmartTabs, eoTabIndent, eoTrimTrailingSpaces]
    RightEdge = 160
    TabWidth = 4
    WantTabs = True
  end
  object sToolBar1: TsToolBar [1]
    Left = 0
    Top = 0
    Width = 523
    Height = 22
    AutoSize = True
    Caption = 'sToolBar1'
    Images = InterfaceModule.MenuImageList
    TabOrder = 1
    SkinData.SkinSection = 'TOOLBAR'
    object OpenButton: TToolButton
      Left = 0
      Top = 0
      Caption = 'OpenButton'
      ImageIndex = 1
      OnClick = OpenButtonClick
    end
    object ToolButton1: TToolButton
      Left = 23
      Top = 0
      Width = 8
      Caption = 'ToolButton1'
      ImageIndex = 2
      Style = tbsSeparator
    end
    object SaveButton: TToolButton
      Left = 31
      Top = 0
      Caption = 'SaveButton'
      ImageIndex = 0
      OnClick = SaveButtonClick
    end
    object SaveAsButton: TToolButton
      Left = 54
      Top = 0
      Caption = 'SaveAsButton'
      ImageIndex = 2
      OnClick = SaveAsButtonClick
    end
    object ToolButton2: TToolButton
      Left = 77
      Top = 0
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 3
      Style = tbsSeparator
    end
    object CutButton: TToolButton
      Left = 85
      Top = 0
      Caption = 'Cut'
      MenuItem = CutMenuItem
    end
    object CopyButton: TToolButton
      Left = 108
      Top = 0
      Caption = 'Copy'
      MenuItem = CopyMenuItem
    end
    object PasteButton: TToolButton
      Left = 131
      Top = 0
      Caption = 'Paste'
      MenuItem = PasteMenuItem
    end
    object ToolButton3: TToolButton
      Left = 154
      Top = 0
      Width = 8
      Caption = 'ToolButton3'
      ImageIndex = 0
      Style = tbsSeparator
    end
    object UndoButton: TToolButton
      Left = 162
      Top = 0
      Caption = 'undo'
      MenuItem = UndoMenuItem
    end
    object RedoButton: TToolButton
      Left = 185
      Top = 0
      Caption = 'redo'
      MenuItem = RedoMenuItem
    end
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 80
  end
  object SynCompletionProposal1: TSynCompletionProposal
    Options = [scoCaseSensitive, scoLimitToMatchedText, scoEndCharCompletion, scoCompleteWithTab, scoCompleteWithEnter]
    EndOfTokenChr = '()[]. '
    TriggerChars = '()[]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clBtnText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = [fsBold]
    Columns = <>
    ShortCut = 16416
    Editor = Edit
    Left = 16
    Top = 40
  end
  object SynDcmSyn1: TSynDcmSyn
    IdentifierAttri.Foreground = clBlack
    KeyAttri.Foreground = clMaroon
    SymbolAttri.Foreground = clBlack
    Left = 136
    Top = 32
  end
  object EditMenu: TPopupMenu
    Images = InterfaceModule.MenuImageList
    Left = 304
    Top = 104
    object UndoMenuItem: TMenuItem
      Caption = 'undo'
      OnClick = UndoMenuItemClick
    end
    object RedoMenuItem: TMenuItem
      Caption = 'redo'
      OnClick = RedoMenuItemClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object CutMenuItem: TMenuItem
      Caption = 'Cut'
      OnClick = CutMenuItemClick
    end
    object CopyMenuItem: TMenuItem
      Caption = 'Copy'
      OnClick = CopyMenuItemClick
    end
    object PasteMenuItem: TMenuItem
      Caption = 'Paste'
      OnClick = PasteMenuItemClick
    end
  end
  object OpenDialog: TsOpenDialog
    DefaultExt = '.dcm'
    Filter = 
      'DCMap Scripts (*.dcm)|*.dcm|DCMap Files|*.dcm;*.ini;*.lng|All Fi' +
      'les|*.*'
    ZipShowing = zsAsFile
    Left = 376
    Top = 40
  end
  object SaveDialog: TsSaveDialog
    DefaultExt = '.dcm'
    Filter = 
      'DCMap Scripts (*.dcm)|*.dcm|DCMap Files|*.dcm;*.ini;*.lng|All Fi' +
      'les|*.*'
    Left = 432
    Top = 40
  end
  object SynIniSyn1: TSynIniSyn
    Left = 200
    Top = 32
  end
  object Search: TSynEditSearch
    Left = 256
    Top = 40
  end
  object RegexSearch: TSynEditRegexSearch
    Left = 320
    Top = 40
  end
  object OptionsDialog: TSynEditOptionsDialog
    UseExtendedStrings = False
    Left = 48
    Top = 104
  end
  object SynXMLSyn1: TSynXMLSyn
    WantBracesParsed = False
    Left = 184
    Top = 96
  end
  object FrameMenu: TMainMenu
    Left = 376
    Top = 104
    object i1: TMenuItem
      Caption = 'File'
      object New1: TMenuItem
        Caption = 'New'
      end
      object Open1: TMenuItem
        Caption = 'Open'
      end
      object Save1: TMenuItem
        Caption = 'Save'
      end
      object SaveAs1: TMenuItem
        Caption = 'Save As'
      end
    end
    object Edit1: TMenuItem
      Caption = 'Edit'
      object Undo1: TMenuItem
        Caption = 'Undo'
      end
      object Redo1: TMenuItem
        Caption = 'Redo'
      end
    end
  end
end
