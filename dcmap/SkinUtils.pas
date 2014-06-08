unit SkinUtils;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  sSkinManager, StdCtrls, sSkinProvider, ExtCtrls,sGraphUtils,sConst;

procedure SkinPaintItem(Control:TControl;Rect:TRect;Canvas:TCanvas;SkinSection:string; Manager:TsSkinManager);

implementation

procedure SkinPaintItem(Control:TControl;Rect:TRect;Canvas:TCanvas;SkinSection:string; Manager:TsSkinManager);
var
  BGInfo : sConst.TacBGInfo;
  CacheInfo : sConst.TCacheInfo;
  SkinIndex : integer;
begin
  if SkinSection <> '' then begin
    SkinIndex := Manager.GetSkinIndex(SkinSection);
    if SkinIndex =  -1 then SkinSection := 'BUTTON';

    SkinIndex := Manager.GetSkinIndex(SkinSection);
    if SkinIndex <> -1 then begin

      CacheInfo.Ready := False;

      GetBGInfo(@BGInfo, Control.Parent.Handle); // Receiving background image of Form1
      CacheInfo := BGInfoToCI(@BGInfo);

      // Drawing on PaintBox1.Canvas, but exists overloaded PaintItem procedures for drawing on TBitmap (drawing on TBitmap is faster)
      PaintItem(SkinIndex, SkinSection, CacheInfo, True, 0 {State},
                Rect, Point(Control.Left, Control.Top), Canvas.Handle);
    end;
  end;
end;

end.
