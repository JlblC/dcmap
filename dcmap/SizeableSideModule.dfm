inherited SizeableSideFrame: TSizeableSideFrame
  object BottomSizeBar: TsDragBar [0]
    Left = 0
    Top = 200
    Width = 177
    Height = 5
    Cursor = crSizeNS
    Align = alBottom
    BevelOuter = bvNone
    Caption = ' '
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clCaptionText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    ShowCaption = False
    TabOrder = 0
    OnMouseDown = BottomSizeBarMouseDown
    OnMouseMove = BottomSizeBarMouseMove
    SkinData.SkinSection = 'SPLITTER'
    ExplicitTop = 201
  end
end
