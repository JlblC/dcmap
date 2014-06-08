inherited GridDataEditForm: TGridDataEditForm
  Left = 121
  Top = 89
  Caption = 'GridDataEditForm'
  ClientHeight = 316
  ClientWidth = 495
  Position = poDesigned
  ExplicitWidth = 503
  ExplicitHeight = 348
  PixelsPerInch = 96
  TextHeight = 13
  object Grid: TDrawGrid [0]
    Left = 0
    Top = 0
    Width = 495
    Height = 316
    Align = alClient
    DefaultRowHeight = 18
    RowCount = 2
    Options = [goVertLine, goHorzLine, goRangeSelect, goColSizing, goColMoving, goEditing, goTabs, goAlwaysShowEditor, goThumbTracking]
    TabOrder = 0
    OnColumnMoved = GridColumnMoved
    OnDrawCell = GridDrawCell
    OnGetEditText = GridGetEditText
    OnSetEditText = GridSetEditText
    ExplicitLeft = 16
    ExplicitTop = 15
    ExplicitWidth = 585
    ExplicitHeight = 231
  end
  inherited SkinProvider: TsSkinProvider
    Left = 400
    Top = 232
  end
end
