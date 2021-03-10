object FormResBar: TFormResBar
  Left = 0
  Top = 0
  Caption = 'FormResBar'
  ClientHeight = 251
  ClientWidth = 335
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object LViewResBar: TListView
    Left = 0
    Top = 0
    Width = 335
    Height = 113
    Align = alTop
    Columns = <
      item
        Caption = 'Tipo'
      end
      item
        Caption = 'SAIFI90'
        Width = 70
      end
      item
        Caption = 'SAIFI70'
        Width = 70
      end
      item
        Caption = 'SAIFI40'
        Width = 70
      end
      item
        Caption = 'Interrup'#231#245'es'
        Width = 70
      end>
    TabOrder = 0
    ViewStyle = vsReport
    OnSelectItem = LViewResBarSelectItem
  end
  object chtAfunda: TChart
    Left = 0
    Top = 113
    Width = 335
    Height = 138
    Legend.Visible = False
    MarginBottom = 0
    MarginLeft = 5
    MarginRight = 5
    MarginTop = 20
    Title.CustomPosition = True
    Title.Left = 130
    Title.Text.Strings = (
      'Eventos/ano')
    Title.Top = 5
    View3D = False
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitLeft = 16
    ExplicitTop = 121
    ExplicitWidth = 160
    ExplicitHeight = 122
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series1: TBarSeries
      Marks.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
  end
end
