object DialogForm: TDialogForm
  Left = 0
  Top = 0
  Caption = 'DialogForm'
  ClientHeight = 327
  ClientWidth = 482
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnDestroy = TntFormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object sSkinProvider1: TsSkinProvider
    SkinData.SkinSection = 'FORM'
    TitleButtons = <>
    Left = 16
    Top = 16
  end
end
