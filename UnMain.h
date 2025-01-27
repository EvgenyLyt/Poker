//---------------------------------------------------------------------------

#ifndef UnMainH
#define UnMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TImage *imgBackground;
	TSpeedButton *btnStartGame;
	TImage *imgPlayerCard1;
	TImage *imgPlayerCard2;
	TImage *imgPlayerCard3;
	TImage *imgPlayerCard4;
	TImage *imgPlayerCard5;
	TMainMenu *MainMenu;
	TMenuItem *mmGame;
	TMenuItem *NewGame;
	TMenuItem *N;
	TMenuItem *ExitGame;
	TImage *imgDealerCard1;
	TImage *imgDealerCard2;
	TImage *imgDealerCard3;
	TImage *imgDealerCard4;
	TImage *imgDealerCard5;
	TSpeedButton *btnRaise;
	TSpeedButton *btnCheck;
	TSpeedButton *btnFold;
	TLabel *lblDlCash;
	TLabel *lblPlCash;
	TLabel *lblCash;
	TLabel *lblPlComb;
	TLabel *lblDlComb;
	TLabel *lblDialog;
	TActionList *ActionList1;
	TTimer *Time_Open;
	TSpeedButton *btnChange;
	TLabel *lblCh1;
	TLabel *lblCh2;
	TLabel *lblCh3;
	TLabel *lblCh4;
	TLabel *lblCh5;
	TPanel *PRaise;
	TSpeedButton *btnR5;
	TSpeedButton *btnR10;
	TSpeedButton *btnR15;
	TSpeedButton *btnR20;
	TSpeedButton *btnR25;
	TSpeedButton *btnBack;
	TMenuItem *Help;
	TMenuItem *AllCombinations;
	TMenuItem *GameRules;
	void __fastcall btnStartGameClick(TObject *Sender);
	void __fastcall ExitGameClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall NewGameClick(TObject *Sender);
	void __fastcall btnFoldClick(TObject *Sender);
	void __fastcall btnRaiseClick(TObject *Sender);
	void __fastcall btnChangeClick(TObject *Sender);
	void __fastcall imgPlayerCard1Click(TObject *Sender);
	void __fastcall imgPlayerCard2Click(TObject *Sender);
	void __fastcall imgPlayerCard3Click(TObject *Sender);
	void __fastcall imgPlayerCard4Click(TObject *Sender);
	void __fastcall imgPlayerCard5Click(TObject *Sender);
	void __fastcall btnCheckClick(TObject *Sender);
	void __fastcall Time_OpenTimer(TObject *Sender);
	void __fastcall btnBackClick(TObject *Sender);
	void __fastcall btnR5Click(TObject *Sender);
	void __fastcall btnR10Click(TObject *Sender);
	void __fastcall btnR15Click(TObject *Sender);
	void __fastcall btnR20Click(TObject *Sender);
	void __fastcall btnR25Click(TObject *Sender);
	void __fastcall AllCombinationsClick(TObject *Sender);
	void __fastcall GameRulesClick(TObject *Sender);
private:
public:
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
