object BaseModuleFrame: TBaseModuleFrame
  Left = 0
  Top = 0
  Width = 177
  Height = 205
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  ParentBackground = False
  ParentFont = False
  TabOrder = 0
  TabStop = True
  OnResize = FrameResize
  object sFrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'CHECKBOX'
    Left = 40
    Top = 32
  end
end
