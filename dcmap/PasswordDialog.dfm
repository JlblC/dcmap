object PasswordDialogForm: TPasswordDialogForm
  Left = 0
  Top = 0
  ActiveControl = PasswordEdit
  BorderStyle = bsDialog
  Caption = 'DCMap'
  ClientHeight = 98
  ClientWidth = 253
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label: TsLabel
    Left = 24
    Top = 8
    Width = 25
    Height = 13
    Caption = 'Label'
  end
  object OkBtn: TsButton
    Left = 34
    Top = 60
    Width = 75
    Height = 25
    Caption = 'Ok'
    Default = True
    ModalResult = 1
    TabOrder = 1
    SkinData.SkinSection = 'BUTTON'
  end
  object CancelBtn: TsButton
    Left = 142
    Top = 60
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
    SkinData.SkinSection = 'BUTTON'
  end
  object PasswordEdit: TsEdit
    Left = 24
    Top = 29
    Width = 201
    Height = 21
    PasswordChar = '*'
    TabOrder = 0
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object sSkinProvider1: TsSkinProvider
    CaptionAlignment = taCenter
    SkinData.SkinSection = 'DIALOG'
    ShowAppIcon = False
    TitleButtons = <>
    Left = 128
  end
end
