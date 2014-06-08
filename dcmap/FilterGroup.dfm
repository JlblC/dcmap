object FilterGroupFrame: TFilterGroupFrame
  Left = 0
  Top = 0
  Width = 451
  Height = 53
  Align = alTop
  ParentBackground = False
  TabOrder = 0
  TabStop = True
  object MarkerShape: TShape
    AlignWithMargins = True
    Left = 1
    Top = 14
    Width = 2
    Height = 37
    Margins.Left = 1
    Margins.Top = 14
    Margins.Right = 1
    Margins.Bottom = 2
    Align = alLeft
    Shape = stRoundRect
    ExplicitLeft = 0
    ExplicitTop = 11
    ExplicitHeight = 31
  end
  object ClientPanel: TsPanel
    Left = 4
    Top = 0
    Width = 447
    Height = 53
    Margins.Left = 5
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    SkinData.SkinManager = MainForm.sSkinManager
    SkinData.SkinSection = 'CHECKBOX'
    object HeaderPanel: TsPanel
      Left = 0
      Top = 0
      Width = 447
      Height = 22
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      SkinData.SkinManager = MainForm.sSkinManager
      SkinData.SkinSection = 'CHECKBOX'
      DesignSize = (
        447
        22)
      object CollapseButton: TsSpeedButton
        AlignWithMargins = True
        Left = 0
        Top = 3
        Width = 16
        Height = 16
        Margins.Left = 0
        Align = alLeft
        Caption = '-'
        Flat = True
        OnClick = CollapseButtonClick
        SkinData.SkinManager = MainForm.sSkinManager
        SkinData.SkinSection = 'SPEEDBUTTON'
        ExplicitLeft = 3
      end
      object AddBtn: TsSpeedButton
        AlignWithMargins = True
        Left = 410
        Top = 2
        Width = 34
        Height = 18
        Margins.Top = 2
        Margins.Bottom = 2
        Align = alRight
        Caption = '+'
        Flat = True
        OnMouseActivate = AddBtnMouseActivate
        ButtonStyle = tbsDropDown
        SkinData.SkinManager = MainForm.sSkinManager
        SkinData.SkinSection = 'SPEEDBUTTON'
        ExplicitLeft = 205
        ExplicitTop = 4
        ExplicitHeight = 20
      end
      object sLabel1: TsLabel
        Left = 30
        Top = 8
        Width = 36
        Height = 13
        Anchors = [akLeft, akTop, akRight]
        Caption = 'sLabel1'
        ParentFont = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
      end
    end
  end
  object sFrameAdapter1: TsFrameAdapter
    SkinData.SkinManager = MainForm.sSkinManager
    SkinData.SkinSection = 'CHECKBOX'
    Left = 208
    Top = 64
  end
end
