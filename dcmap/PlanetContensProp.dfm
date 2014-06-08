inherited PlanetGarrisonFrame: TPlanetGarrisonFrame
  Width = 307
  Height = 343
  ExplicitWidth = 307
  ExplicitHeight = 343
  object AddToQueueBtn: TsSpeedButton [0]
    Left = 3
    Top = 314
    Width = 142
    Height = 23
    Caption = 'Add To Queue'
    ButtonStyle = tbsDropDown
    SkinData.SkinSection = 'SPEEDBUTTON'
    DropdownMenu = ProjectsMenu
  end
  object BuildingsView: TsListView [1]
    Left = 3
    Top = 14
    Width = 142
    Height = 150
    BoundLabel.Active = True
    BoundLabel.Caption = 'BuildingsView'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'EDIT'
    AllocBy = 16
    Columns = <
      item
        Width = 120
      end>
    ShowColumnHeaders = False
    TabOrder = 0
    ViewStyle = vsReport
    OnDeletion = BuildingsViewDeletion
  end
  object GarrisonView: TsListView [2]
    Left = 151
    Top = 14
    Width = 146
    Height = 150
    BoundLabel.Active = True
    BoundLabel.Caption = 'GarrisonView'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'EDIT'
    AllocBy = 16
    Columns = <
      item
        Width = 120
      end>
    ShowColumnHeaders = False
    TabOrder = 1
    ViewStyle = vsReport
    OnDeletion = BuildingsViewDeletion
  end
  object QueueView: TsListView [3]
    Left = 3
    Top = 181
    Width = 142
    Height = 130
    BoundLabel.Active = True
    BoundLabel.Caption = 'QueueView'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'EDIT'
    AllocBy = 16
    Columns = <
      item
        Width = 120
      end>
    ShowColumnHeaders = False
    TabOrder = 2
    ViewStyle = vsReport
    OnDeletion = BuildingsViewDeletion
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 144
    Top = 320
  end
  object ProjectsMenu: TPopupMenu
    TrackButton = tbLeftButton
    Left = 200
    Top = 232
    object BuildingsMenu: TMenuItem
      Caption = 'BuildingsMenu'
    end
    object UnitsMenu: TMenuItem
      AutoLineReduction = maAutomatic
      Caption = 'UnitsMenu'
    end
  end
end
