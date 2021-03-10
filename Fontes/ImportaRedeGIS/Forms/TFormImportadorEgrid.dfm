object FormImportadorEgrid: TFormImportadorEgrid
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'EDP: Importador de arquivo .egrid'
  ClientHeight = 866
  ClientWidth = 982
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 976
    Height = 24
    AutoSize = True
    ButtonWidth = 141
    EdgeBorders = [ebBottom]
    Images = ImageList
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object btnAbrir: TToolButton
      Left = 0
      Top = 0
      Action = ActionAbrirEgrid
    end
    object btnReiniciarRede: TToolButton
      AlignWithMargins = True
      Left = 141
      Top = 0
      Action = ActionRedeClear
      AutoSize = True
    end
    object btnImportar: TToolButton
      Left = 220
      Top = 0
      Action = ActionIniciarImportacao
    end
    object ToolButton1: TToolButton
      Left = 361
      Top = 0
      Action = ActionAborta
    end
    object ToolButton2: TToolButton
      Left = 502
      Top = 0
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 0
      Style = tbsSeparator
    end
    object ToolButton3: TToolButton
      Left = 510
      Top = 0
      Action = ActionHelp
      AutoSize = True
    end
    object ButFecha: TToolButton
      Left = 569
      Top = 0
      Action = ActionFecha
      AutoSize = True
    end
  end
  object PanelOpcao: TPanel
    Left = 0
    Top = 30
    Width = 375
    Height = 836
    Align = alLeft
    TabOrder = 1
    object RadioGroupDestino: TRadioGroup
      AlignWithMargins = True
      Left = 4
      Top = 57
      Width = 367
      Height = 72
      Align = alTop
      Caption = 'Como tratar as redes importadas'
      ItemIndex = 0
      Items.Strings = (
        'Apenas carregar as subesta'#231#245'es selecionadas no SINAPgrid'
        'Salvar todas subesta'#231#245'es selecionadas em uma '#250'nica base Access'
        'Salvar cada subesta'#231#227'o selecionada em uma base Access separada')
      TabOrder = 0
    end
    object RadioGroupRedeSec: TRadioGroup
      AlignWithMargins = True
      Left = 4
      Top = 415
      Width = 367
      Height = 69
      Align = alTop
      Caption = 'Rede Secund'#225'ria'
      ItemIndex = 2
      Items.Strings = (
        'Rede secund'#225'ria completa'
        'Transformador MT/BT com carga concentrada no seu secund'#225'rio'
        'Carga concentrada na rede prim'#225'ria')
      ParentBackground = False
      TabOrder = 1
    end
    object RadioGroupConsPrimario: TRadioGroup
      AlignWithMargins = True
      Left = 4
      Top = 185
      Width = 367
      Height = 55
      Align = alTop
      Caption = 'Consumidor Prim'#225'rio'
      ItemIndex = 0
      Items.Strings = (
        'Energia'
        'Curva de demanda')
      TabOrder = 2
    end
    object GBoxCondutor: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 683
      Width = 367
      Height = 149
      Align = alClient
      Caption = 'Cabo a ser utilizado caso o cabo do trecho n'#227'o esteja definido'
      TabOrder = 3
      object pnlRedePri: TPanel
        Left = 2
        Top = 15
        Width = 363
        Height = 26
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object Label1: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 100
          Height = 20
          Align = alLeft
          AutoSize = False
          Caption = 'Rede Prim'#225'ria'
          Layout = tlCenter
        end
        object CBoxCondutorMT: TComboBox
          AlignWithMargins = True
          Left = 109
          Top = 3
          Width = 251
          Height = 21
          Align = alClient
          Style = csDropDownList
          Sorted = True
          TabOrder = 0
        end
      end
      object pnlRedeSec: TPanel
        Left = 2
        Top = 41
        Width = 363
        Height = 26
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 1
        object Label2: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 100
          Height = 20
          Align = alLeft
          AutoSize = False
          Caption = 'Rede Secund'#225'ria'
          Layout = tlCenter
        end
        object CBoxCondutorBT: TComboBox
          AlignWithMargins = True
          Left = 109
          Top = 3
          Width = 251
          Height = 21
          Align = alClient
          Style = csDropDownList
          Sorted = True
          TabOrder = 0
        end
      end
      object pnlCaboRamal: TPanel
        Left = 2
        Top = 93
        Width = 363
        Height = 26
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 2
        object Label3: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 100
          Height = 20
          Align = alLeft
          AutoSize = False
          Caption = 'Ramal de Liga'#231#227'o'
          Layout = tlCenter
        end
        object CBoxCondutorRL: TComboBox
          AlignWithMargins = True
          Left = 109
          Top = 3
          Width = 251
          Height = 21
          Align = alClient
          Style = csDropDownList
          TabOrder = 0
        end
      end
      object pnlCaboIP: TPanel
        Left = 2
        Top = 67
        Width = 363
        Height = 26
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 3
        object Label4: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 100
          Height = 20
          Align = alLeft
          AutoSize = False
          Caption = 'Ilumina'#231#227'o P'#250'blica'
          Layout = tlCenter
        end
        object CBoxCondutorIP: TComboBox
          AlignWithMargins = True
          Left = 109
          Top = 3
          Width = 251
          Height = 21
          Align = alClient
          Style = csDropDownList
          TabOrder = 0
        end
      end
      object CBoxArranjoMontante: TCheckBox
        AlignWithMargins = True
        Left = 5
        Top = 122
        Width = 357
        Height = 17
        Align = alTop
        Caption = 'Utilizar arranjo do trecho a montante (se houver)'
        Checked = True
        State = cbChecked
        TabOrder = 4
      end
    end
    object GBoxLog: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 246
      Width = 367
      Height = 91
      Align = alTop
      Caption = 'Relat'#243'rios'
      TabOrder = 4
      object CheckBoxLog: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 18
        Width = 354
        Height = 17
        Margins.Left = 6
        Align = alTop
        Caption = 'Gerar relat'#243'rio de log de erros de dados'
        TabOrder = 0
      end
      object CheckBoxCronometro: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 41
        Width = 354
        Height = 17
        Margins.Left = 6
        Align = alTop
        Caption = 'Cronometrar tempos da extra'#231#227'o'
        TabOrder = 1
      end
      object CheckBoxRelEqpto: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 64
        Width = 354
        Height = 17
        Margins.Left = 6
        Align = alTop
        Caption = 'Gerar relat'#243'rio de equipamentos'
        TabOrder = 2
        OnClick = CheckBoxRelEqptoExit
        OnExit = CheckBoxRelEqptoExit
      end
    end
    object GBoxBaseDado: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 367
      Height = 47
      Align = alTop
      Caption = 'Atualiza'#231#227'o de bases de dados'
      TabOrder = 5
      object PanelData: TPanel
        Left = 2
        Top = 15
        Width = 363
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object LabelData: TLabel
          Left = 106
          Top = 0
          Width = 257
          Height = 25
          Align = alClient
          Alignment = taCenter
          Caption = 'Data de refer'#234'ncia dos dados'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
          ExplicitWidth = 192
          ExplicitHeight = 16
        end
        object DateTimePicker: TDateTimePicker
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 100
          Height = 19
          Align = alLeft
          Date = 40697.416249247680000000
          Format = 'dd/MM/yyyy'
          Time = 40697.416249247680000000
          DateFormat = dfLong
          TabOrder = 0
        end
      end
    end
    object GBoxPastaPublica: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 135
      Width = 367
      Height = 44
      Align = alTop
      Caption = 'Pasta destino para as bases Access das redes'
      TabOrder = 6
      object EditPastaPublica: TEdit
        AlignWithMargins = True
        Left = 5
        Top = 18
        Width = 321
        Height = 21
        Align = alClient
        TabOrder = 0
      end
      object ButPastaPublica: TBitBtn
        AlignWithMargins = True
        Left = 332
        Top = 18
        Width = 30
        Height = 21
        Action = ActionSelPastaPublica
        Align = alRight
        TabOrder = 1
      end
    end
    object GroupBox2: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 597
      Width = 367
      Height = 80
      Align = alTop
      Caption = 'Verifica'#231#245'es avan'#231'adas'
      TabOrder = 7
      object CheckBoxValidaFase: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 18
        Width = 354
        Height = 18
        Margins.Left = 6
        Margins.Bottom = 0
        Align = alTop
        Caption = 'Verificar consist'#234'ncia das fases de cargas e trechos'
        Checked = True
        State = cbChecked
        TabOrder = 0
        WordWrap = True
      end
      object CheckBoxCorrigeFase: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 39
        Width = 354
        Height = 18
        Margins.Left = 6
        Margins.Bottom = 0
        Align = alTop
        Caption = 'Corrigir fases de trechos e cargas'
        Checked = True
        State = cbChecked
        TabOrder = 1
        WordWrap = True
      end
      object CheckBoxCorrigePerdaFerro: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 60
        Width = 354
        Height = 18
        Margins.Left = 6
        Margins.Bottom = 0
        Align = alTop
        Caption = 'Corrigir informa'#231#227'o de perda no ferro dos transformadores'
        Checked = True
        State = cbChecked
        TabOrder = 2
        WordWrap = True
      end
    end
    object GBoxMalha: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 490
      Width = 367
      Height = 101
      Align = alTop
      Caption = 'Desfazer malhas em redes prim'#225'rias'
      TabOrder = 8
      object CheckBoxAbrirChaveExt: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 18
        Width = 354
        Height = 18
        Margins.Left = 6
        Margins.Bottom = 0
        Align = alTop
        Caption = 'Abrir chaves envolvendo redes prim'#225'rias'
        TabOrder = 0
        WordWrap = True
      end
      object CheckBoxAbrirChaveInt: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 60
        Width = 354
        Height = 18
        Margins.Left = 6
        Margins.Bottom = 0
        Align = alTop
        Caption = 'Abrir chaves em malhas internas'
        TabOrder = 1
        WordWrap = True
      end
      object CheckBoxDeslocarTrechoInt: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 81
        Width = 354
        Height = 18
        Margins.Left = 6
        Margins.Bottom = 0
        Align = alTop
        Caption = 'Deslocar trechos em malhas internas'
        TabOrder = 2
        WordWrap = True
      end
      object CheckBoxDeslocarTrechoExt: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 39
        Width = 354
        Height = 18
        Margins.Left = 6
        Margins.Bottom = 0
        Align = alTop
        Caption = 'Deslocar trechos envolvendo redes prim'#225'rias'
        TabOrder = 3
        WordWrap = True
      end
    end
    object GroupBox1: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 343
      Width = 367
      Height = 66
      Align = alTop
      Caption = 'Relat'#243'rios de Equipamentos'
      TabOrder = 9
      object CheckBoxSalvarRede: TCheckBox
        AlignWithMargins = True
        Left = 8
        Top = 18
        Width = 354
        Height = 17
        Margins.Left = 6
        Align = alTop
        Caption = 'N'#227'o salvar a rede no mdb (diminuir tempo)'
        TabOrder = 0
      end
      object Panel1: TPanel
        Left = 2
        Top = 38
        Width = 363
        Height = 28
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 1
        object Label5: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 57
          Height = 13
          Align = alLeft
          Caption = '  Salvar em:'
          Layout = tlCenter
        end
        object BitBtnSelDirRelEqpto: TBitBtn
          AlignWithMargins = True
          Left = 330
          Top = 3
          Width = 30
          Height = 22
          Action = ActionSelPastaRelatorioEqpto
          Align = alRight
          TabOrder = 0
        end
        object EditDirRelEqpto: TEdit
          AlignWithMargins = True
          Left = 66
          Top = 3
          Width = 258
          Height = 22
          Align = alClient
          TabOrder = 1
          ExplicitHeight = 21
        end
      end
    end
  end
  object PanelSelecao: TPanel
    Left = 375
    Top = 30
    Width = 607
    Height = 836
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    object Splitter2: TSplitter
      Left = 468
      Top = 153
      Height = 683
      ExplicitLeft = 133
      ExplicitTop = 0
      ExplicitHeight = 649
    end
    object Splitter3: TSplitter
      Left = 471
      Top = 153
      Height = 683
      ExplicitLeft = 833
      ExplicitTop = 30
      ExplicitHeight = 649
    end
    object Splitter4: TSplitter
      Left = 0
      Top = 149
      Width = 607
      Height = 4
      Cursor = crVSplit
      Align = alTop
      ExplicitLeft = 321
      ExplicitTop = 105
      ExplicitWidth = 544
    end
    object GBoxPrimario: TGroupBox
      AlignWithMargins = True
      Left = 477
      Top = 156
      Width = 127
      Height = 677
      Align = alClient
      Caption = 'Prim'#225'rios'
      TabOrder = 0
      object CLBoxPrimario: TCheckListBox
        Left = 2
        Top = 41
        Width = 123
        Height = 634
        OnClickCheck = CLBoxPrimarioClickCheck
        Align = alClient
        BorderStyle = bsNone
        Columns = 1
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 12
        ParentFont = False
        Sorted = True
        Style = lbOwnerDrawFixed
        TabOrder = 0
        OnClick = CLBoxPrimarioClickCheck
      end
      object ToolBar1: TToolBar
        Left = 2
        Top = 15
        Width = 123
        Height = 26
        ButtonHeight = 19
        Caption = 'ToolBarPrimario'
        EdgeBorders = [ebTop, ebBottom]
        TabOrder = 1
        object ButPrimarioSelAll: TBitBtn
          Left = 0
          Top = 0
          Width = 17
          Height = 19
          Action = ActionPrimarioSelAll
          Align = alTop
          Caption = #254
          Font.Charset = SYMBOL_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Wingdings'
          Font.Style = [fsBold]
          NumGlyphs = 2
          ParentFont = False
          TabOrder = 0
        end
        object ButPrimarioSelNone: TBitBtn
          Left = 17
          Top = 0
          Width = 18
          Height = 19
          Action = ActionPrimarioSelNone
          Align = alTop
          Caption = 'o'
          Font.Charset = SYMBOL_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
      end
    end
    object GBoxSubestacaoAereo: TGroupBox
      AlignWithMargins = True
      Left = 3
      Top = 156
      Width = 462
      Height = 677
      Align = alLeft
      Caption = 'Subesta'#231#245'es'
      TabOrder = 1
      object CLBoxSubestacao: TCheckListBox
        Left = 2
        Top = 42
        Width = 458
        Height = 633
        OnClickCheck = CLBoxSubestacaoClickCheck
        Align = alClient
        BorderStyle = bsNone
        Columns = 2
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        Font.Quality = fqClearType
        ItemHeight = 12
        ParentFont = False
        Sorted = True
        Style = lbOwnerDrawFixed
        TabOrder = 0
        OnClick = CLBoxSubestacaoClickCheck
      end
      object ToolBar2: TToolBar
        Left = 2
        Top = 15
        Width = 458
        Height = 27
        ButtonHeight = 19
        Caption = 'ToolBarRede'
        EdgeBorders = [ebTop, ebBottom]
        TabOrder = 1
        object ButSubestacaoSelAll: TBitBtn
          Left = 0
          Top = 0
          Width = 17
          Height = 19
          Action = ActionSubestacaoSelAll
          Caption = #254
          Font.Charset = SYMBOL_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Wingdings'
          Font.Style = [fsBold]
          NumGlyphs = 2
          ParentFont = False
          TabOrder = 0
        end
        object ButSubestacaoSelNone: TBitBtn
          Left = 17
          Top = 0
          Width = 17
          Height = 19
          Action = ActionSubestacaoSelNone
          Caption = 'o'
          Font.Charset = SYMBOL_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Wingdings'
          Font.Style = [fsBold]
          NumGlyphs = 2
          ParentFont = False
          TabOrder = 1
        end
      end
    end
    object PanelAgrupamentos: TPanel
      Left = 0
      Top = 0
      Width = 607
      Height = 149
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 2
      object Splitter1: TSplitter
        Left = 300
        Top = 0
        Width = 4
        Height = 149
        ExplicitLeft = 305
        ExplicitTop = 3
      end
      object GBoxLocalidade: TGroupBox
        AlignWithMargins = True
        Left = 307
        Top = 3
        Width = 297
        Height = 143
        Align = alClient
        Caption = 'Localidade'
        TabOrder = 0
        object ToolBar3: TToolBar
          Left = 2
          Top = 15
          Width = 293
          Height = 26
          ButtonHeight = 19
          Caption = 'ToolBarRede'
          EdgeBorders = [ebTop, ebBottom]
          TabOrder = 0
          object ButLocalidadeSelAll: TBitBtn
            Left = 0
            Top = 0
            Width = 17
            Height = 19
            Action = ActionLocalidadeSelAll
            Align = alTop
            Caption = #254
            Font.Charset = SYMBOL_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Wingdings'
            Font.Style = [fsBold]
            NumGlyphs = 2
            ParentFont = False
            TabOrder = 0
          end
          object ButLocalidadeSelNone: TBitBtn
            Left = 17
            Top = 0
            Width = 18
            Height = 19
            Action = ActionLocalidadeSelNone
            Align = alTop
            Caption = 'o'
            Font.Charset = SYMBOL_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Wingdings'
            Font.Style = [fsBold]
            NumGlyphs = 2
            ParentFont = False
            TabOrder = 1
          end
        end
        object CLBoxLocalidade: TCheckListBox
          Left = 2
          Top = 41
          Width = 293
          Height = 100
          OnClickCheck = CLBoxLocalidadeClickCheck
          Align = alClient
          BorderStyle = bsNone
          Columns = 3
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          Font.Quality = fqClearType
          ItemHeight = 12
          ParentFont = False
          Sorted = True
          Style = lbOwnerDrawFixed
          TabOrder = 1
          OnClick = CLBoxLocalidadeClickCheck
        end
      end
      object GBoxRegional: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 294
        Height = 143
        Align = alLeft
        Caption = 'Regional'
        TabOrder = 1
        object CLBoxRegional: TCheckListBox
          Left = 2
          Top = 42
          Width = 290
          Height = 99
          OnClickCheck = CLBoxRegionalClickCheck
          Align = alClient
          BorderStyle = bsNone
          Columns = 1
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          Font.Quality = fqClearType
          ItemHeight = 12
          ParentFont = False
          Sorted = True
          Style = lbOwnerDrawFixed
          TabOrder = 0
          OnClick = CLBoxRegionalClickCheck
        end
        object ToolBarRede: TToolBar
          Left = 2
          Top = 15
          Width = 290
          Height = 27
          ButtonHeight = 19
          Caption = 'ToolBarRede'
          EdgeBorders = [ebTop, ebBottom]
          TabOrder = 1
          object ButRegionalSelAll: TBitBtn
            Left = 0
            Top = 0
            Width = 17
            Height = 19
            Action = ActionRegionalSelAll
            Caption = #254
            Font.Charset = SYMBOL_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Wingdings'
            Font.Style = [fsBold]
            NumGlyphs = 2
            ParentFont = False
            TabOrder = 0
          end
          object ButRegionalSelNone: TBitBtn
            Left = 17
            Top = 0
            Width = 17
            Height = 19
            Action = ActionRegionalSelNone
            Caption = 'o'
            Font.Charset = SYMBOL_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Wingdings'
            Font.Style = [fsBold]
            NumGlyphs = 2
            ParentFont = False
            TabOrder = 1
          end
        end
      end
    end
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 360
    Top = 88
    object ActionAbrirEgrid: TAction
      Caption = 'Abrir arquivo .egrid'
      ImageIndex = 3
      OnExecute = ActionAbrirEgridExecute
    end
    object ActionRedeClear: TAction
      Caption = 'Reiniciar Rede'
      OnExecute = ActionRedeClearExecute
    end
    object ActionIniciarImportacao: TAction
      Caption = 'Iniciar Importa'#231#227'o'
      Enabled = False
      OnExecute = ActionIniciarImportacaoExecute
    end
    object ActionSelPastaPublica: TAction
      ImageIndex = 3
      OnExecute = ActionSelPastaPublicaExecute
    end
    object ActionSubestacaoSelAll: TAction
      Caption = #254
      OnExecute = ActionSubestacaoSelAllExecute
    end
    object ActionSubestacaoSelNone: TAction
      Caption = 'o'
      OnExecute = ActionSubestacaoSelNoneExecute
    end
    object ActionPrimarioSelAll: TAction
      Caption = #254
      OnExecute = ActionPrimarioSelAllExecute
    end
    object ActionPrimarioSelNone: TAction
      Caption = 'o'
      OnExecute = ActionPrimarioSelNoneExecute
    end
    object ActionHelp: TAction
      Caption = 'Ajuda'
      ImageIndex = 0
      OnExecute = ActionHelpExecute
    end
    object ActionFecha: TAction
      Caption = 'Fechar'
      ImageIndex = 1
      OnExecute = ActionFechaExecute
    end
    object ActionRegionalSelAll: TAction
      Caption = #254
      OnExecute = ActionRegionalSelAllExecute
    end
    object ActionRegionalSelNone: TAction
      Caption = 'o'
      OnExecute = ActionRegionalSelNoneExecute
    end
    object ActionLocalidadeSelAll: TAction
      Caption = #254
      OnExecute = ActionLocalidadeSelAllExecute
    end
    object ActionLocalidadeSelNone: TAction
      Caption = 'o'
      OnExecute = ActionLocalidadeSelNoneExecute
    end
    object ActionRelValidacao: TAction
      Caption = 'DEBUG: Relat'#243'rio Valida'#231#227'o'
      OnExecute = ActionRelValidacaoExecute
    end
    object ActionSelPastaRelatorioEqpto: TAction
      ImageIndex = 3
      OnExecute = ActionSelPastaRelatorioEqptoExecute
    end
    object ActionAborta: TAction
      Caption = 'Interromper importa'#231#227'o'
      Enabled = False
      ImageIndex = 4
      OnExecute = ActionAbortaExecute
    end
  end
  object ImageList: TImageList
    Left = 368
    Top = 248
    Bitmap = {
      494C010105003401C00210001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000002000000001002000000000000020
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000000000000000808080000000000080808000000000000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF00000000000000
      FF000000FF000000FF0080808000000000008080800000000000000000000000
      00000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      0000000000000000000080808000000000008080800000000000000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000800000000000000080000000FF00000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF00000000000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000000080808000000000008080800000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
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
      000000000000FFFFFF00808080000000FF0080808000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF000000FF000000FF000000FF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF00808080000000FF0080808000FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      FF000000FF00C0C0C000FFFFFF00FFFFFF00FFFFFF00C0C0C0000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      84000000840000008400000000000000000000000000000000000000FF000000
      FF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      84000000840000000000000000000000000000000000000000000000FF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFF
      FF000000FF0000000000000000000000000000000000000000000000000000FF
      FF00C0C0C00000FFFF00C0C0C00000FFFF00C0C0C00000FFFF00C0C0C00000FF
      FF0000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF000000FF008080800000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      840000000000000000000000000000000000000000000000FF00C0C0C000FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C0C0C0000000FF0000000000000000000000000000000000FFFFFF000000
      000000FFFF00C0C0C00000FFFF00C0C0C00000FFFF00C0C0C00000FFFF00C0C0
      C00000FFFF0000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      000000000000000000000000000000000000000000000000FF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000FF000000000000000000000000000000000000FFFF00FFFF
      FF000000000000FFFF00C0C0C00000FFFF00C0C0C00000FFFF00C0C0C00000FF
      FF00C0C0C00000FFFF000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      000000000000000000000000000000000000000000000000FF00FFFFFF00FF00
      0000FFFFFF00FFFFFF00FFFFFF000000FF000000000000000000000000000000
      0000FFFFFF000000FF0000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF00808080008080800000FFFF00FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      000000000000000000000000000000000000000000000000FF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000FF000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      000000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF000000FF000000FF00FFFFFF0000FFFF00808080000000FF000000FF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      840000000000000000000000000000000000000000000000FF00C0C0C000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C0C0C0000000FF0000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF000000FF000000FF0080808000FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      84000000840000000000000000000000000000000000000000000000FF00FFFF
      FF00FF000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFF
      FF000000FF00000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF000000FF000000FF000000FF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      84000000840000008400000000000000000000000000000000000000FF000000
      FF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF0000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF000000FF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      FF000000FF00C0C0C000FFFFFF00FF000000FFFFFF00C0C0C0000000FF000000
      FF00000000000000000000000000000000000000000000000000808080000000
      0000000000000000000000000000808080000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF00FFFF000000000000F83F000000000000
      E00F000000000000CC470000000000008463000000000000A073000000000000
      31F900000000000038F90000000000003C790000000000003C39000000000000
      3C190000000000009C0B0000000000008C43000000000000C467000000000000
      E00F000000000000F83F000000000000FFFFFFFFFFFFFFFFF83FFFFFF83FFFFF
      E00FFFFFE00FFFFFC007E7F3C007FFFF8003E3E38003C00F8003F1C780038007
      0001F88F000180030001FC1F000180010001FE3F000180010001FC1F0001800F
      0001F88F0001800F8003F1C78003801F8003E3E38003C0FFC007E7F3C007C0FF
      E00FFFFFE00FFFFFF83FFFFFF83FFFFF00000000000000000000000000000000
      000000000000}
  end
  object dlgOpen: TOpenDialog
    Left = 368
    Top = 192
  end
end
