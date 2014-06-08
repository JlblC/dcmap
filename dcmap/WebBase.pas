unit WebBase;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms, 
  Dialogs, sFrameAdapter, Menus;


type

  TBrowserParams = class
  public
    UserAgend : string;
  end;

  TWebBaseFrame = class(TFrame)
    FrameAdapter: TsFrameAdapter;
    LoginMenu: TPopupMenu;
  private
    { Private declarations }
  public
    HomePageURL: string;
    CurrentURL: string;

    procedure SetupBrowser(const Params : TBrowserParams);virtual;abstract;

    procedure Go(URL:string);overload;virtual;abstract;
    procedure Go(URL,Post:string);overload;virtual;abstract;
    procedure Wait(Time:Cardinal=0);virtual;abstract;

    procedure SetUserName(Name:String);virtual;abstract;
    { Public declarations }
  end;

function  CreateBrowser(Owner:TComponent): TWebBaseFrame ;



implementation

uses WebBrowser;


function  CreateBrowser(Owner:TComponent): TWebBaseFrame ;
begin
   CreateBrowser := TWebBrowserFrame.Create(Owner);

end;

{$R *.dfm}

end.
