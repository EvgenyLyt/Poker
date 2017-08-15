//---------------------------------------------------------------------------

#ifndef UnWarningH
#define UnWarningH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TFormExit : public TForm
{
__published:	// IDE-managed Components
	TImage *imgBackGround;
	TSpeedButton *btnYes;
	TSpeedButton *btnNo;
	TLabel *lblWarning;
	TLabel *lblWord;
	void __fastcall btnYesClick(TObject *Sender);
	void __fastcall btnNoClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormExit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormExit *FormExit;
//---------------------------------------------------------------------------
#endif
