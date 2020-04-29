object MainForm: TMainForm
  Left = 191
  Top = 119
  Width = 1260
  Height = 368
  Caption = 'Bitviewer: The ROM bit manipulation tool'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Visible = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    1244
    310)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 10
    Top = 5
    Width = 45
    Height = 13
    Caption = 'Bit Size X'
  end
  object Label2: TLabel
    Left = 10
    Top = 29
    Width = 45
    Height = 13
    Caption = 'Bit Size Y'
  end
  object Label5: TLabel
    Left = 240
    Top = 5
    Width = 63
    Height = 13
    Caption = 'Bits per Row:'
  end
  object Label6: TLabel
    Left = 227
    Top = 29
    Width = 76
    Height = 13
    Caption = 'Cols per Output:'
  end
  object Label7: TLabel
    Left = 138
    Top = 5
    Width = 35
    Height = 13
    Caption = 'Bit Gap'
  end
  object Label8: TLabel
    Left = 112
    Top = 29
    Width = 61
    Height = 13
    Caption = 'Column Gap:'
  end
  object Label9: TLabel
    Left = 1001
    Top = 8
    Width = 55
    Height = 13
    Caption = 'Bit Number:'
  end
  object Label11: TLabel
    Left = 1192
    Top = 8
    Width = 25
    Height = 13
    Caption = 'Row:'
  end
  object Label12: TLabel
    Left = 1199
    Top = 24
    Width = 18
    Height = 13
    Caption = 'Col:'
  end
  object Label13: TLabel
    Left = 1112
    Top = 8
    Width = 32
    Height = 13
    Caption = 'Group:'
  end
  object BitNumberLabel: TLabel
    Left = 1060
    Top = 8
    Width = 3
    Height = 13
  end
  object RowLabel: TLabel
    Left = 1224
    Top = 8
    Width = 3
    Height = 13
  end
  object ColLabel: TLabel
    Left = 1224
    Top = 24
    Width = 3
    Height = 13
  end
  object GroupLabel: TLabel
    Left = 1148
    Top = 8
    Width = 3
    Height = 13
  end
  object Label14: TLabel
    Left = 1126
    Top = 24
    Width = 18
    Height = 13
    Caption = 'Col:'
  end
  object FullColLabel: TLabel
    Left = 1148
    Top = 24
    Width = 3
    Height = 13
  end
  object ActionRadioGroup: TRadioGroup
    Left = 768
    Top = 0
    Width = 217
    Height = 48
    Columns = 2
    Items.Strings = (
      'Select Bit'
      'Select Row'
      'Select Column'
      'Set Bit')
    TabOrder = 13
  end
  object BitImageEnView: TImageEnView
    Left = 2
    Top = 52
    Width = 1240
    Height = 255
    Cursor = crArrow
    Background = clSilver
    ParentCtl3D = False
    BorderStyle = bsNone
    ImageEnVersion = '3.0.2'
    Anchors = [akLeft, akTop, akRight, akBottom]
    EnableInteractionHints = True
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
    OnMouseDown = BitImageEnViewMouseDown
    OnMouseMove = BitImageEnViewMouseMove
    OnMouseUp = BitImageEnViewMouseUp
  end
  object BitSizeXEdit: TEdit
    Left = 64
    Top = 2
    Width = 41
    Height = 21
    TabOrder = 1
    Text = '4'
    OnKeyPress = BitsPerRowEditKeyPress
  end
  object BitSizeYEdit: TEdit
    Left = 64
    Top = 26
    Width = 41
    Height = 21
    TabOrder = 2
    Text = '4'
    OnKeyPress = BitsPerRowEditKeyPress
  end
  object BitsPerRowEdit: TEdit
    Left = 308
    Top = 2
    Width = 41
    Height = 21
    TabOrder = 5
    Text = '1024'
    OnKeyPress = BitsPerRowEditKeyPress
  end
  object BitsPerOutputEdit: TEdit
    Left = 308
    Top = 26
    Width = 41
    Height = 21
    TabOrder = 6
    Text = '32'
    OnKeyPress = BitsPerRowEditKeyPress
  end
  object ColGapEdit: TEdit
    Left = 178
    Top = 26
    Width = 41
    Height = 21
    TabOrder = 4
    Text = '4'
    OnKeyPress = BitsPerRowEditKeyPress
  end
  object OpenBinFileButton: TButton
    Left = 360
    Top = 2
    Width = 75
    Height = 21
    Caption = 'Open file'
    TabOrder = 7
    OnClick = OpenBinFileButtonClick
  end
  object BitGapEdit: TEdit
    Left = 178
    Top = 2
    Width = 41
    Height = 21
    TabOrder = 3
    Text = '2'
    OnKeyPress = BitsPerRowEditKeyPress
  end
  object SaveBitProjButton: TButton
    Left = 360
    Top = 26
    Width = 75
    Height = 21
    Caption = 'Save project'
    TabOrder = 8
    OnClick = SaveBitProjButtonClick
  end
  object SaveImageButton: TButton
    Left = 440
    Top = 2
    Width = 75
    Height = 21
    Caption = 'Save image'
    TabOrder = 9
    OnClick = SaveImageButtonClick
  end
  object InvertSelButton: TButton
    Left = 680
    Top = 2
    Width = 75
    Height = 21
    Caption = 'Invert sel.'
    TabOrder = 12
    OnClick = InvertSelButtonClick
  end
  object ClearSelButton: TButton
    Left = 600
    Top = 26
    Width = 75
    Height = 21
    Caption = 'Unselect all'
    TabOrder = 11
    OnClick = ClearSelButtonClick
  end
  object SelAllButton: TButton
    Left = 600
    Top = 2
    Width = 75
    Height = 21
    Caption = 'Select all'
    TabOrder = 10
    OnClick = SelAllButtonClick
  end
  object ExportOptsButton: TButton
    Left = 520
    Top = 26
    Width = 75
    Height = 21
    Caption = 'Export options'
    TabOrder = 14
    OnClick = ExportOptsButtonClick
  end
  object SaveBinButton: TButton
    Left = 440
    Top = 26
    Width = 75
    Height = 21
    Caption = 'Save .bin'
    TabOrder = 15
    OnClick = SaveBinButtonClick
  end
  object ByteViewButton: TButton
    Left = 520
    Top = 2
    Width = 75
    Height = 21
    Caption = 'Byte view (hex)'
    TabOrder = 16
    OnClick = Byteviewhex1Click
  end
  object MainMenu1: TMainMenu
    Left = 192
    Top = 48
    object File1: TMenuItem
      Caption = '&File'
      object Openfile1: TMenuItem
        Caption = '&Open file...'
        OnClick = OpenBinFileButtonClick
      end
      object SaveBin1: TMenuItem
        Caption = '&Save project...'
        OnClick = SaveBitProjButtonClick
      end
      object SaveImage1: TMenuItem
        Caption = 'Save &image...'
        OnClick = SaveImageButtonClick
      end
      object Exportbinary1: TMenuItem
        Caption = 'Export &binary...'
        OnClick = ExportOptsButtonClick
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object Edit1: TMenuItem
      Caption = '&Edit'
      object SelectAll1: TMenuItem
        Caption = 'Select &all'
        OnClick = SelAllButtonClick
      end
      object Unselect1: TMenuItem
        Caption = 'U&nselect all'
        OnClick = ClearSelButtonClick
      end
      object Selectpattern1: TMenuItem
        Caption = 'Select &pattern...'
        OnClick = Selectpattern1Click
      end
    end
    object Bits1: TMenuItem
      Caption = '&Bits'
      object Invertselectedbits1: TMenuItem
        Caption = '&Invert selected bits'
        OnClick = InvertSelButtonClick
      end
    end
    object View1: TMenuItem
      Caption = '&View'
      object Byteviewhex1: TMenuItem
        Caption = 'Byte view... (&hex)'
        OnClick = Byteviewhex1Click
      end
      object Exportoptions1: TMenuItem
        Caption = 'Export options...'
        OnClick = ExportOptsButtonClick
      end
    end
  end
  object FileOpenDialog: TOpenDialog
    DefaultExt = '.txt'
    Filter = 
      'Project, text and binary files (*.bit;*.txt, *.bin)|*.bit;*.txt;' +
      '*.bin|Bit project files (*.bit)|*.bit|Binary files (*.bin)|*.bin' +
      '|Text files (*.txt)|*.txt'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 224
    Top = 48
  end
  object BitFileSaveDialog: TSaveDialog
    DefaultExt = '.bin'
    Filter = 'Bit project files (*.bit)|*.bit'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 256
    Top = 48
  end
  object JpgSaveDialog: TSaveDialog
    DefaultExt = '.png'
    Filter = 'PNG images|*.png|TIFF images|*.jpg|JPEG images|*.tif'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 288
    Top = 48
  end
  object BitImageEnIO: TImageEnIO
    AttachedImageEn = BitImageEnView
    Background = clSilver
    PreviewFont.Charset = DEFAULT_CHARSET
    PreviewFont.Color = clWindowText
    PreviewFont.Height = -11
    PreviewFont.Name = 'MS Sans Serif'
    PreviewFont.Style = []
    Left = 64
    Top = 40
  end
  object BinSaveDialog: TSaveDialog
    DefaultExt = '.bin'
    Filter = 'Binary files (*.bin)|*.bin'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 256
    Top = 72
  end
end
