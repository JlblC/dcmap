object ScriptEditFrame: TScriptEditFrame
  Left = 0
  Top = 0
  Width = 278
  Height = 118
  TabOrder = 0
  TabStop = True
  object Edit: TSynEdit
    Left = 0
    Top = 0
    Width = 278
    Height = 118
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    TabOrder = 0
    Gutter.Font.Charset = DEFAULT_CHARSET
    Gutter.Font.Color = clWindowText
    Gutter.Font.Height = -11
    Gutter.Font.Name = 'Courier New'
    Gutter.Font.Style = []
    Highlighter = SynGeneralSyn1
    Options = [eoAutoIndent, eoDragDropEditing, eoEnhanceEndKey, eoGroupUndo, eoShowScrollHint, eoSmartTabDelete, eoSmartTabs, eoTabIndent, eoTrimTrailingSpaces]
    RightEdge = 160
    TabWidth = 4
    WantTabs = True
    ExplicitWidth = 687
    ExplicitHeight = 507
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
    Left = 56
    Top = 16
    EndOfTokenChrW = '()[]. '
    TriggerCharsW = '()[]'
  end
  object SynGeneralSyn1: TSynGeneralSyn
    CommentAttri.Foreground = clGreen
    Comments = []
    DetectPreprocessor = True
    IdentifierAttri.Foreground = clBlack
    IdentifierChars = '$_0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'
    KeyAttri.Foreground = clBlue
    KeyWords.UnicodeStrings = 
      '$_'#13#10'abs'#13#10'acos'#13#10'advance_progress'#13#10'and'#13#10'arccos'#13#10'arcsin'#13#10'arctg'#13#10'asi' +
      'n'#13#10'atan'#13#10'break'#13#10'cancel'#13#10'ceil'#13#10'clbrd'#13#10'clear_console'#13#10'clipboard'#13#10'c' +
      'onsole_caption'#9#13#10'console_status'#13#10'continue'#13#10'cos'#13#10'die'#13#10'echo'#13#10'edit_' +
      'table'#13#10'else'#13#10'execute'#13#10'exp'#13#10'floor'#13#10'for'#13#10'foreach'#13#10'game_get'#13#10'game_l' +
      'ogin'#13#10'game_logout'#9#13#10'game_post'#13#10'hide_console'#13#10'if'#13#10'insert'#13#10'iterate' +
      '_all'#13#10'iterate_child'#13#10'let'#13#10'loadfile'#13#10'loadfile_utf8'#13#10'message'#13#10'my'#13#10 +
      'nil'#13#10'or'#13#10'pause'#13#10'post'#13#10'proc'#13#10'push'#13#10'q'#13#10'qq'#13#10'retrieve'#13#10'return'#13#10'save_' +
      'db'#13#10'savefile'#13#10'savefile_utf8'#13#10'search'#13#10'select'#13#10'select_path'#13#10'set'#13#10's' +
      'etup_progress'#13#10'show_console'#13#10'sin'#13#10'split'#13#10'sqrt'#13#10'sub'#13#10'sysexit'#13#10'tab' +
      'le'#13#10'tan'#13#10'test_cancel'#13#10'tg'#13#10'update'#13#10'urlencode'#13#10'webget'#13#10'webpost'#13#10'wh' +
      'ile'#13#10'xmlunqoute'#13#10'xor'
    NumberAttri.Foreground = clNavy
    PreprocessorAttri.Foreground = clGreen
    PreprocessorAttri.Style = [fsItalic]
    StringAttri.Foreground = clPurple
    SymbolAttri.Foreground = clPurple
    SymbolAttri.Style = [fsBold]
    Left = 96
    Top = 16
    IdentifierCharsW = '$_0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'
  end
end
