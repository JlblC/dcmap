inherited FilterSelectForm: TFilterSelectForm
  BorderStyle = bsDialog
  Caption = 'FilterSelectForm'
  ClientHeight = 196
  ClientWidth = 250
  Position = poOwnerFormCenter
  ExplicitWidth = 256
  ExplicitHeight = 226
  PixelsPerInch = 96
  TextHeight = 13
  object ItemsView: TsListView [0]
    Left = 8
    Top = 11
    Width = 153
    Height = 177
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
    Anchors = [akLeft, akTop, akRight, akBottom]
    Columns = <
      item
        AutoSize = True
      end>
    RowSelect = True
    ShowColumnHeaders = False
    SortType = stText
    TabOrder = 0
    ViewStyle = vsReport
    OnDblClick = ItemsViewDblClick
    ExplicitWidth = 177
  end
  object OkButton: TsBitBtn [1]
    Left = 167
    Top = 11
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = 'Ok'
    Default = True
    DoubleBuffered = True
    ModalResult = 1
    ParentDoubleBuffered = False
    TabOrder = 1
    SkinData.SkinSection = 'BUTTON'
    ExplicitLeft = 191
  end
  object CancelButton: TsBitBtn [2]
    Left = 167
    Top = 42
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Cancel'
    DoubleBuffered = True
    ModalResult = 2
    ParentDoubleBuffered = False
    TabOrder = 2
    SkinData.SkinSection = 'BUTTON'
    ExplicitLeft = 191
  end
  object DeleteButton: TsBitBtn [3]
    Left = 167
    Top = 163
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Delete'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 3
    OnClick = DeleteButtonClick
    SkinData.SkinSection = 'BUTTON'
    ExplicitLeft = 191
  end
  inherited SkinProvider: TsSkinProvider
    Left = 192
    Top = 80
  end
end
