program WebTest;

uses
  Forms,
  Main in 'Main.pas' {Form1},
  WebBrowser in '..\WebBrowser.pas' {WebBrowserFrame: TFrame},
  WebBase in '..\WebBase.pas' {WebBaseFrame: TFrame};

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
