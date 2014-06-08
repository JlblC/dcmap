//---------------------------------------------------------------------------

#ifndef Main2H
#define Main2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TMainForm2 : public TForm
{
__published:	// IDE-managed Components
	TsSkinProvider *SkinProvider;
	TMainMenu *MainMenu;
	TMenuItem *ViewMenu;
	TMenuItem *NewTabMenuItem;
	TMenuItem *CreateTabMenu;
	TMenuItem *StandartTabMenuItem;
	TMenuItem *N1;
	TMenuItem *RenameMenuItem;
	TMenuItem *CloseTabMenuItem;
	TMenuItem *N22;
	TMenuItem *SetupTabMenuItem;
	TMenuItem *SaveTemplateMenuItem;
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm2 *MainForm2;
//---------------------------------------------------------------------------
#endif
