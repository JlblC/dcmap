inherited ProgressForm: TProgressForm
  BorderIcons = [biSystemMenu]
  Caption = 'DCMap'
  ClientHeight = 190
  ClientWidth = 469
  FormStyle = fsStayOnTop
  ExplicitWidth = 477
  ExplicitHeight = 222
  DesignSize = (
    469
    190)
  PixelsPerInch = 96
  TextHeight = 13
  object ProgressBar: TsGauge [0]
    Left = 8
    Top = 154
    Width = 367
    Height = 28
    Anchors = [akLeft, akRight, akBottom]
    Visible = False
    SkinData.SkinSection = 'GAUGE'
    ForeColor = clBlack
    Suffix = '%'
    ExplicitTop = 177
    ExplicitWidth = 345
  end
  object StatusLabel: TsLabel [1]
    Left = 8
    Top = 154
    Width = 276
    Height = 32
    Anchors = [akLeft, akRight, akBottom]
    AutoSize = False
    Caption = 'StatusLabel'
    ParentFont = False
    WordWrap = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ExplicitTop = 188
    ExplicitWidth = 302
  end
  object MessagesMemo: TsMemo [2]
    Left = 8
    Top = 8
    Width = 453
    Height = 141
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
    WantReturns = False
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
  object CancelBtn: TsButton [3]
    Left = 387
    Top = 154
    Width = 74
    Height = 28
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cansel'
    TabOrder = 1
    OnClick = CancelBtnClick
    SkinData.SkinSection = 'BUTTON'
  end
  object ContinueBtn: TsButton [4]
    Left = 290
    Top = 154
    Width = 83
    Height = 27
    Anchors = [akRight, akBottom]
    Caption = 'Continue'
    Default = True
    TabOrder = 2
    Visible = False
    OnClick = ContinueBtnClick
    SkinData.SkinSection = 'BUTTON'
  end
  object RetryBtn: TsButton [5]
    Left = 193
    Top = 154
    Width = 83
    Height = 27
    Anchors = [akRight, akBottom]
    Caption = 'Retry'
    TabOrder = 3
    Visible = False
    OnClick = RetryBtnClick
    SkinData.SkinSection = 'BUTTON'
  end
end
