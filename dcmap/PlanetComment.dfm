inherited PlanetCommentFrame: TPlanetCommentFrame
  Width = 195
  Height = 109
  ExplicitWidth = 195
  ExplicitHeight = 109
  object CommentMemo: TsMemo [0]
    AlignWithMargins = True
    Left = 5
    Top = 5
    Width = 185
    Height = 99
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      #1053#1077#1090' '#1087#1083#1072#1085#1077#1090#1099' - '#1085#1077#1090' '#1082#1086#1084#1084#1077#1085#1090#1072#1088#1080#1077#1074)
    ParentFont = False
    TabOrder = 0
    OnChange = CommentMemoChange
    OnExit = CommentMemoExit
    Text = #1053#1077#1090' '#1087#1083#1072#1085#1077#1090#1099' - '#1085#1077#1090' '#1082#1086#1084#1084#1077#1085#1090#1072#1088#1080#1077#1074#13#10
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
  inherited sFrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'BARPANEL'
    Left = 104
    Top = 168
  end
end
