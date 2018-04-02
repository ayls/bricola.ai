program BriscolaGen;

uses
  Forms,
  uBriskolaMain in 'uBriskolaMain.pas' {BriscolaMain};

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'Briscola Learning case generator';
  Application.CreateForm(TBriscolaMain, BriscolaMain);
  Application.Run;
end.
