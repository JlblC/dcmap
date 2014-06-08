inherited FleetSideFrame: TFleetSideFrame
  Width = 303
  Height = 421
  Constraints.MinHeight = 300
  Constraints.MinWidth = 170
  ExplicitWidth = 303
  ExplicitHeight = 421
  object sSplitter1: TsSplitter [0]
    Left = 0
    Top = 214
    Width = 303
    Height = 4
    Cursor = crVSplit
    Align = alBottom
    AutoSnap = False
    MinSize = 20
    ResizeStyle = rsUpdate
    SkinData.SkinSection = 'SPLITTER'
    ExplicitTop = 143
    ExplicitWidth = 200
  end
  object DragBar: TsDragBar [1]
    Left = 0
    Top = 416
    Width = 303
    Height = 5
    Cursor = crSizeNS
    Align = alBottom
    BevelOuter = bvNone
    Caption = ' '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clCaptionText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    Visible = False
    OnMouseDown = DragBarMouseDown
    OnMouseMove = DragBarMouseMove
    SkinData.SkinSection = 'SPLITTER'
    DraggedControl = WinControlForm_1.Owner
  end
  object BottomPanel: TsPanel [2]
    Left = 0
    Top = 315
    Width = 303
    Height = 101
    Align = alBottom
    TabOrder = 1
    SkinData.SkinSection = 'CHECKBOX'
    DesignSize = (
      303
      101)
    object sLabel1: TsLabel
      Left = 40
      Top = -1
      Width = 30
      Height = 13
      Alignment = taRightJustify
      Caption = #1060#1083#1086#1090':'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object FleetNameLabel: TsLabel
      Left = 73
      Top = -1
      Width = 76
      Height = 13
      Caption = 'FleetNameLabel'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object sLabel3: TsLabel
      Left = 17
      Top = 12
      Width = 53
      Height = 13
      Alignment = taRightJustify
      Caption = #1042#1083#1072#1076#1077#1083#1077#1094':'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object FleetOwnerLabel: TsLabel
      Left = 73
      Top = 12
      Width = 81
      Height = 13
      Caption = 'FleetOwnerLabel'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object sLabel5: TsLabel
      Left = 18
      Top = 29
      Width = 52
      Height = 13
      Alignment = taRightJustify
      Caption = #1057#1082#1086#1088#1086#1089#1090#1100':'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object sLabel6: TsLabel
      Left = 11
      Top = 43
      Width = 59
      Height = 13
      Alignment = taRightJustify
      Caption = #1044#1072#1083#1100#1085#1086#1089#1090#1100':'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object FleetSpeedLabel: TsLabel
      Left = 73
      Top = 29
      Width = 79
      Height = 13
      Caption = 'FleetSpeedLabel'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object FleetRangeLabel: TsLabel
      Left = 73
      Top = 43
      Width = 80
      Height = 13
      Caption = 'FleetRangeLabel'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object FleetInfoLabel: TsLabel
      Left = 11
      Top = 60
      Width = 69
      Height = 13
      Caption = 'FleetInfoLabel'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object UnitNameLabel: TsLabel
      Left = 11
      Top = 84
      Width = 3
      Height = 13
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
    end
    object sBevel1: TsBevel
      Left = 5
      Top = 73
      Width = 301
      Height = 7
      Anchors = [akLeft, akTop, akRight]
      Shape = bsBottomLine
    end
    object SendFleetBtn: TsButton
      Left = 237
      Top = 27
      Width = 61
      Height = 29
      Anchors = [akTop, akRight]
      Caption = #1051#1077#1090#1077#1090#1100' '#1082' '#1087#1083#1072#1085#1077#1090#1077
      TabOrder = 0
      OnClick = SendFleetBtnClick
      OnMouseLeave = SendFleetBtnMouseLeave
      OnStartDrag = FleetViewStartDrag
      SkinData.SkinSection = 'BUTTON'
    end
    object CancelJumpBtn: TsButton
      Left = 236
      Top = 27
      Width = 61
      Height = 30
      Anchors = [akTop, akRight]
      Caption = #1054#1090#1084#1077#1085#1080#1090#1100' '#1087#1086#1083#1105#1090
      TabOrder = 1
      Visible = False
      OnClick = CancelJumpBtnClick
      SkinData.SkinSection = 'BUTTON'
    end
  end
  object FleetView: TsTreeView [3]
    Left = 0
    Top = 0
    Width = 303
    Height = 214
    Align = alClient
    DragMode = dmAutomatic
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    HotTrack = True
    Images = InterfaceModule.FleetIcons
    Indent = 19
    MultiSelect = True
    MultiSelectStyle = [msControlSelect, msShiftSelect]
    ParentFont = False
    ParentShowHint = False
    PopupMenu = FleetPopup
    ReadOnly = True
    RightClickSelect = True
    ShowHint = True
    StateImages = InterfaceModule.FleetStateIcons
    TabOrder = 2
    OnAdvancedCustomDrawItem = FleetViewAdvancedCustomDrawItem
    OnChange = FleetViewChange
    OnChanging = FleetViewChanging
    OnClick = FleetViewClick
    OnCollapsing = FleetViewCollapsing
    OnContextPopup = FleetViewContextPopup
    OnDblClick = FleetViewDblClick
    OnDeletion = FleetViewDeletion
    OnDragDrop = FleetViewDragDrop
    OnDragOver = FleetViewDragOver
    OnEdited = FleetViewEdited
    OnEditing = FleetViewEditing
    OnExpanding = FleetViewExpanding
    OnKeyDown = FleetViewKeyDown
    OnMouseMove = FleetViewMouseMove
    OnStartDrag = FleetViewStartDrag
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
  object PlanetFleetView: TsTreeView [4]
    Left = 0
    Top = 218
    Width = 303
    Height = 97
    Align = alBottom
    DragMode = dmAutomatic
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    HotTrack = True
    Images = InterfaceModule.FleetIcons
    Indent = 19
    MultiSelect = True
    MultiSelectStyle = [msControlSelect, msShiftSelect]
    ParentFont = False
    ParentShowHint = False
    PopupMenu = FleetPopup
    ReadOnly = True
    RightClickSelect = True
    ShowHint = True
    ShowRoot = False
    StateImages = InterfaceModule.FleetStateIcons
    TabOrder = 3
    OnAdvancedCustomDrawItem = FleetViewAdvancedCustomDrawItem
    OnChange = FleetViewChange
    OnChanging = FleetViewChanging
    OnClick = FleetViewClick
    OnCollapsing = FleetViewCollapsing
    OnContextPopup = FleetViewContextPopup
    OnDblClick = FleetViewDblClick
    OnDeletion = FleetViewDeletion
    OnDragDrop = FleetViewDragDrop
    OnDragOver = FleetViewDragOver
    OnEdited = FleetViewEdited
    OnEditing = FleetViewEditing
    OnExpanding = FleetViewExpanding
    OnKeyDown = FleetViewKeyDown
    OnMouseMove = FleetViewMouseMove
    OnStartDrag = FleetViewStartDrag
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
    ExplicitTop = 220
  end
  inherited sFrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'BARPANEL'
    Left = 26
    Top = 9
  end
  object FleetPopup: TPopupMenu
    Left = 88
    Top = 64
    object HideFleetMenu: TMenuItem
      Tag = 1
      Caption = 'HideFleet'
      Visible = False
      OnClick = HideFleetMenuClick
    end
    object CancelHideFleetMenu: TMenuItem
      Tag = 2
      Caption = 'CancelHideFleet'
      Visible = False
      OnClick = HideFleetMenuClick
    end
    object UnhideFleetMenu: TMenuItem
      Tag = 3
      Caption = 'UnhideFleet'
      Visible = False
      OnClick = HideFleetMenuClick
    end
    object CancelUnhideFleetMenu: TMenuItem
      Tag = 4
      Caption = 'CancelUnhideFleet'
      Visible = False
      OnClick = HideFleetMenuClick
    end
    object UnpackMenu: TMenuItem
      Caption = 'Unpack'
      OnClick = UnpackMenuClick
    end
    object CancelCargoLoadMenu: TMenuItem
      Caption = 'Cancel Cargo Load'
      OnClick = CancelCargoLoadMenuClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object FlyToMenu: TMenuItem
      Caption = 'FlyTo'
      OnClick = FlyToMenuClick
    end
    object DisbandMenu: TMenuItem
      Caption = 'Disband'
      OnClick = DisbandMenuClick
    end
    object CreateFleetFromSelMenu: TMenuItem
      Caption = 'CreateFleetFromSel'
      OnClick = CreateFleetFromSelMenuClick
    end
    object CancelJumpMenu: TMenuItem
      Caption = 'CancelJump'
      OnClick = CancelJumpMenuClick
    end
    object MoveToFleetMenu: TMenuItem
      Caption = 'MoveToFleet'
      SubMenuImages = InterfaceModule.FleetIcons
      object MoveToNewFleetMenu: TMenuItem
        Caption = 'CreateNew'
        ImageIndex = 10
        OnClick = CreateFleetFromSelMenuClick
      end
    end
    object SetIconMenu: TMenuItem
      Caption = 'SetIcon'
      SubMenuImages = InterfaceModule.IconsImageList
      OnClick = SetIconMenuClick
      object SetEmptyIconMenu: TMenuItem
        Caption = 'None'
        OnClick = OnSetIcon
      end
    end
    object RenameMenu: TMenuItem
      Caption = 'Rename'
      OnClick = RenameMenuClick
    end
    object PropertiesMenu: TMenuItem
      Caption = 'Properties'
      OnClick = PropertiesMenuClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
  end
  object QueuePopup: TPopupMenu
    TrackButton = tbLeftButton
    Left = 24
    Top = 64
    object BuildingsMenu: TMenuItem
      Caption = 'BuildingsMenu'
      SubMenuImages = InterfaceModule.FleetIcons
    end
    object UnitsMenu: TMenuItem
      AutoLineReduction = maAutomatic
      Caption = 'UnitsMenu'
      SubMenuImages = InterfaceModule.FleetIcons
    end
  end
end
