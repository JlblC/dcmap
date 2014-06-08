inherited BaseEditForm: TBaseEditForm
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'BaseEditForm'
  ClientHeight = 423
  ClientWidth = 622
  Position = poOwnerFormCenter
  ExplicitWidth = 628
  ExplicitHeight = 453
  DesignSize = (
    622
    423)
  PixelsPerInch = 96
  TextHeight = 13
  object DataCopyButton: TsSpeedButton [0]
    Left = 387
    Top = 8
    Width = 49
    Height = 27
    OnClick = DataCopyButtonClick
    ButtonStyle = tbsDropDown
    SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    DropdownMenu = CopyMenu
  end
  object ContextButton: TsSpeedButton [1]
    Left = 464
    Top = 8
    Width = 41
    Height = 27
    Visible = False
    OnClick = ContextButtonClick
    ButtonStyle = tbsDropDown
    SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    DropdownMenu = ContextMenu
    Images = InterfaceModule.MenuImageList
    ShowCaption = False
  end
  object DataPasteButton: TsBitBtn [2]
    Left = 340
    Top = 8
    Width = 33
    Height = 27
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 0
    OnClick = DataPasteButtonClick
    SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
  end
  inline PropPageFrame: TPropPageFrame [3]
    Left = 162
    Top = 48
    Width = 349
    Height = 190
    Anchors = [akLeft, akTop, akRight, akBottom]
    ParentBackground = False
    TabOrder = 1
    TabStop = True
    ExplicitLeft = 162
    ExplicitTop = 48
    ExplicitWidth = 349
    ExplicitHeight = 190
    inherited Tabs: TsTabControl
      Width = 349
      Height = 190
      ExplicitWidth = 349
      ExplicitHeight = 190
      inherited ClientPanel: TsPanel
        Width = 341
        Height = 177
        ExplicitWidth = 341
        ExplicitHeight = 177
      end
    end
  end
  object CopyMenu: TPopupMenu
    Left = 208
    Top = 8
  end
  object ContextMenu: TPopupMenu
    OnPopup = ContextMenuPopup
    Left = 544
    Top = 16
  end
end
