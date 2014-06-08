inherited FilterFrame: TFilterFrame
  Width = 259
  Height = 360
  ExplicitWidth = 259
  ExplicitHeight = 360
  object sPanel1: TsPanel [0]
    Left = 0
    Top = 0
    Width = 259
    Height = 25
    Align = alTop
    Caption = 'sPanel1'
    TabOrder = 0
    SkinData.SkinManager = MainForm.sSkinManager
    SkinData.SkinSection = 'PANEL'
  end
  object sScrollBox1: TsScrollBox [1]
    Left = 0
    Top = 25
    Width = 259
    Height = 335
    HorzScrollBar.Visible = False
    VertScrollBar.Style = ssHotTrack
    VertScrollBar.Tracking = True
    Align = alClient
    BorderStyle = bsNone
    TabOrder = 1
    SkinData.SkinManager = MainForm.sSkinManager
    SkinData.SkinSection = 'CHECKBOX'
    inline FilterGroupFrame1: TFilterGroupFrame
      Left = 0
      Top = 0
      Width = 259
      Height = 53
      Align = alTop
      ParentBackground = False
      TabOrder = 0
      TabStop = True
      ExplicitWidth = 259
      inherited ClientPanel: TsPanel
        Width = 255
        ExplicitWidth = 255
        inherited HeaderPanel: TsPanel
          Width = 255
          ExplicitWidth = 255
          inherited AddBtn: TsSpeedButton
            Left = 218
            ExplicitLeft = 218
          end
          inherited sLabel1: TsLabel
            Width = 36
          end
        end
      end
    end
  end
  inherited sFrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'BARPANEL'
    Left = 192
    Top = 240
  end
  object AddMenu: TPopupMenu
    Left = 192
    Top = 184
    object N1: TMenuItem
      Caption = #1043#1088#1091#1087#1087#1072
      OnClick = N1Click
    end
  end
end
