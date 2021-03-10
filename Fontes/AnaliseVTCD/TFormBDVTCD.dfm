object FormBDVTCD: TFormBDVTCD
  Left = 0
  Top = 0
  Caption = 'FormBDVTCD'
  ClientHeight = 315
  ClientWidth = 546
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 153
    Top = 0
    Height = 315
    ExplicitLeft = 88
    ExplicitHeight = 100
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 153
    Height = 315
    Align = alLeft
    TabOrder = 0
    object Button1: TButton
      Left = 16
      Top = 16
      Width = 129
      Height = 25
      Action = ActionDefLisRegioes
      TabOrder = 0
    end
    object Button5: TButton
      Left = 16
      Top = 47
      Width = 129
      Height = 25
      Action = ActionDefUCSV
      TabOrder = 1
    end
    object Button2: TButton
      Left = 16
      Top = 79
      Width = 129
      Height = 25
      Action = ActionDefAmostra
      TabOrder = 2
    end
    object Button3: TButton
      Left = 16
      Top = 110
      Width = 129
      Height = 25
      Action = ActionGeraCurtos
      TabOrder = 3
    end
    object Button4: TButton
      Left = 16
      Top = 141
      Width = 129
      Height = 25
      Action = ActionGravaBD
      TabOrder = 4
    end
  end
  object GroupBox2: TGroupBox
    Left = 156
    Top = 0
    Width = 390
    Height = 315
    Align = alClient
    TabOrder = 1
    object PageControl1: TPageControl
      Left = 2
      Top = 15
      Width = 386
      Height = 298
      ActivePage = TabSheet1
      Align = alClient
      TabOrder = 0
      object TabSheet2: TTabSheet
        Caption = 'Resultados'
        ImageIndex = 1
        object gbResultados: TGroupBox
          Left = 0
          Top = 0
          Width = 378
          Height = 270
          Align = alClient
          TabOrder = 0
        end
      end
      object TabSheet1: TTabSheet
        Caption = 'Dados gerais'
        object GroupBox3: TGroupBox
          Left = 0
          Top = 128
          Width = 378
          Height = 91
          Align = alTop
          Caption = 'Resist'#234'ncia de falta/%'
          TabOrder = 0
          object sgridTaxasImpedancia: TStringGrid
            Left = 2
            Top = 15
            Width = 374
            Height = 74
            Align = alClient
            ColCount = 2
            DefaultColWidth = 125
            DefaultRowHeight = 18
            FixedColor = clMenuBar
            FixedCols = 0
            RowCount = 2
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goAlwaysShowEditor]
            TabOrder = 0
            ColWidths = (
              125
              100)
            RowHeights = (
              18
              18)
          end
        end
        object GroupBox4: TGroupBox
          Left = 0
          Top = 0
          Width = 378
          Height = 128
          Align = alTop
          Caption = 'Tipo de defeito/%'
          TabOrder = 1
          object sgridTaxasDefeito: TStringGrid
            Left = 2
            Top = 15
            Width = 374
            Height = 111
            Align = alClient
            ColCount = 2
            DefaultColWidth = 125
            DefaultRowHeight = 18
            FixedColor = clMenuBar
            RowCount = 2
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goAlwaysShowEditor]
            TabOrder = 0
            ColWidths = (
              125
              100)
            RowHeights = (
              18
              18)
          end
        end
        object GroupBox5: TGroupBox
          Left = 0
          Top = 219
          Width = 378
          Height = 51
          Align = alClient
          Caption = 'Taxa de amostragem '
          TabOrder = 2
          object sgridTaxaAmostra: TStringGrid
            Left = 2
            Top = 15
            Width = 374
            Height = 34
            Align = alClient
            ColCount = 2
            DefaultColWidth = 125
            DefaultRowHeight = 18
            FixedColor = clMenuBar
            RowCount = 2
            FixedRows = 0
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goAlwaysShowEditor]
            TabOrder = 0
            ColWidths = (
              125
              100)
            RowHeights = (
              18
              18)
          end
        end
      end
    end
  end
  object ActionList1: TActionList
    Left = 16
    Top = 224
    object ActionDefLisRegioes: TAction
      Caption = 'Def.Regionais'
      OnExecute = ActionDefLisRegioesExecute
    end
    object ActionDefAmostra: TAction
      Caption = 'Amostra curtos'
      OnExecute = ActionDefAmostraExecute
    end
    object ActionGeraCurtos: TAction
      Caption = 'Gera Curtos'
      OnExecute = ActionGeraCurtosExecute
    end
    object ActionGravaBD: TAction
      Caption = 'Grava BD'
      OnExecute = ActionGravaBDExecute
    end
    object ActionDefUCSV: TAction
      Caption = 'Def.UCSV'
      OnExecute = ActionDefUCSVExecute
    end
  end
end
