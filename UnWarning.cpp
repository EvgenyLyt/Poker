#include "UnWarning.h"
#include "UnMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormExit *FormExit;
//---------------------------------------------------------------------------
__fastcall TFormExit::TFormExit(TComponent* Owner)
	: TForm(Owner)
{
}

void __fastcall TFormExit::btnYesClick(TObject *Sender)
{
   MainForm->NewGameClick(Sender);
   FormExit->Close();
}

void __fastcall TFormExit::btnNoClick(TObject *Sender)
{
   Application->Terminate();
}

