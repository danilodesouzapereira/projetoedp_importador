object FormRegiaoCurtos: TFormRegiaoCurtos
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'FormRegiaoCurtos'
  ClientHeight = 225
  ClientWidth = 434
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object lvRegiaoCurtos: TListView
    Left = 0
    Top = 0
    Width = 434
    Height = 225
    Align = alClient
    Columns = <
      item
        Caption = 'C'#243'digo'
        Width = 120
      end
      item
        Caption = 'Falhas (f/ano)'
        Width = 120
      end>
    MultiSelect = True
    TabOrder = 0
    ViewStyle = vsReport
    OnClick = ActionSelBarrasExecute
  end
  object ActionList1: TActionList
    Left = 216
    Top = 128
    object ActionSelBarras: TAction
      Caption = 'ActionSelBarras'
      OnExecute = ActionSelBarrasExecute
    end
  end
end
