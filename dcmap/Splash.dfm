inherited SplashForm: TSplashForm
  AlphaBlendValue = 200
  BorderStyle = bsNone
  Caption = 'SplashForm'
  ClientHeight = 136
  ClientWidth = 326
  Position = poDesktopCenter
  Visible = True
  ExplicitWidth = 326
  ExplicitHeight = 136
  PixelsPerInch = 96
  TextHeight = 13
  object Panel: TsPanel [0]
    Left = 0
    Top = 0
    Width = 326
    Height = 136
    Align = alClient
    BevelOuter = bvNone
    Padding.Left = 4
    Padding.Top = 4
    Padding.Right = 4
    Padding.Bottom = 4
    TabOrder = 0
    SkinData.SkinSection = 'PANEL'
    object sLabelFX1: TsLabelFX
      Left = 11
      Top = 3
      Width = 195
      Height = 67
      AutoSize = False
      Caption = 'DCMap'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -48
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object RevisionLabel: TsLabelFX
      Left = 125
      Top = 8
      Width = 62
      Height = 23
      Caption = 'revision 4'
      ParentFont = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Times New Roman'
      Font.Style = [fsItalic]
    end
    object StrID: TsLabelFX
      Left = 166
      Top = 52
      Width = 36
      Height = 24
      Caption = '1.26'
      ParentFont = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object StatusLabel: TsLabelFX
      Left = 4
      Top = 91
      Width = 318
      Height = 41
      Align = alBottom
      Alignment = taCenter
      AutoSize = False
      Caption = 'Loading...'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Fixedsys'
      Font.Style = []
      ExplicitLeft = 8
      ExplicitTop = 87
      ExplicitWidth = 225
    end
    object sLabelFX2: TsLabelFX
      Left = 13
      Top = 2
      Width = 195
      Height = 67
      AutoSize = False
      Caption = '____'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -53
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
  end
  inherited SkinProvider: TsSkinProvider
    Left = 32
    Top = 80
  end
end
