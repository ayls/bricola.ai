object BriscolaMain: TBriscolaMain
  Left = 352
  Top = 215
  Width = 510
  Height = 370
  Caption = 'Briscola Learning case generator'
  Color = clGreen
  Constraints.MaxHeight = 370
  Constraints.MaxWidth = 510
  Constraints.MinHeight = 370
  Constraints.MinWidth = 510
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar: TStatusBar
    Left = 0
    Top = 305
    Width = 502
    Height = 19
    Panels = <
      item
        Width = 125
      end
      item
        Width = 125
      end
      item
        Width = 125
      end
      item
        Width = 125
      end>
    SimplePanel = False
  end
  object gbPlayerOne: TGroupBox
    Left = 0
    Top = 0
    Width = 273
    Height = 145
    Caption = 'Player One'
    TabOrder = 1
    object imPlayerOneCardOne: TImage
      Left = 16
      Top = 16
      Width = 60
      Height = 120
      OnClick = CardClick
    end
    object imPlayerOneCardThree: TImage
      Left = 192
      Top = 16
      Width = 60
      Height = 120
      OnClick = CardClick
    end
    object imPlayerOneCardTwo: TImage
      Left = 104
      Top = 16
      Width = 60
      Height = 120
      OnClick = CardClick
    end
  end
  object gbPlayerTwo: TGroupBox
    Left = 0
    Top = 152
    Width = 273
    Height = 145
    Caption = 'Player Two'
    TabOrder = 2
    object imPlayerTwoCardOne: TImage
      Left = 17
      Top = 17
      Width = 60
      Height = 120
      OnClick = CardClick
    end
    object imPlayerTwoCardThree: TImage
      Left = 193
      Top = 17
      Width = 60
      Height = 120
      OnClick = CardClick
    end
    object imPlayerTwoCardTwo: TImage
      Left = 105
      Top = 17
      Width = 60
      Height = 120
      OnClick = CardClick
    end
  end
  object gbTable: TGroupBox
    Left = 280
    Top = 0
    Width = 217
    Height = 297
    Caption = 'Table'
    TabOrder = 3
    object imPlayerTwoCardDown: TImage
      Left = 17
      Top = 168
      Width = 60
      Height = 120
    end
    object imPlayerOneCardDown: TImage
      Left = 17
      Top = 16
      Width = 60
      Height = 120
    end
    object imBriscola: TImage
      Left = 136
      Top = 96
      Width = 60
      Height = 120
    end
  end
  object MainMenu: TMainMenu
    object Game: TMenuItem
      Caption = '&Game'
      object Newgame: TMenuItem
        Caption = '&New game'
        OnClick = NewgameClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exitchoice: TMenuItem
        Caption = '&Exit'
        OnClick = ExitchoiceClick
      end
    end
  end
  object imlCards: TImageList
    Height = 120
    Width = 60
    Left = 32
  end
end
