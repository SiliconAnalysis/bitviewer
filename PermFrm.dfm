object PermForm: TPermForm
  Left = 229
  Top = 110
  Width = 183
  Height = 177
  Caption = 'Bit Permutations'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object EveryFourthButton: TButton
    Left = 24
    Top = 48
    Width = 121
    Height = 25
    Caption = 'Every fourth column'
    TabOrder = 0
    OnClick = EveryFourthButtonClick
  end
  object OneBitPerColButton: TButton
    Left = 24
    Top = 16
    Width = 121
    Height = 25
    Caption = 'One bit per column'
    TabOrder = 1
    OnClick = OneBitPerColButtonClick
  end
  object Pattern1122Button: TButton
    Left = 24
    Top = 80
    Width = 121
    Height = 25
    Caption = '11221212 pattern'
    TabOrder = 2
    OnClick = Pattern1122ButtonClick
  end
  object Every4ReversedButton: TButton
    Left = 8
    Top = 112
    Width = 153
    Height = 25
    Caption = 'Every fourth column reversed'
    TabOrder = 3
    OnClick = Every4ReversedButtonClick
  end
end
