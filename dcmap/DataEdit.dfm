inherited DataEditForm: TDataEditForm
  Left = 329
  Top = 179
  Anchors = [akLeft, akTop, akRight, akBottom]
  BorderStyle = bsDialog
  Caption = 'DataEditForm'
  ClientHeight = 166
  ClientWidth = 344
  Constraints.MinWidth = 350
  Font.Charset = RUSSIAN_CHARSET
  ExplicitWidth = 350
  ExplicitHeight = 196
  DesignSize = (
    344
    166)
  PixelsPerInch = 96
  TextHeight = 13
  inherited DataCopyButton: TsSpeedButton
    Left = 37
    Top = 133
    Width = 40
    Height = 25
    Anchors = [akLeft, akBottom]
    ExplicitLeft = 37
    ExplicitTop = 175
    ExplicitWidth = 40
    ExplicitHeight = 25
  end
  inherited ContextButton: TsSpeedButton
    Left = 81
    Top = 133
    Width = 40
    Height = 25
    Anchors = [akLeft, akBottom]
    ExplicitLeft = 81
    ExplicitTop = 175
    ExplicitWidth = 40
    ExplicitHeight = 25
  end
  object CloseButton: TsButton [2]
    Left = 271
    Top = 133
    Width = 67
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 1
    SkinData.SkinSection = 'BUTTON'
  end
  object OkButton: TsButton [3]
    Left = 205
    Top = 133
    Width = 59
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = #1054#1082
    Default = True
    ModalResult = 1
    TabOrder = 0
    SkinData.SkinSection = 'BUTTON'
  end
  inherited DataPasteButton: TsBitBtn
    Left = 4
    Top = 133
    Width = 30
    Height = 25
    Anchors = [akLeft, akBottom]
    TabOrder = 2
    ExplicitLeft = 4
    ExplicitTop = 133
    ExplicitWidth = 30
    ExplicitHeight = 25
  end
  object ApplyButton: TsBitBtn [5]
    Left = 128
    Top = 133
    Width = 68
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 4
    OnClick = ApplyButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  inherited PropPageFrame: TPropPageFrame
    Left = 4
    Top = 5
    Width = 334
    Height = 114
    TabOrder = 3
    ExplicitLeft = 4
    ExplicitTop = 5
    ExplicitWidth = 334
    ExplicitHeight = 114
    inherited Tabs: TsTabControl
      Width = 334
      Height = 114
      ExplicitWidth = 334
      ExplicitHeight = 114
      inherited ClientPanel: TsPanel
        Width = 326
        Height = 101
        ExplicitWidth = 326
        ExplicitHeight = 101
      end
    end
    inherited sFrameAdapter: TsFrameAdapter
      Top = 16
    end
  end
  inherited SkinProvider: TsSkinProvider
    Left = 112
    Top = 32
  end
end
