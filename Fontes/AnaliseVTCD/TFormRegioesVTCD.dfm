object FormRegioesVTCD: TFormRegioesVTCD
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'FormRegioesVTCD'
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
  object Splitter1: TSplitter
    Left = 431
    Top = 28
    Height = 197
    Align = alRight
    ExplicitLeft = 216
    ExplicitTop = 64
    ExplicitHeight = 100
  end
  object Splitter2: TSplitter
    Left = 206
    Top = 28
    Height = 197
    ExplicitLeft = 216
    ExplicitTop = 64
    ExplicitHeight = 100
  end
  object ToolBar2: TToolBar
    Left = 0
    Top = 0
    Width = 434
    Height = 28
    ButtonHeight = 19
    ButtonWidth = 103
    EdgeBorders = [ebTop, ebBottom]
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = True
    TabOrder = 0
    Wrapable = False
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Action = ActionMostraBarrasCurto
    end
    object ToolButton2: TToolButton
      Left = 103
      Top = 0
      Action = ActionMostraEqProt
    end
  end
  object gbBarrasCurto: TGroupBox
    Left = 209
    Top = 28
    Width = 222
    Height = 197
    Align = alClient
    TabOrder = 1
  end
  object lvRegioes: TListView
    Left = 0
    Top = 28
    Width = 206
    Height = 197
    Align = alLeft
    Columns = <
      item
        Caption = 'Alimentador'
        Width = 100
      end
      item
        Caption = 'ChaveRegi'#227'o'
        Width = 100
      end>
    TabOrder = 2
    ViewStyle = vsReport
    OnClick = ActionMostraRegiaoExecute
    OnColumnClick = lvRegioesColumnClick
    OnCompare = lvRegioesCompare
    OnDblClick = ActionMostraBarrasCurtoExecute
  end
  object ActionList1: TActionList
    Left = 16
    Top = 136
    object ActionMostraRegiao: TAction
      Caption = 'ActionMostraRegiao'
      OnExecute = ActionMostraRegiaoExecute
    end
    object ActionMostraEqProt: TAction
      Caption = 'MostraEqProt'
      OnExecute = ActionMostraEqProtExecute
    end
    object ActionMostraBarrasCurto: TAction
      Caption = 'MostraBarrasCurto'
      OnExecute = ActionMostraBarrasCurtoExecute
    end
  end
end
