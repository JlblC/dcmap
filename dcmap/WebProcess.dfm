object WebProcessForm: TWebProcessForm
  Left = 180
  Top = 263
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'DCMap'
  ClientHeight = 87
  ClientWidth = 336
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMode = pmExplicit
  PopupParent = MainForm.Owner
  Position = poDesigned
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Gauge: TsGauge
    Left = 8
    Top = 27
    Width = 320
    Height = 25
    Visible = False
    SkinData.SkinSection = 'GAUGE'
    ForeColor = clBlack
    Suffix = '%'
  end
  object StatusLabel: TsLabel
    Left = 298
    Top = 58
    Width = 30
    Height = 13
    Alignment = taRightJustify
    Caption = '          '
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8542779
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object OperationLabel: TsLabel
    Left = 8
    Top = 8
    Width = 27
    Height = 13
    Caption = '         '
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8542779
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object CanselBtn: TsButton
    Left = 8
    Top = 58
    Width = 81
    Height = 25
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 0
    SkinData.SkinSection = 'BUTTON'
  end
  object IdHTTP: TIdHTTP
    OnStatus = IdHTTPStatus
    OnWork = IdHTTPWork
    OnWorkBegin = IdHTTPWorkBegin
    OnWorkEnd = IdHTTPWorkEnd
    AllowCookies = True
    HandleRedirects = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.Accept = 'text/html, */*'
    Request.BasicAuthentication = False
    Request.UserAgent = 'DCMap'
    HTTPOptions = []
    CookieManager = IdCookieManager
    Compressor = IdCompressorZLib1
    Left = 168
    Top = 8
  end
  object IdCookieManager: TIdCookieManager
    Left = 248
    Top = 8
  end
  object sSkinProvider: TsSkinProvider
    CaptionAlignment = taCenter
    SkinData.SkinSection = 'FORM'
    TitleButtons = <>
    Left = 288
    Top = 8
  end
  object IdCompressorZLib1: TIdCompressorZLib
    Left = 208
    Top = 8
  end
  object IdAntiFreeze1: TIdAntiFreeze
    IdleTimeOut = 50
    OnlyWhenIdle = False
    Left = 128
    Top = 8
  end
end
