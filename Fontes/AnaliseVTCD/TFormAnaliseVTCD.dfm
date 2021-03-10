inherited FormAnaliseVTCD: TFormAnaliseVTCD
  Caption = 'An'#225'lise de VTCDs'
  ClientHeight = 565
  ClientWidth = 849
  Position = poDesigned
  OnClose = FormClose
  ExplicitWidth = 865
  ExplicitHeight = 604
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter6: TSplitter [0]
    Left = 210
    Top = 25
    Height = 540
    ExplicitLeft = 248
    ExplicitTop = 344
    ExplicitHeight = 100
  end
  inherited PanelToolBar: TPanel
    Width = 849
    ExplicitWidth = 849
    inherited PanelLeft: TPanel
      Width = 641
      ExplicitWidth = 641
      inherited ToolBarLeft: TToolBar
        Width = 641
        ExplicitWidth = 641
        inherited butAlign: TToolButton
          ExplicitWidth = 48
        end
        inherited butEdt: TToolButton
          ExplicitWidth = 27
        end
        object ToolButton6: TToolButton
          Left = 97
          Top = 0
          Action = ActionExecutar
        end
        object ToolButton8: TToolButton
          Left = 120
          Top = 0
          Width = 23
          Caption = 'ToolButton8'
          ImageIndex = 8
          Style = tbsSeparator
        end
        object ChkMapa: TCheckBox
          Left = 143
          Top = 0
          Width = 97
          Height = 22
          Hint = 'Habilitar visualiza'#231#227'o do mapa tem'#225'tico de afundamentos'
          Caption = 'Mapa Tem'#225'tico'
          Checked = True
          Enabled = False
          State = cbChecked
          TabOrder = 2
          OnClick = chkMapaClick
        end
        object chkDif: TCheckBox
          Left = 240
          Top = 0
          Width = 97
          Height = 22
          Hint = 'Habilitar visualizar diferen'#231'as em rela'#231#227'o a rede b'#225'sica'
          Caption = 'Diferen'#231'as'
          TabOrder = 4
          Visible = False
        end
        object CBoxAf: TComboBox
          Left = 337
          Top = 0
          Width = 92
          Height = 21
          Hint = 'Selecionar n'#237'vel de afundamento'
          Style = csDropDownList
          Enabled = False
          TabOrder = 3
          OnChange = CBoxAfChange
          Items.Strings = (
            'SARFI 40'
            'SARFI 70'
            'SARFI 90'
            'Interrup'#231#245'es')
        end
        object CBoxDef: TComboBox
          Left = 429
          Top = 0
          Width = 45
          Height = 21
          Hint = 'Selecionar defeito p/ anima'#231#227'o gr'#225'fica'
          Style = csDropDownList
          TabOrder = 1
          Visible = False
        end
        object pnMin: TPanel
          Left = 474
          Top = 0
          Width = 64
          Height = 22
          ParentBackground = False
          TabOrder = 5
          Visible = False
        end
        object lblMin: TLabel
          Left = 538
          Top = 0
          Width = 3
          Height = 22
          Layout = tlCenter
          Visible = False
        end
        object lblMax: TLabel
          Left = 541
          Top = 0
          Width = 3
          Height = 22
          Layout = tlCenter
          Visible = False
        end
        object pnMax: TPanel
          Left = 544
          Top = 0
          Width = 65
          Height = 22
          ParentBackground = False
          TabOrder = 6
          Visible = False
        end
      end
    end
    inherited PanelRight: TPanel
      Left = 641
      Width = 208
      ExplicitLeft = 641
      ExplicitWidth = 208
    end
  end
  object pgcResult: TPageControl [2]
    Left = 213
    Top = 25
    Width = 636
    Height = 540
    ActivePage = tbsDadosEntrada
    Align = alClient
    TabOrder = 1
    object tbsDadosEntrada: TTabSheet
      Caption = 'Dados de entrada'
      object Splitter5: TSplitter
        Left = 0
        Top = 271
        Width = 628
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ExplicitTop = 137
        ExplicitWidth = 219
      end
      object Splitter7: TSplitter
        Left = 0
        Top = 274
        Width = 628
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ExplicitLeft = 3
        ExplicitTop = 214
        ExplicitWidth = 350
      end
      object Splitter9: TSplitter
        Left = 0
        Top = 329
        Width = 628
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ExplicitWidth = 100
      end
      object GroupBox10: TGroupBox
        Left = 0
        Top = 97
        Width = 628
        Height = 174
        Align = alTop
        Caption = 'Sele'#231#227'o de tipos de defeito'
        TabOrder = 0
        object sgridTaxasDefeito: TStringGrid
          Left = 2
          Top = 44
          Width = 624
          Height = 128
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
        object ToolBar1: TToolBar
          Left = 2
          Top = 15
          Width = 624
          Height = 29
          ButtonHeight = 21
          ButtonWidth = 79
          Caption = 'ToolBar1'
          EdgeBorders = [ebTop, ebBottom]
          ShowCaptions = True
          TabOrder = 1
          object ToolButton9: TToolButton
            Left = 0
            Top = 0
            Action = ActionDefaultTipoCurto
          end
        end
      end
      object sgridTaxasRede: TStringGrid
        Left = 0
        Top = 0
        Width = 628
        Height = 97
        Align = alTop
        ColCount = 2
        DefaultColWidth = 50
        DefaultRowHeight = 18
        FixedColor = clMenuBar
        RowCount = 2
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goAlwaysShowEditor]
        TabOrder = 1
        ColWidths = (
          126
          99)
        RowHeights = (
          18
          18)
      end
      object GroupBox12: TGroupBox
        Left = 0
        Top = 448
        Width = 484
        Height = 61
        Caption = 'Sele'#231#227'o de patamares'
        TabOrder = 2
        Visible = False
        object ToolBarPatamar: TToolBar
          Left = 2
          Top = 15
          Width = 480
          Height = 26
          ButtonHeight = 20
          ButtonWidth = 24
          Caption = 'ToolBar1'
          EdgeBorders = [ebTop, ebBottom]
          Font.Charset = SYMBOL_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Wingdings'
          Font.Style = [fsBold]
          List = True
          ParentFont = False
          ShowCaptions = True
          TabOrder = 0
          Wrapable = False
          object ToolButton4: TToolButton
            Left = 0
            Top = 0
            Action = ActionEnablePatamar
          end
          object ToolButton7: TToolButton
            Left = 24
            Top = 0
            Action = ActionDisablePatamar
          end
        end
        object CLBoxPatamar: TCheckListBox
          Left = 2
          Top = 41
          Width = 480
          Height = 18
          Align = alClient
          BorderStyle = bsNone
          Columns = 2
          ItemHeight = 13
          TabOrder = 1
        end
      end
      object Panel4: TPanel
        Left = 0
        Top = 277
        Width = 628
        Height = 52
        Align = alTop
        TabOrder = 3
        object Splitter8: TSplitter
          Left = 439
          Top = 1
          Height = 50
          Align = alRight
          ExplicitLeft = 288
          ExplicitTop = 24
          ExplicitHeight = 100
        end
        object GroupBox11: TGroupBox
          Left = 1
          Top = 1
          Width = 438
          Height = 50
          Align = alClient
          Caption = 'Taxa de amostragem'
          TabOrder = 0
          object tbAmostra: TTrackBar
            Left = 3
            Top = 19
            Width = 174
            Height = 27
            Position = 4
            TabOrder = 0
            ThumbLength = 10
            OnEnter = tbAmostraEnter
          end
          object Button1: TButton
            Left = 182
            Top = 13
            Width = 95
            Height = 25
            Action = ActionExibeAmostra
            TabOrder = 1
          end
        end
        object GroupBox13: TGroupBox
          Left = 442
          Top = 1
          Width = 185
          Height = 50
          Align = alRight
          Caption = 'Horizonte de an'#225'lise'
          TabOrder = 1
          Visible = False
          object Label3: TLabel
            Left = 8
            Top = 23
            Width = 70
            Height = 13
            Caption = 'Ano/ Dura'#231#227'o:'
          end
          object rchAno: TRichEdit
            Left = 91
            Top = 20
            Width = 49
            Height = 20
            BiDiMode = bdRightToLeft
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            Lines.Strings = (
              '2011')
            ParentBiDiMode = False
            ParentFont = False
            TabOrder = 0
            Zoom = 100
          end
          object rchDura: TRichEdit
            Left = 148
            Top = 20
            Width = 26
            Height = 20
            BiDiMode = bdRightToLeft
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            Lines.Strings = (
              '2')
            ParentBiDiMode = False
            ParentFont = False
            TabOrder = 1
            Zoom = 100
          end
        end
      end
      object Panel5: TPanel
        Left = 16
        Top = 352
        Width = 385
        Height = 81
        Caption = 'Panel5'
        TabOrder = 4
        Visible = False
        object ListView1: TListView
          Left = 1
          Top = 1
          Width = 383
          Height = 79
          Align = alClient
          Checkboxes = True
          Columns = <
            item
              Width = 20
            end
            item
              Caption = 'IdBarra'
              Width = 125
            end
            item
              Alignment = taRightJustify
              Caption = 'NAmostra'
              Width = 70
            end>
          HideSelection = False
          MultiSelect = True
          RowSelect = True
          TabOrder = 0
          ViewStyle = vsReport
          Visible = False
          OnColumnClick = LViewIndTotalColumnClick
          OnCompare = LViewIndTotalCompare
          OnMouseUp = LViewIndTotalMouseUp
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Relat'#243'rios'
      ImageIndex = 1
      TabVisible = False
      object PageControl2: TPageControl
        Left = 0
        Top = 0
        Width = 628
        Height = 512
        ActivePage = TabSheet3
        Align = alClient
        TabOrder = 0
        object TabSheet3: TTabSheet
          Caption = 'Rede'
          object Splitter1: TSplitter
            Left = 280
            Top = 24
            Height = 460
            ExplicitLeft = 304
            ExplicitTop = 30
            ExplicitHeight = 388
          end
          object ToolBar2: TToolBar
            Left = 0
            Top = 0
            Width = 620
            Height = 24
            ButtonHeight = 13
            ButtonWidth = 8
            Caption = 'ToolBar1'
            EdgeBorders = [ebBottom]
            ShowCaptions = True
            TabOrder = 0
            object Label1: TLabel
              Left = 0
              Top = 0
              Width = 105
              Height = 13
              Caption = 'Tipo de curto-circuito:'
            end
            object ComboBox1: TComboBox
              Left = 105
              Top = 0
              Width = 145
              Height = 21
              TabOrder = 0
              Text = 'ComboBox1'
            end
          end
          object GroupBox1: TGroupBox
            Left = 0
            Top = 24
            Width = 280
            Height = 460
            Align = alLeft
            Caption = 'Rede Base'
            TabOrder = 1
            object ListView2: TListView
              Left = 2
              Top = 15
              Width = 276
              Height = 145
              Align = alTop
              Columns = <
                item
                  Caption = 'Rede'
                  Width = 60
                end
                item
                  Alignment = taCenter
                  Caption = 'Total'
                  Width = 60
                end
                item
                  Alignment = taCenter
                  Caption = 'Alta'
                end
                item
                  Alignment = taCenter
                  Caption = 'M'#233'dia'
                end
                item
                  Alignment = taCenter
                  Caption = 'Baixa'
                end>
              TabOrder = 0
              ViewStyle = vsReport
            end
            object Chart1: TChart
              Left = 2
              Top = 160
              Width = 276
              Height = 298
              BackWall.Brush.Style = bsClear
              Legend.CheckBoxes = True
              Title.Font.Color = clBlack
              Title.Font.Height = -12
              Title.Text.Strings = (
                'Frequ'#234'ncia de afundamentos por rede')
              AxisBehind = False
              BottomAxis.Title.Caption = 'Faixa de Intensidade'
              DepthAxis.Automatic = False
              DepthAxis.AutomaticMaximum = False
              DepthAxis.AutomaticMinimum = False
              DepthAxis.Maximum = 0.829999999999989500
              DepthAxis.Minimum = -0.170000000000004400
              DepthTopAxis.Automatic = False
              DepthTopAxis.AutomaticMaximum = False
              DepthTopAxis.AutomaticMinimum = False
              DepthTopAxis.Maximum = 0.829999999999989500
              DepthTopAxis.Minimum = -0.170000000000004400
              LeftAxis.Automatic = False
              LeftAxis.AutomaticMaximum = False
              LeftAxis.AutomaticMinimum = False
              LeftAxis.Maximum = 100.000000000000000000
              LeftAxis.Title.Caption = 'Frequ'#234'ncia %'
              View3D = False
              Align = alClient
              TabOrder = 1
              DefaultCanvas = 'TGDIPlusCanvas'
              PrintMargins = (
                15
                25
                15
                25)
              ColorPaletteIndex = 13
              object Series1: TBarSeries
                Marks.Visible = False
                Title = 'Rede 1'
                XValues.Name = 'X'
                XValues.Order = loAscending
                YValues.Name = 'Bar'
                YValues.Order = loNone
                Data = {
                  0006000000000000000000000000000000000024400000000000004E40000000
                  000000344000000000000014400000000000000840}
              end
              object Series2: TBarSeries
                Marks.Visible = False
                Title = 'Rede 2'
                XValues.Name = 'X'
                XValues.Order = loAscending
                YValues.Name = 'Bar'
                YValues.Order = loNone
                Data = {
                  000600000000000000000000000000000000002E400000000000804140000000
                  000000494000000000000024400000000000001440}
              end
              object Series3: TBarSeries
                Marks.Visible = False
                Title = 'Total'
                XValues.Name = 'X'
                XValues.Order = loAscending
                YValues.Name = 'Bar'
                YValues.Order = loNone
                Data = {
                  0006000000000000000000144000000000000024400000000000003940000000
                  0000804B400000000000003E400000000000002440}
              end
            end
          end
          object GroupBox2: TGroupBox
            Left = 283
            Top = 24
            Width = 337
            Height = 460
            Align = alClient
            Caption = 'Rede com GD'
            TabOrder = 2
            object ListView5: TListView
              Left = 2
              Top = 15
              Width = 333
              Height = 145
              Align = alTop
              Columns = <
                item
                  Caption = 'Rede'
                  Width = 60
                end
                item
                  Alignment = taCenter
                  Caption = 'Total'
                  Width = 60
                end
                item
                  Alignment = taCenter
                  Caption = 'Alta'
                end
                item
                  Alignment = taCenter
                  Caption = 'M'#233'dia'
                end
                item
                  Alignment = taCenter
                  Caption = 'Baixa'
                end>
              TabOrder = 0
              ViewStyle = vsReport
            end
            object Chart3: TChart
              Left = 2
              Top = 160
              Width = 333
              Height = 298
              BackWall.Brush.Style = bsClear
              Legend.CheckBoxes = True
              Title.Font.Color = clBlack
              Title.Font.Height = -12
              Title.Text.Strings = (
                'Frequ'#234'ncia de afundamentos por rede')
              AxisBehind = False
              BottomAxis.Title.Caption = 'Faixa de Intensidade'
              DepthAxis.Automatic = False
              DepthAxis.AutomaticMaximum = False
              DepthAxis.AutomaticMinimum = False
              DepthAxis.Maximum = 0.829999999999989500
              DepthAxis.Minimum = -0.170000000000004400
              DepthTopAxis.Automatic = False
              DepthTopAxis.AutomaticMaximum = False
              DepthTopAxis.AutomaticMinimum = False
              DepthTopAxis.Maximum = 0.829999999999989500
              DepthTopAxis.Minimum = -0.170000000000004400
              LeftAxis.Automatic = False
              LeftAxis.AutomaticMaximum = False
              LeftAxis.AutomaticMinimum = False
              LeftAxis.Maximum = 100.000000000000000000
              LeftAxis.Title.Caption = 'Frequ'#234'ncia %'
              View3D = False
              Align = alClient
              TabOrder = 1
              DefaultCanvas = 'TGDIPlusCanvas'
              PrintMargins = (
                15
                25
                15
                25)
              ColorPaletteIndex = 13
              object BarSeries1: TBarSeries
                Marks.Visible = False
                Title = 'Rede 1'
                XValues.Name = 'X'
                XValues.Order = loAscending
                YValues.Name = 'Bar'
                YValues.Order = loNone
                Data = {
                  0006000000000000000000000000000000000024400000000000004E40000000
                  000000344000000000000014400000000000000840}
              end
              object BarSeries2: TBarSeries
                Marks.Visible = False
                Title = 'Rede 2'
                XValues.Name = 'X'
                XValues.Order = loAscending
                YValues.Name = 'Bar'
                YValues.Order = loNone
                Data = {
                  000600000000000000000000000000000000002E400000000000804140000000
                  000000494000000000000024400000000000001440}
              end
              object BarSeries3: TBarSeries
                Marks.Visible = False
                Title = 'Total'
                XValues.Name = 'X'
                XValues.Order = loAscending
                YValues.Name = 'Bar'
                YValues.Order = loNone
                Data = {
                  0006000000000000000000144000000000000024400000000000003940000000
                  0000804B400000000000003E400000000000002440}
              end
            end
          end
        end
        object TabSheet4: TTabSheet
          Caption = 'Barra'
          ImageIndex = 1
          object ToolBar3: TToolBar
            Left = 0
            Top = 0
            Width = 620
            Height = 24
            ButtonHeight = 13
            ButtonWidth = 8
            Caption = 'ToolBar1'
            EdgeBorders = [ebBottom]
            ShowCaptions = True
            TabOrder = 0
            object Label2: TLabel
              Left = 0
              Top = 0
              Width = 29
              Height = 13
              Caption = 'Rede:'
            end
            object ComboBox2: TComboBox
              Left = 29
              Top = 0
              Width = 145
              Height = 21
              TabOrder = 0
              Text = 'ComboBox1'
            end
          end
          object Chart2: TChart
            Left = 0
            Top = 230
            Width = 620
            Height = 254
            BackWall.Brush.Style = bsClear
            Title.Font.Color = clBlack
            Title.Font.Height = -12
            Title.Text.Strings = (
              'PERFIL DE VTCD NO TRONCO: Rede:  /   Patamar:'
              '')
            BottomAxis.Automatic = False
            BottomAxis.AutomaticMaximum = False
            BottomAxis.AutomaticMinimum = False
            BottomAxis.Title.Caption = 'pbar'
            DepthAxis.Automatic = False
            DepthAxis.AutomaticMaximum = False
            DepthAxis.AutomaticMinimum = False
            DepthAxis.Maximum = 0.829999999999989500
            DepthAxis.Minimum = -0.170000000000004400
            DepthTopAxis.Automatic = False
            DepthTopAxis.AutomaticMaximum = False
            DepthTopAxis.AutomaticMinimum = False
            DepthTopAxis.Maximum = 0.829999999999989500
            DepthTopAxis.Minimum = -0.170000000000004400
            LeftAxis.Automatic = False
            LeftAxis.AutomaticMaximum = False
            LeftAxis.AutomaticMinimum = False
            LeftAxis.Title.Caption = 'vpu'
            RightAxis.Automatic = False
            RightAxis.AutomaticMaximum = False
            RightAxis.AutomaticMinimum = False
            RightAxis.Title.Caption = '#Trechos/Geradores'
            View3D = False
            Align = alClient
            TabOrder = 1
            DefaultCanvas = 'TGDIPlusCanvas'
            PrintMargins = (
              15
              9
              15
              9)
            ColorPaletteIndex = 13
            object Series0: TLineSeries
              Marks.Callout.Length = 0
              SeriesColor = 16737380
              Title = 'Rede base'
              Brush.BackColor = clDefault
              LinePen.Width = 2
              Pointer.HorizSize = 2
              Pointer.InflateMargins = True
              Pointer.Style = psCircle
              Pointer.VertSize = 2
              XValues.Name = 'X'
              XValues.Order = loAscending
              YValues.Name = 'Y'
              YValues.Order = loNone
            end
            object LineSeries1: TLineSeries
              SeriesColor = 7274496
              Title = 'Rede com GD'
              Brush.BackColor = clDefault
              LinePen.Width = 2
              Pointer.HorizSize = 2
              Pointer.InflateMargins = True
              Pointer.Style = psCircle
              Pointer.VertSize = 2
              XValues.Name = 'X'
              XValues.Order = loAscending
              YValues.Name = 'Y'
              YValues.Order = loNone
            end
          end
          object Panel1: TPanel
            Left = 0
            Top = 24
            Width = 620
            Height = 206
            Align = alTop
            Caption = 'Panel1'
            TabOrder = 2
            object Splitter2: TSplitter
              Left = 272
              Top = 1
              Height = 204
              ExplicitLeft = 304
              ExplicitTop = 30
            end
            object GroupBox4: TGroupBox
              Left = 275
              Top = 1
              Width = 344
              Height = 204
              Align = alClient
              Caption = 'Rede com GD'
              TabOrder = 0
              object ListView3: TListView
                Left = 2
                Top = 15
                Width = 340
                Height = 187
                Align = alClient
                Columns = <
                  item
                    Caption = 'Rede'
                  end
                  item
                    Caption = 'Barra'
                  end
                  item
                    Alignment = taCenter
                    Caption = 'Total'
                    Width = 40
                  end
                  item
                    Alignment = taCenter
                    Caption = 'Alta'
                    Width = 40
                  end
                  item
                    Alignment = taCenter
                    Caption = 'M'#233'dia'
                    Width = 40
                  end
                  item
                    Alignment = taCenter
                    Caption = 'Baixa'
                    Width = 40
                  end>
                TabOrder = 0
                ViewStyle = vsReport
              end
            end
            object GroupBox5: TGroupBox
              Left = 1
              Top = 1
              Width = 271
              Height = 204
              Align = alLeft
              Caption = 'Rede base'
              TabOrder = 1
              object ListView6: TListView
                Left = 2
                Top = 15
                Width = 267
                Height = 187
                Align = alClient
                Columns = <
                  item
                    Caption = 'Rede'
                  end
                  item
                    Caption = 'Barra'
                  end
                  item
                    Alignment = taCenter
                    Caption = 'Total'
                    Width = 40
                  end
                  item
                    Alignment = taCenter
                    Caption = 'Alta'
                    Width = 40
                  end
                  item
                    Alignment = taCenter
                    Caption = 'M'#233'dia'
                    Width = 40
                  end
                  item
                    Alignment = taCenter
                    Caption = 'Baixa'
                    Width = 40
                  end>
                TabOrder = 0
                ViewStyle = vsReport
              end
            end
          end
        end
        object TabSheet5: TTabSheet
          Caption = #205'ndices de qualidade de servi'#231'o'
          ImageIndex = 2
          object Panel2: TPanel
            Left = 0
            Top = 0
            Width = 620
            Height = 484
            Align = alClient
            TabOrder = 0
            object Splitter3: TSplitter
              Left = 281
              Top = 1
              Height = 482
              ExplicitLeft = 304
              ExplicitTop = 30
              ExplicitHeight = 175
            end
            object GroupBox6: TGroupBox
              Left = 1
              Top = 1
              Width = 280
              Height = 482
              Align = alLeft
              Caption = 'Rede base'
              TabOrder = 0
              object ListView4: TListView
                Left = 2
                Top = 15
                Width = 276
                Height = 162
                Align = alTop
                Columns = <
                  item
                    Caption = 'Rede'
                    Width = 100
                  end
                  item
                    Alignment = taCenter
                    Caption = 'SARFI'
                    Width = 80
                  end
                  item
                    Alignment = taCenter
                    Caption = 'ASIDI'
                    Width = 80
                  end>
                TabOrder = 0
                ViewStyle = vsReport
              end
              object Chart4: TChart
                Left = 2
                Top = 177
                Width = 276
                Height = 303
                BackWall.Brush.Style = bsClear
                Legend.CheckBoxes = True
                Title.Font.Color = clBlack
                Title.Font.Height = -12
                Title.Text.Strings = (
                  'Indicadores de qualidade')
                AxisBehind = False
                BottomAxis.Title.Caption = 'Faixa de Intensidade'
                DepthAxis.Automatic = False
                DepthAxis.AutomaticMaximum = False
                DepthAxis.AutomaticMinimum = False
                DepthAxis.Maximum = 0.829999999999989500
                DepthAxis.Minimum = -0.170000000000004400
                DepthTopAxis.Automatic = False
                DepthTopAxis.AutomaticMaximum = False
                DepthTopAxis.AutomaticMinimum = False
                DepthTopAxis.Maximum = 0.829999999999989500
                DepthTopAxis.Minimum = -0.170000000000004400
                LeftAxis.Automatic = False
                LeftAxis.AutomaticMaximum = False
                LeftAxis.AutomaticMinimum = False
                LeftAxis.Maximum = 100.000000000000000000
                LeftAxis.Title.Caption = 'Frequ'#234'ncia %'
                View3D = False
                Align = alClient
                TabOrder = 1
                DefaultCanvas = 'TGDIPlusCanvas'
                PrintMargins = (
                  15
                  25
                  15
                  25)
                ColorPaletteIndex = 13
                object BarSeries4: TBarSeries
                  Marks.Visible = False
                  Title = 'Rede 1'
                  XValues.Name = 'X'
                  XValues.Order = loAscending
                  YValues.Name = 'Bar'
                  YValues.Order = loNone
                  Data = {
                    0006000000000000000000000000000000000024400000000000004E40000000
                    000000344000000000000014400000000000000840}
                end
                object BarSeries5: TBarSeries
                  Marks.Visible = False
                  Title = 'Rede 2'
                  XValues.Name = 'X'
                  XValues.Order = loAscending
                  YValues.Name = 'Bar'
                  YValues.Order = loNone
                  Data = {
                    000600000000000000000000000000000000002E400000000000804140000000
                    000000494000000000000024400000000000001440}
                end
                object BarSeries6: TBarSeries
                  Marks.Visible = False
                  Title = 'Total'
                  XValues.Name = 'X'
                  XValues.Order = loAscending
                  YValues.Name = 'Bar'
                  YValues.Order = loNone
                  Data = {
                    0006000000000000000000144000000000000024400000000000003940000000
                    0000804B400000000000003E400000000000002440}
                end
              end
            end
            object GroupBox7: TGroupBox
              Left = 284
              Top = 1
              Width = 335
              Height = 482
              Align = alClient
              Caption = 'Rede com GD'
              TabOrder = 1
              object ListView7: TListView
                Left = 2
                Top = 15
                Width = 331
                Height = 162
                Align = alTop
                Columns = <
                  item
                    Caption = 'Rede'
                    Width = 100
                  end
                  item
                    Alignment = taCenter
                    Caption = 'SARFI'
                    Width = 80
                  end
                  item
                    Alignment = taCenter
                    Caption = 'ASIDI'
                    Width = 80
                  end>
                TabOrder = 0
                ViewStyle = vsReport
              end
              object Chart5: TChart
                Left = 2
                Top = 177
                Width = 331
                Height = 303
                BackWall.Brush.Style = bsClear
                Legend.CheckBoxes = True
                Title.Font.Color = clBlack
                Title.Font.Height = -12
                Title.Text.Strings = (
                  'Indicadores de qualidade')
                AxisBehind = False
                BottomAxis.Title.Caption = 'Faixa de Intensidade'
                DepthAxis.Automatic = False
                DepthAxis.AutomaticMaximum = False
                DepthAxis.AutomaticMinimum = False
                DepthAxis.Maximum = 0.829999999999989500
                DepthAxis.Minimum = -0.170000000000004400
                DepthTopAxis.Automatic = False
                DepthTopAxis.AutomaticMaximum = False
                DepthTopAxis.AutomaticMinimum = False
                DepthTopAxis.Maximum = 0.829999999999989500
                DepthTopAxis.Minimum = -0.170000000000004400
                LeftAxis.Automatic = False
                LeftAxis.AutomaticMaximum = False
                LeftAxis.AutomaticMinimum = False
                LeftAxis.Maximum = 100.000000000000000000
                LeftAxis.Title.Caption = 'Frequ'#234'ncia %'
                View3D = False
                Align = alClient
                TabOrder = 1
                DefaultCanvas = 'TGDIPlusCanvas'
                PrintMargins = (
                  15
                  25
                  15
                  25)
                ColorPaletteIndex = 13
                object BarSeries7: TBarSeries
                  Marks.Visible = False
                  Title = 'Rede 1'
                  XValues.Name = 'X'
                  XValues.Order = loAscending
                  YValues.Name = 'Bar'
                  YValues.Order = loNone
                  Data = {
                    0006000000000000000000000000000000000024400000000000004E40000000
                    000000344000000000000014400000000000000840}
                end
                object BarSeries8: TBarSeries
                  Marks.Visible = False
                  Title = 'Rede 2'
                  XValues.Name = 'X'
                  XValues.Order = loAscending
                  YValues.Name = 'Bar'
                  YValues.Order = loNone
                  Data = {
                    000600000000000000000000000000000000002E400000000000804140000000
                    000000494000000000000024400000000000001440}
                end
                object BarSeries9: TBarSeries
                  Marks.Visible = False
                  Title = 'Total'
                  XValues.Name = 'X'
                  XValues.Order = loAscending
                  YValues.Name = 'Bar'
                  YValues.Order = loNone
                  Data = {
                    0006000000000000000000144000000000000024400000000000003940000000
                    0000804B400000000000003E400000000000002440}
                end
              end
            end
          end
        end
      end
    end
    object tbsIndicVTCD: TTabSheet
      Caption = 'Indicadores de VTCD (Eventos/ano)'
      Enabled = False
      ImageIndex = 4
      object Splitter4: TSplitter
        Left = 0
        Top = 197
        Width = 628
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ExplicitLeft = 7
        ExplicitTop = 208
        ExplicitWidth = 373
      end
      object Splitter10: TSplitter
        Left = 0
        Top = 97
        Width = 628
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ExplicitTop = 100
        ExplicitWidth = 329
      end
      object Splitter11: TSplitter
        Left = 0
        Top = 314
        Width = 628
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ExplicitWidth = 90
      end
      object GroupBox3: TGroupBox
        Left = 0
        Top = 100
        Width = 628
        Height = 97
        Align = alTop
        TabOrder = 0
        object LViewIndRede: TListView
          Left = 2
          Top = 15
          Width = 624
          Height = 80
          Align = alClient
          Checkboxes = True
          Columns = <
            item
              Width = 40
            end
            item
              Caption = 'Rede'
              Width = 125
            end
            item
              Alignment = taRightJustify
              Caption = 'SARFI 90'
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SARFI 70  '
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SARFI 40       '
              Width = 70
            end
            item
              Caption = 'Interrup'#231#245'es'
              Width = 70
            end
            item
              Caption = 'Consumidores'
            end
            item
              Caption = 'Alternativa'
              Width = 125
            end>
          HideSelection = False
          MultiSelect = True
          RowSelect = True
          TabOrder = 0
          ViewStyle = vsReport
          OnClick = LViewIndRedeClick
          OnColumnClick = LViewIndRedeColumnClick
          OnCompare = LViewIndRedeCompare
          OnMouseUp = LViewIndRedeMouseUp
        end
      end
      object GroupBox8: TGroupBox
        Left = 0
        Top = 200
        Width = 628
        Height = 114
        Align = alTop
        TabOrder = 1
        object LViewIndCarga: TListView
          Left = 2
          Top = 15
          Width = 624
          Height = 97
          Align = alClient
          Columns = <
            item
              Width = 1
            end
            item
              Alignment = taCenter
              Caption = 'Carga'
              Width = 165
            end
            item
              Alignment = taRightJustify
              Caption = 'SARFI 90'
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SARFI 70  '
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SARFI 40       '
              Width = 70
            end
            item
              Caption = 'Interrup'#231#245'es'
              Width = 70
            end
            item
              Caption = 'Consumidores'
            end
            item
              Caption = 'Alternativa'
              Width = 125
            end
            item
              Caption = 'Rede'
              Width = 70
            end>
          HideSelection = False
          RowSelect = True
          TabOrder = 0
          ViewStyle = vsReport
          OnClick = LViewIndCargaClick
          OnColumnClick = LViewIndCargaColumnClick
          OnCompare = LViewIndCargaCompare
          OnDblClick = LViewIndCargaDblClick
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 317
        Width = 628
        Height = 195
        Align = alClient
        TabOrder = 2
        object chtEvRede: TChart
          Left = 1
          Top = 1
          Width = 626
          Height = 193
          Legend.CheckBoxes = True
          Title.Text.Strings = (
            'Eventos/Ano')
          DepthAxis.Automatic = False
          DepthAxis.AutomaticMaximum = False
          DepthAxis.AutomaticMinimum = False
          DepthAxis.Maximum = 0.500000000000000000
          DepthAxis.Minimum = -0.500000000000000200
          DepthTopAxis.Automatic = False
          DepthTopAxis.AutomaticMaximum = False
          DepthTopAxis.AutomaticMinimum = False
          DepthTopAxis.Maximum = 0.500000000000000000
          DepthTopAxis.Minimum = -0.500000000000000200
          RightAxis.Automatic = False
          RightAxis.AutomaticMaximum = False
          RightAxis.AutomaticMinimum = False
          View3D = False
          View3DOptions.Orthogonal = False
          Align = alClient
          TabOrder = 0
          DefaultCanvas = 'TGDIPlusCanvas'
          PrintMargins = (
            15
            30
            15
            30)
          ColorPaletteIndex = 13
          object Series4: TBarSeries
            Marks.Visible = False
            Title = 'SARFI 90'
            XValues.Name = 'X'
            XValues.Order = loAscending
            YValues.Name = 'Bar'
            YValues.Order = loNone
          end
          object Series5: TBarSeries
            Marks.Visible = False
            Title = 'SARFI 70'
            XValues.Name = 'X'
            XValues.Order = loAscending
            YValues.Name = 'Bar'
            YValues.Order = loNone
          end
          object Series6: TBarSeries
            Marks.Visible = False
            Title = 'SARFI 40'
            XValues.Name = 'X'
            XValues.Order = loAscending
            YValues.Name = 'Bar'
            YValues.Order = loNone
          end
          object Series7: TBarSeries
            Marks.Visible = False
            Title = 'Interrup'#231'oes'
            XValues.Name = 'X'
            XValues.Order = loAscending
            YValues.Name = 'Bar'
            YValues.Order = loNone
          end
        end
      end
      object GroupBox9: TGroupBox
        Left = 0
        Top = 0
        Width = 628
        Height = 97
        Align = alTop
        TabOrder = 3
        object LViewIndTotal: TListView
          Left = 2
          Top = 15
          Width = 624
          Height = 80
          Align = alClient
          Checkboxes = True
          Columns = <
            item
              Width = 40
            end
            item
              Caption = 'Alternativa'
              Width = 125
            end
            item
              Alignment = taRightJustify
              Caption = 'SARFI 90'
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SARFI 70  '
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SARFI 40       '
              Width = 70
            end
            item
              Caption = 'Interrup'#231#245'es'
              Width = 70
            end
            item
              Caption = 'Consumidores'
            end>
          HideSelection = False
          MultiSelect = True
          RowSelect = True
          TabOrder = 0
          ViewStyle = vsReport
          OnColumnClick = LViewIndTotalColumnClick
          OnCompare = LViewIndTotalCompare
          OnMouseUp = LViewIndTotalMouseUp
        end
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'Distribui'#231#245'es'
      ImageIndex = 2
      TabVisible = False
      object panelAfundaBarra: TPanel
        Left = 0
        Top = 30
        Width = 474
        Height = 397
        TabOrder = 0
      end
      object ToolBar4: TToolBar
        Left = 0
        Top = 0
        Width = 628
        Height = 24
        Anchors = [akLeft, akRight, akBottom]
        ButtonHeight = 21
        ButtonWidth = 107
        Caption = 'ToolBar1'
        EdgeBorders = [ebBottom]
        ShowCaptions = True
        TabOrder = 1
        object ToolButton1: TToolButton
          Left = 0
          Top = 0
          Action = ActionReinciaBarras
        end
        object ToolButton2: TToolButton
          Left = 107
          Top = 0
          Action = ActionMostraFreqBarras
        end
        object ToolButton5: TToolButton
          Left = 214
          Top = 0
          Action = ActionMostraAfundamentos
        end
      end
      object panelCargas: TPanel
        Left = 480
        Top = 30
        Width = 89
        Height = 128
        TabOrder = 2
      end
    end
    object TabSheet7: TTabSheet
      Caption = 'Afundamentos por barra'
      ImageIndex = 3
      TabVisible = False
      object LViewBarras: TListView
        Left = 0
        Top = 0
        Width = 628
        Height = 121
        Align = alTop
        Columns = <
          item
            Caption = 'C'#243'digo'
          end
          item
            Alignment = taRightJustify
            Caption = 'Id'
          end
          item
            Alignment = taRightJustify
            Caption = 'afundamentos/ano   '
            Width = 150
          end
          item
            Alignment = taRightJustify
            Caption = 'Interrup'#231#245'es/ano        '
            Width = 150
          end
          item
            Alignment = taRightJustify
            Caption = 'm'#237'nimo'
          end
          item
            Alignment = taRightJustify
            Caption = 'm'#233'dio'
          end
          item
            Alignment = taRightJustify
            Caption = 'm'#225'ximo'
          end>
        TabOrder = 0
        ViewStyle = vsReport
        OnClick = LViewBarrasClick
      end
      object GroupBox14: TGroupBox
        Left = 0
        Top = 121
        Width = 628
        Height = 336
        Align = alTop
        TabOrder = 1
        object LViewIndAuxCarga: TListView
          Left = 2
          Top = 237
          Width = 624
          Height = 97
          Align = alBottom
          Columns = <
            item
            end
            item
              Caption = 'Alternativa'
              Width = 125
            end
            item
              Caption = 'Rede'
              Width = 70
            end
            item
              Caption = 'Carga'
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SAIFI90'
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SAIFI70  '
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SAIFI40       '
              Width = 70
            end
            item
              Caption = 'Interrup'#231#245'es'
              Width = 70
            end
            item
              Caption = 'Consumidores'
            end>
          HideSelection = False
          RowSelect = True
          TabOrder = 0
          ViewStyle = vsReport
          OnClick = LViewIndCargaClick
          OnColumnClick = LViewIndCargaColumnClick
          OnCompare = LViewIndCargaCompare
        end
        object LViewIndAuxRede: TListView
          Left = 2
          Top = 138
          Width = 624
          Height = 99
          Align = alBottom
          Columns = <
            item
            end
            item
              Caption = 'Alternativa'
              Width = 125
            end
            item
              Alignment = taRightJustify
              Caption = 'SAIFIs90'
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SAIFIs70  '
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SAIFIs40       '
              Width = 70
            end
            item
              Caption = 'Interrup'#231#245'es'
              Width = 70
            end
            item
              Caption = 'Rede'
              Width = 70
            end
            item
              Caption = 'Consumidores'
            end>
          HideSelection = False
          MultiSelect = True
          RowSelect = True
          TabOrder = 1
          ViewStyle = vsReport
          OnClick = LViewBarrasClick
          OnColumnClick = LViewIndRedeColumnClick
          OnCompare = LViewIndRedeCompare
        end
        object LViewIndAuxTotal: TListView
          Left = 2
          Top = 15
          Width = 624
          Height = 123
          Align = alClient
          Columns = <
            item
            end
            item
              Caption = 'Alternativa'
              Width = 125
            end
            item
              Alignment = taRightJustify
              Caption = 'SAIFIs90'
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SAIFIs70  '
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = 'SAIFIs40       '
              Width = 70
            end
            item
              Caption = 'Interrup'#231#245'es'
              Width = 70
            end
            item
              Caption = 'Consumidores'
            end>
          HideSelection = False
          MultiSelect = True
          RowSelect = True
          TabOrder = 2
          ViewStyle = vsReport
          OnColumnClick = LViewIndTotalColumnClick
          OnCompare = LViewIndTotalCompare
        end
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'TabSheet1'
      ImageIndex = 5
      TabVisible = False
      object GroupBox15: TGroupBox
        Left = 29
        Top = 24
        Width = 353
        Height = 303
        Caption = 'Configura'#231#227'o gr'#225'fica'
        TabOrder = 0
        object Shape1: TShape
          Left = 72
          Top = 31
          Width = 81
          Height = 17
          Brush.Color = clLime
        end
        object Shape2: TShape
          Left = 152
          Top = 31
          Width = 89
          Height = 17
          Brush.Color = clYellow
        end
        object Shape3: TShape
          Left = 240
          Top = 31
          Width = 81
          Height = 17
          Brush.Color = clRed
        end
        object Label4: TLabel
          Left = 10
          Top = 194
          Width = 45
          Height = 13
          Caption = 'Leganda:'
        end
        object Shape4: TShape
          Left = 10
          Top = 213
          Width = 23
          Height = 14
          Brush.Color = clLime
        end
        object Label5: TLabel
          Left = 50
          Top = 213
          Width = 73
          Height = 13
          Caption = #205'ndice < limite1'
        end
        object Shape5: TShape
          Left = 10
          Top = 234
          Width = 23
          Height = 14
          Brush.Color = clYellow
        end
        object Shape6: TShape
          Left = 10
          Top = 255
          Width = 23
          Height = 14
          Brush.Color = clRed
        end
        object Label6: TLabel
          Left = 134
          Top = 13
          Width = 33
          Height = 13
          Caption = ' limite1'
        end
        object Label7: TLabel
          Left = 225
          Top = 13
          Width = 33
          Height = 13
          Caption = ' limite2'
        end
        object Label8: TLabel
          Left = 50
          Top = 255
          Width = 73
          Height = 13
          Caption = #205'ndice > limite2'
        end
        object Label9: TLabel
          Left = 50
          Top = 232
          Width = 133
          Height = 13
          Caption = #205'ndice entre limite1 e limite2'
        end
        object Shape7: TShape
          Left = 10
          Top = 275
          Width = 23
          Height = 14
          Brush.Color = clGray
        end
        object Label10: TLabel
          Left = 49
          Top = 275
          Width = 55
          Height = 13
          Caption = 'Sem '#237'ndices'
        end
        object dec1: TEdit
          Left = 129
          Top = 55
          Width = 57
          Height = 21
          TabOrder = 0
          Text = '1,0'
        end
        object dec2: TEdit
          Left = 225
          Top = 55
          Width = 57
          Height = 21
          TabOrder = 1
          Text = '3,2'
        end
        object butDec: TRadioButton
          Left = 10
          Top = 55
          Width = 113
          Height = 17
          Caption = 'DEC (h/ano)'
          Checked = True
          TabOrder = 2
          TabStop = True
        end
        object butFec: TRadioButton
          Left = 10
          Top = 91
          Width = 113
          Height = 17
          Caption = 'FEC (int./ano)'
          TabOrder = 3
        end
        object fec1: TEdit
          Left = 129
          Top = 89
          Width = 57
          Height = 21
          TabOrder = 4
          Text = '1,5'
        end
        object fec2: TEdit
          Left = 225
          Top = 89
          Width = 57
          Height = 21
          TabOrder = 5
          Text = '2,5'
        end
        object butEnd: TRadioButton
          Left = 10
          Top = 123
          Width = 113
          Height = 17
          Caption = 'END (MWh/ano)'
          TabOrder = 6
        end
        object end1: TEdit
          Left = 129
          Top = 123
          Width = 57
          Height = 21
          TabOrder = 7
          Text = '1'
        end
        object end2: TEdit
          Left = 225
          Top = 123
          Width = 57
          Height = 21
          TabOrder = 8
          Text = '10'
        end
        object butPot: TRadioButton
          Left = 10
          Top = 154
          Width = 113
          Height = 17
          Caption = 'Pot'#234'ncia (MW)'
          TabOrder = 9
        end
        object pot1: TEdit
          Left = 129
          Top = 152
          Width = 57
          Height = 21
          TabOrder = 10
          Text = '1'
        end
        object pot2: TEdit
          Left = 225
          Top = 152
          Width = 57
          Height = 21
          TabOrder = 11
          Text = '10'
        end
      end
    end
    object TabSheet8: TTabSheet
      Caption = 'TabSheet8'
      ImageIndex = 6
    end
  end
  object gbAlt: TGroupBox [3]
    Left = 0
    Top = 25
    Width = 210
    Height = 540
    Align = alLeft
    TabOrder = 2
    object LViewAlt: TListView
      Left = 2
      Top = 15
      Width = 206
      Height = 523
      Align = alClient
      Checkboxes = True
      Columns = <
        item
          Caption = 'Alternativa'
          Width = 210
        end
        item
          Caption = 'Obra'
          Width = 0
        end
        item
          Caption = 'Ano'
          Width = 0
        end>
      HideSelection = False
      MultiSelect = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
      OnClick = LViewAltClick
      OnDragDrop = LViewAltDragDrop
      OnDragOver = LViewAltDragOver
    end
  end
  inherited ActionList: TActionList
    Left = 64
    Top = 384
    object ActionSelEqpto: TAction
      OnExecute = ActionSelEqptoExecute
    end
  end
  inherited ImageList: TImageList
    Left = 128
    Top = 384
    Bitmap = {
      494C010108000900240010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000003000000001002000000000000030
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF000000FF000000FF000000FF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000BFBFBF0000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF0000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000BFBFBF0000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF0000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FF
      FF0000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000BFBFBF0000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF000000000000FFFF000000000000FFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000FFFF000000000000FFFF000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000007F7F
      7F00000000007F7F7F0000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FF000000FF0000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      00000000FF000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FF000000FF0000000000000000000000000000000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F000000000000000000000000007F7F7F00000000000000
      FF00000000007F7F7F0000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FF000000FFFFFF000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F00000000000000FF000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF00000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FF000000FFFFFF000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF0000000000000000000000FF00000000007F7F
      7F00000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F00000000000000
      00000000000000000000000000000000000000000000FFFF00000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFFFF00FFFF
      FF00FF000000FF000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000FFFF000000000000FFFF000000000000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF00000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF007F7F7F00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF00FF000000FF000000FFFFFF00FFFFFF000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF007F7F7F00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000FFFF000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      000000FFFF00FFFFFF0000000000FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF0000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF0000000000000000007F7F
      7F00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000000000000000FFFFFF0000FF
      FF000000000000FFFF00FFFFFF0000000000FFFFFF0000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007F7F7F00000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000000000FFFFFF0000000000FFFF
      FF00FFFFFF00FFFFFF000000FF000000FF000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF000000FF000000000000000000
      000000000000000000000000000000FFFF00FFFFFF000000000000000000FFFF
      FF00FFFFFF00FFFFFF000000FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00000000000000
      000000000000000000000000000000000000FF000000FF0000000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF00FFFF
      FF000000FF000000FF00FFFFFF00FFFFFF000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007F7F7F000000000000000000000000007F7F7F000000
      000000000000000000007F7F7F000000000000000000000000000000FF000000
      FF0000000000000000000000000000000000FFFFFF000000FF000000FF00FFFF
      FF000000FF000000FF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000FF000000FF0000000000FF00
      0000FF000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF00FFFFFF000000FF000000FF000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF0000000000FF00
      0000FF000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF00FFFFFF000000FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000300000000100010000000000800100000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFFFFFFE00FC007FFFFFFFFE00FC007
      FFF81FFFE00FC007F820041FF00FC007F000000FF00FC007F000000FF80FC007
      E0000007F007C00780000001F007C00700000000F007C00780000001F00FC007
      FC00003FF01FC007FE3FFC7FF81FC007FFFFFFFFFE1FC007FFFFFFFFFF1FC007
      FFFFFFFFFF1FC007FFFFFFFFFFBFC007FFFFFC7FFFFFFDFFFFE3F83FC600F8FF
      FFC3F01F8200F8FFFB83F01F0000F87FF907F01F0000F81FF80FF01F0000F80F
      F01FF01F8000F00FF03FF01F8000E00FE01FF83F8000E00FE00FFEC38000E00F
      C07FFEB9C800F01FC1FFFF7D3C00F00F87FFFF3D0E00F00F9FFFFC99CE00F007
      FFFFF9C32600F007FFFFF3FF2600F00700000000000000000000000000000000
      000000000000}
  end
  inherited PopupMenu: TPopupMenu
    Left = 160
    Top = 384
  end
  object ActionList1: TActionList
    Images = ImageList
    Left = 96
    Top = 384
    object ActionExecutar: TAction
      Caption = 'Executar'
      ImageIndex = 7
      OnExecute = ActionExecutarExecute
    end
    object ActionReinciaBarras: TAction
      Caption = 'Reinicia gr'#225'fico'
    end
    object ActionMostraAfundamentos: TAction
      Caption = 'Cargas afetadas'
    end
    object ActionMostraFreqBarras: TAction
      Caption = 'Afundamentos/barra'
    end
    object ActionEnablePatamar: TAction
      Caption = #254
      OnExecute = ActionEnablePatamarExecute
    end
    object ActionDisablePatamar: TAction
      Caption = 'o'
      OnExecute = ActionDisablePatamarExecute
    end
    object ActionExibeAmostra: TAction
      Caption = 'Exibir Amostra'
      OnExecute = ActionExibeAmostraExecute
    end
    object ActionDefaultTipoCurto: TAction
      Caption = 'Valores Padr'#227'o'
      OnExecute = ActionDefaultTipoCurtoExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 64
    Top = 344
  end
end
