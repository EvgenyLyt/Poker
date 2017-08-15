//---------------------------------------------------------------------------

#ifndef UnRulesH
#define UnRulesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TFormRules : public TForm
{
__published:	// IDE-managed Components
	TImage *imgRules;
private:	// User declarations
public:		// User declarations
	__fastcall TFormRules(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormRules *FormRules;
//---------------------------------------------------------------------------
#endif
