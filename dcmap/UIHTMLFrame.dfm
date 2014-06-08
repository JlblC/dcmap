inherited HTMLFrame: THTMLFrame
  Width = 449
  Height = 300
  ExplicitWidth = 449
  ExplicitHeight = 300
  object Html: THTMLViewer [0]
    AlignWithMargins = True
    Left = 4
    Top = 4
    Width = 441
    Height = 292
    Cursor = crDefault
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    OnHotSpotClick = HtmlHotSpotClick
    OnScript = HtmlScript
    OnFormSubmit = htmlFormSubmit
    TabOrder = 0
    Align = alClient
    BorderStyle = htNone
    HistoryMaxCount = 0
    DefFontName = 'Times New Roman'
    DefPreFontName = 'Courier New'
    DefFontSize = 8
    DefOverLinkColor = clAqua
    NoSelect = True
    CharSet = RUSSIAN_CHARSET
    PrintMarginLeft = 2.000000000000000000
    PrintMarginRight = 2.000000000000000000
    PrintMarginTop = 2.000000000000000000
    PrintMarginBottom = 2.000000000000000000
    PrintScale = 1.000000000000000000
    OnProcessing = HtmlProcessing
    OnInclude = HtmlInclude
    OnMeta = htmlMeta
    OnObjectClick = htmlObjectAction
    OnObjectFocus = htmlObjectAction
    OnObjectBlur = htmlObjectAction
    OnObjectChange = htmlObjectAction
  end
  inherited sFrameAdapter: TsFrameAdapter
    Top = 16
  end
end
