unit WebBrowser;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms, 
  Dialogs,WebBase, sFrameAdapter, OleCtrls, SHDocVw_EWB, EwbCore, EmbeddedWB,
  ComCtrls, sStatusBar, ToolWin, sToolBar, StdCtrls, sComboBox, IEAddress,
  sComboBoxes, sGauge, ExtCtrls, sPanel, Menus, sLabel;

type
  TWebBrowserFrame = class(TWebBaseFrame)
    StatusBar: TsStatusBar;
    sToolBar1: TsToolBar;
    GoBtn: TToolButton;
    BackBtn: TToolButton;
    ForwardBtn: TToolButton;
    ReloadBtn: TToolButton;
    UrlBox: TsComboBox;
    ProgressGauge: TsGauge;
    sPanel1: TsPanel;
    WB: TEmbeddedWB;
    LoginBtn: TToolButton;
    UserNameLabel: TsLabel;
    procedure GoBtnClick(Sender: TObject);
    procedure UrlBoxKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure WBBeforeNavigate2(ASender: TObject; const pDisp: IDispatch;
      var URL, Flags, TargetFrameName, PostData, Headers: OleVariant;
      var Cancel: WordBool);
    procedure WBProgressChange(ASender: TObject; Progress,
      ProgressMax: Integer);
    procedure WBStatusTextChange(ASender: TObject; const Text: WideString);
    procedure BackBtnClick(Sender: TObject);
    procedure ForwardBtnClick(Sender: TObject);
    procedure ReloadBtnClick(Sender: TObject);
    procedure WBNavigateComplete2(ASender: TObject; const pDisp: IDispatch;
      var URL: OleVariant);
    procedure WBEvaluateNewWindow(Sender: TCustomEmbeddedWB; pszUrl, pszName,
      pszUrlContext, pszFeatures: PWideChar; fReplace: LongBool; dwFlags,
      dwUserActionTime: Cardinal; var Rezult: HRESULT);
  private
    UserName:string;
  public
    procedure Go(URL:string);overload;override;
    procedure Go(URL,Post:string);overload;override;
    procedure Wait(Time:Cardinal);override;
    procedure SetUserName(Name:String);override;
    procedure SetupBrowser(const Params : TBrowserParams);override;
  end;

implementation

{$R *.dfm}

procedure TWebBrowserFrame.SetupBrowser(const Params : TBrowserParams);
begin
  WB.UserAgent := Params.UserAgend;
end;


procedure TWebBrowserFrame.Go(URL:string);
begin
  WB.Go(URL);
end;

procedure TWebBrowserFrame.Wait(Time:Cardinal);
begin
  WB.Wait(Time);
end;

procedure TWebBrowserFrame.Go(URL,Post:string);
var
  strData: string;
  PostData: OleVariant;
  Headers: OleVariant;
  Empty:OleVariant;
  i: Integer;
begin
  Empty := EmptyParam;
  PostData := VarArrayCreate([0, Length(Post) - 1], varByte);
  for i := 1 to Length(Post) do
    PostData[i-1] := Ord(Post[i]);

  Headers := 'Content-Type: application/x-www-form-urlencoded' + #10#13;

  WB.Navigate(URL, Empty, Empty, PostData, Headers);
end;


procedure TWebBrowserFrame.BackBtnClick(Sender: TObject);
begin
  inherited;
  WB.GoBack;
end;

procedure TWebBrowserFrame.ForwardBtnClick(Sender: TObject);
begin
  inherited;
  WB.GoForward;
end;

procedure TWebBrowserFrame.GoBtnClick(Sender: TObject);
begin
  inherited;
  Go(UrlBox.Text);
end;

procedure TWebBrowserFrame.ReloadBtnClick(Sender: TObject);
begin
  inherited;
  WB.Refresh2;
end;

procedure TWebBrowserFrame.UrlBoxKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  inherited;
  if Key = VK_RETURN then WB.Go(UrlBox.Text);

end;

procedure TWebBrowserFrame.WBBeforeNavigate2(ASender: TObject;
  const pDisp: IDispatch; var URL, Flags, TargetFrameName, PostData,
  Headers: OleVariant; var Cancel: WordBool);
begin
  inherited;
  if TargetFrameName = '' then
  begin
    CurrentURL := WB.LocationURL;
    UrlBox.Text := CurrentURL;
  end;
end;

procedure TWebBrowserFrame.WBEvaluateNewWindow(Sender: TCustomEmbeddedWB;
  pszUrl, pszName, pszUrlContext, pszFeatures: PWideChar; fReplace: LongBool;
  dwFlags, dwUserActionTime: Cardinal; var Rezult: HRESULT);
(*
pszUrl
- A pointer to a buffer that contains the URL of the content that will be displayed in the new window.
pszName
- A pointer to a buffer that contains the name of the new window. This parameter can be NULL.
pszUrlContext
- A pointer to a buffer that contains the URL that has issued the command to open the new window.
pszFeatures
- A pointer to a buffer that contains the feature string for the new window. This value can be NULL.
fReplace
- A boolean value used when the new content specified in pszUrl is loaded into the existing window instead of creating a new one. TRUE if the new document should replace the current document in the history list; FALSE if the new document should be given a new entry.
dwFlags
- A flag or flags from the NWMF enumeration that provide situational information about the call to open the new window. This value can be 0 if no flags are needed.
dwUserActionTime
- The tick count when the last user action occurred. To find out how long ago the action occurred, call GetTickCount and compare the result with the value in this parameter.
*)
begin
  inherited;

    Rezult := S_OK; //Block display of the window;
end;

procedure TWebBrowserFrame.WBNavigateComplete2(ASender: TObject;
  const pDisp: IDispatch; var URL: OleVariant);
begin
  inherited;
    CurrentURL := WB.LocationURL;
    UrlBox.Text := CurrentURL;
end;

procedure TWebBrowserFrame.WBProgressChange(ASender: TObject; Progress,
  ProgressMax: Integer);
begin
  inherited;
  if ProgressMax <> 0 then
  begin
    ProgressGauge.Progress := Progress;
    ProgressGauge.MaxValue := ProgressMax;
    ProgressGauge.Visible := true;
    StatusBar.Panels[0].Width := ProgressGauge.Width + 5;
  end
  else
  begin
    ProgressGauge.Visible := false;
    StatusBar.Panels[0].Width := 0;
  end;

end;

procedure TWebBrowserFrame.WBStatusTextChange(ASender: TObject;
  const Text: WideString);
begin
  inherited;
  StatusBar.Panels[1].Text := Text;
end;

procedure TWebBrowserFrame.SetUserName(Name:String);
begin
  UserName := Name;
  UserNameLabel.Caption := UserName;
end;


end.
