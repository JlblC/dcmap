inherited FilterPanelFrame: TFilterPanelFrame
  Height = 291
  ExplicitHeight = 291
  inline FilterFrame1: TFilterFrame [0]
    Left = 0
    Top = 0
    Width = 177
    Height = 291
    HorzScrollBar.Visible = False
    VertScrollBar.Visible = False
    Align = alClient
    ParentBackground = False
    TabOrder = 0
    TabStop = True
    ExplicitWidth = 177
    ExplicitHeight = 291
    inherited sPanel1: TsPanel
      Width = 177
      ExplicitWidth = 177
    end
    inherited sScrollBox1: TsScrollBox
      Width = 177
      Height = 266
      ExplicitWidth = 177
      ExplicitHeight = 266
      inherited FilterGroupFrame1: TFilterGroupFrame
        Width = 177
        ExplicitWidth = 177
        inherited ClientPanel: TsPanel
          Width = 173
          ExplicitWidth = 173
          inherited HeaderPanel: TsPanel
            Width = 173
            ExplicitWidth = 173
            inherited AddBtn: TsSpeedButton
              Left = 136
              ExplicitLeft = 136
            end
          end
        end
      end
    end
  end
  inherited sFrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'BARPANEL'
    Left = 144
    Top = 56
  end
end
