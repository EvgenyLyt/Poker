//---------------------------------------------------------------------------

#ifndef UnCombinationsH
#define UnCombinationsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TFormCombinations : public TForm
{
__published:	// IDE-managed Components
	TImage *imgCombinations;
private:	// User declarations
public:		// User declarations
	__fastcall TFormCombinations(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCombinations *FormCombinations;
//---------------------------------------------------------------------------
#endif
