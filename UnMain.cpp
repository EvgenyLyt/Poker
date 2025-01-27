//---------------------------------------------------------------------------
#include <time.h>
#include <iostream.h>

#include "UnMain.h"
#include "UnWarning.h"
#include "UnCombinations.h"
#include "UnRules.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
typedef AnsiString Str;
Str NameCombinations[10] = {"������� �����","����","��� ����","���","�����",
   "����","���-����","����","�����-����","����-����"};
Str Deck[53];
bool IfRaise = False,CardCh;
int Hands[20],PlCash,DlCash,Cash = 0,
AmountGames = 0,MFlush,Bet,
ChCards[5];
struct ListCards
{
   Str suit;
   Str value;
   int priority;
   int number;
};
struct ListCombination
{
	Str comb;
	int priorcomb;
	int signcomb;
};
typedef struct ListCards Cards;
typedef struct ListCombination Combination;
Cards PlCards[5], DlCards[5];
Combination PlComb,DlComb;
TMainForm *MainForm;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{

}

void Retry(int AmCash)
{
	MainForm->btnCheck->Visible = False;
	MainForm->btnRaise->Visible = False;
	MainForm->btnFold->Visible = False;
	MainForm->PRaise->Visible = False;
	MainForm->btnCheck->Enabled=True;
	MainForm->btnRaise->Enabled=True;
	MainForm->btnFold->Enabled=True;
	MainForm->btnStartGame->Visible = True;
	MainForm->lblPlComb->Caption = "";
	MainForm->lblDlComb->Visible = False;
	Cash = AmCash;
	if (AmCash!=0)
		AmountGames++;
	MainForm->lblCash->Caption = IntToStr(Cash);
	Bet = 0;
	IfRaise = False;
	MFlush = -1;
}

bool InHands(int Hands[], int Card)
{
	int index = 0;
	bool result = false;
	do
	{
	   if (Hands[index]==Card)
		  result = true;
	   index++;
	}
	while ((index!=20)&&(result==false));
	return result;
}

Str CardValue(int Card)
{
   Str Value;
   if ((Card%13 > 0) && (Card%13<10))
	  Value = IntToStr((Card%13)+1);
   else
	  switch (Card%13) {
		case 0:Value = "Ace";break;
		case 10:Value = "Jack";break;
		case 11:Value =  "Queen";break;
		case 12:Value = "King";break;
	   }
   return Value;
}

void RecordCards(Cards Partner[], int index, int Card)
{
   if (Card%13==0)
	  Partner[index].priority =14;
   else
	  Partner[index].priority =(Card%13)+1;
   Partner[index].value = CardValue(Card);
   Partner[index].number = Card;
   switch (Card/13)
   {
	  case 0: Partner[index].suit = "Clubs"; break;
	  case 1: Partner[index].suit = "Diamonds"; break;
	  case 2: Partner[index].suit = "Hearts"; break;
	  case 3: Partner[index].suit = "Spades"; break;
   }
}

void SortCards(Cards Partner[])
{
   Cards OneCard;
   for (int i = 0; i < 5; i++)
	   for (int index = 0; index < 4; index++)
		  if (Partner[index].priority>Partner[index+1].priority)
		  {
			 OneCard = Partner[index];
			 Partner[index] = Partner[index+1];
			 Partner[index+1] = OneCard;
		  }
}

bool RoyalFlush(Cards Partner[])
{
	bool found = true;
	int index = 0;
	while ((found==true)&&(index<4))
	{
		if (((Partner[index].suit)!=(Partner[index+1].suit))||((Partner[index+1].priority-Partner[index].priority)!=1))
		   found = false;
		index++;
	}
	if (found==true)
	   if ((Partner[0].suit==Partner[4].suit)&&(Partner[4].priority-Partner[3].priority==1)&&(Partner[4].priority==14))
		  found = true;
	   else
		  found = false;
	return found;
}

bool StraightFlush(Cards Partner[])
{
	bool found = true;
	int index = 0;
	while ((found==true)&&(index<3))
	{
		if (((Partner[index].suit)!=(Partner[index+1].suit))||((Partner[index+1].priority-Partner[index].priority)!=1))
		   found = false;
		index++;
	}
	if (found==true)
		if ((Partner[0].suit==Partner[4].suit)&&(((Partner[4].priority-Partner[3].priority)==1)||(Partner[4].priority-Partner[3].priority==9)))
			found = true;
		else
			found = false;
	return found;
}

bool Quads(Cards Partner[])
{
	bool found = true;
	int index,count;
	if (Partner[0].priority!=Partner[1].priority)
	   index = 1;
	else
	   index = 0;
	count=3+index;
	while ((found==true)&&(index<count))
	{
		if ((Partner[index].priority)!=(Partner[index+1].priority))
		   found = false;
		index++;
	}
	return found;
}

bool FullHouse(Cards Partner[])
{
	bool found = true;
	int index = 0,count;
	while ((found==true)&&(index<2))
	{
		if ((index==1)&&((Partner[index].priority)!=(Partner[index+1].priority)))
		   count = 2;
		else
		   if ((index==1)&&((Partner[index].priority)==(Partner[index+1].priority)))
			  count = 3;
		   else
			  if ((Partner[index].priority)!=(Partner[index+1].priority))
				 found = false;
		index++;
	}
	index = count;
	while ((found==true)&&(index<4))
	{
	   if ((Partner[index].priority)!=(Partner[index+1].priority))
		  found = false;
		index++;
	}
	return found;
}

bool Flush(Cards Partner[])
{
	bool found = true;
	int index = 0;
	while ((found==true)&&(index<4))
	{
		if ((Partner[index].suit)!=(Partner[index+1].suit))
		   found = false;
		index++;
	}
	return found;
}

bool Straight(Cards Partner[])
{
	bool found = true;
	int index = 0;
	while ((found==true)&&(index<3))
	{
		if ((Partner[index+1].priority-Partner[index].priority)!=1)
		   found = false;
		index++;
	}
	if (found==true)
		if (((Partner[4].priority-Partner[3].priority)!=1)&&(Partner[4].priority-Partner[3].priority!=9))
			found = false;
	return found;
}

bool SET(Cards Partner[])
{
	bool found = true;
	int index, count;
	if (Partner[0].priority==Partner[1].priority)
	   index = 1;
	else
	   if (Partner[1].priority==Partner[2].priority)
		  index = 2;
	   else
		  if (Partner[2].priority==Partner[3].priority)
			 index = 3;
		  else
			 found = false;
	count = 1+index;
	while ((found==true)&&(index<count))
	{
		if (Partner[index].priority!=Partner[index+1].priority)
		   found = false;
		index++;
	}
	return found;
}

bool TwoPairs(Cards Partner[])
{
	bool found = true;
	int index, count;
	if (Partner[0].priority==Partner[1].priority)
	{
		if (((Partner[2].priority!=Partner[3].priority)&&(Partner[2].priority!=Partner[4].priority))
			  &&(Partner[3].priority!=Partner[4].priority))
			  found = false;
	}
	else
	{
	   if (Partner[1].priority==Partner[2].priority)
	   {
		  if (Partner[3].priority!=Partner[4].priority)
			 found = false;
	   }
	   else
		  found = false;
	}
	return found;
}

bool OnePair(Cards Partner[])
{
	bool found = false;
	int count;
	for (int i = 0; i < 4; i++)
		for (int index = i+1; index<5; index++)
		   if (Partner[i].priority==Partner[index].priority)
			  found = true;
	return found;
}

bool Kicker(Cards Partner[])
{
	bool found = true;
	return found;
}

void Search(Cards Partner[], Combination *PartnerComb)
{
   bool found = false;
   int index = 10, count;
   typedef bool (*search[10])(Cards Partner[]);
   search combcards ={&Kicker,&OnePair,&TwoPairs,&SET,&Straight,&Flush,&FullHouse,&Quads,&StraightFlush,&RoyalFlush};
   while (found==false)
   {
	   found = combcards[index-1](Partner);
	   index--;
   }
   switch(index+1)
   {
	  case 10:(*PartnerComb).signcomb=10;break;
	  case 9:if((Partner[4].priority-Partner[3].priority)==9)
				(*PartnerComb).signcomb = Partner[3].priority;
			 else
				(*PartnerComb).signcomb = Partner[4].priority;
			 break;
	  case 8:(*PartnerComb).signcomb=Partner[2].priority;break;
	  case 7:(*PartnerComb).signcomb=Partner[2].priority*100+Partner[1].priority;break;
	  case 6:(*PartnerComb).signcomb=6;break;
	  case 5:if((Partner[4].priority-Partner[3].priority)==9)
				(*PartnerComb).signcomb = Partner[3].priority;
			 else
				(*PartnerComb).signcomb = Partner[4].priority;
			 break;
	  case 4:(*PartnerComb).signcomb=Partner[2].priority;break;
	  case 3:if (Partner[1].priority>Partner[3].priority)
				 (*PartnerComb).signcomb = Partner[1].priority*100+Partner[3].priority;
			  else
				 (*PartnerComb).signcomb = Partner[3].priority*100+Partner[1].priority;
			  break;
	  case 2:
	  {
		 for (int i = 0; i < 4; i++)
			for (int j = i+1; j<5; j++)
			   if (Partner[i].priority==Partner[j].priority)
				  count = i;
		 (*PartnerComb).signcomb = Partner[count].priority;
	  }
	  break;
	  case 1:(*PartnerComb).signcomb=Partner[4].priority;break;
   }
   (*PartnerComb).comb=NameCombinations[index];
   (*PartnerComb).priorcomb = index+1;
}

int MaybeFlush()
{
	int index, result;
	int Suit[4] = {0,0,0,0};
	Str OneSuit;
	for (index = 0; index <5 ; index++)
	   switch (DlCards[index].number/13) {
		  case 0: Suit[0]++; break;
		  case 1: Suit[1]++; break;
		  case 2: Suit[2]++; break;
		  case 3: Suit[3]++; break;
	   }
	index = 0;
	while ((Suit[index]!=4)&&(index<4))
		index++;
	if (index<4) {
	   switch (index) {
	   case 0:OneSuit = "Clubs";break;
	   case 1:OneSuit = "Diamonds";break;
	   case 2:OneSuit = "Hearts";break;
	   case 3:OneSuit = "Spades";break;
	   }
	   for (index = 0; index < 5; index++)
		  if (DlCards[index].suit!=OneSuit)
			 result = index;
	}
	else
	   result = -1;
	return result;
}

void CardsOnTable()
{  TImage *PlayerCard[5] ={MainForm->imgPlayerCard1, MainForm->imgPlayerCard2,
   MainForm->imgPlayerCard3, MainForm->imgPlayerCard4, MainForm->imgPlayerCard5};
   TImage *DealerCard[5] = {MainForm->imgDealerCard1, MainForm->imgDealerCard2,
   MainForm->imgDealerCard3, MainForm->imgDealerCard4, MainForm->imgDealerCard5};
   int Card = 0, index;
   for (index = 0; index < 10; index++) {
	  srand(time(0));
	  Card = random(52);
	  while (InHands(Hands,Card))
		 Card = random(52);
	  Hands[index] = Card;
	  if (index<5)
		 RecordCards(PlCards,index,Card);
	  else
		 RecordCards(DlCards,index-5,Card);
   }
   SortCards(PlCards);
   SortCards(DlCards);
   for (index = 0; index < 5; index++) {
	  PlayerCard[index]->Picture->LoadFromFile(Deck[PlCards[index].number]);
	  DealerCard[index]->Picture->LoadFromFile(Deck[52]);
   }
   Search(PlCards,&PlComb);
   Search(DlCards,&DlComb);
   MFlush = -1;
   if (DlComb.priorcomb<3)
	  MFlush = MaybeFlush();
   MainForm->lblPlComb->Caption = PlComb.comb;
}

void ChNames(Str name1, Str name2)
{
   MainForm->btnCheck->Caption = name1;
   MainForm->btnRaise->Caption = name2;
}

void PlNoth(int coef, Str Word)
{
   Bet = 5*coef;
   DlCash-=Bet;
   Cash+=Bet;
   MainForm->lblDialog->Caption=Word+"������ "+IntToStr(Bet)+" ������!";
   MainForm->lblCash->Caption=IntToStr(Cash);
   MainForm->lblDlCash->Caption=IntToStr(DlCash);
   ChNames("Check","Raise");
}

void DlRaise(int coeff, Str Word)
{
   int NewBet;
   NewBet = 5*coeff;
   Cash+=(Bet+NewBet);
   DlCash-=(Bet+NewBet);
   MainForm->lblDialog->Caption=Word+"������� �� "+IntToStr(NewBet)+" ������!";
   MainForm->lblCash->Caption=IntToStr(Cash);
   MainForm->lblDlCash->Caption=IntToStr(DlCash);
   Bet = NewBet;
   ChNames("Check","Raise");
}

void WIN(int *PartnerCash, TLabel *Label, TLabel *LabelCash, Str word,int AllCash)
{
   Label->Caption = word+IntToStr(AllCash)+" ������.";
   (*PartnerCash)+=AllCash;
   LabelCash->Caption = IntToStr(*PartnerCash);
}

void NoMoney()
{
	if (PlCash<=0) {
	   FormExit->lblWarning->Caption="�� �������� ��� ������...";
	   FormExit->ShowModal();
	}
	else
	   if (DlCash<=0) {
		  FormExit->lblWarning->Caption="� �������� ��� ������...";
		  FormExit->ShowModal();
	   }
}

void EndGame()
{
	MainForm->lblDlComb->Visible = True;
	if (PlComb.priorcomb>DlComb.priorcomb)
	   WIN(&PlCash, MainForm->lblDialog, MainForm->lblPlCash, "��� ����� ���� �����, �������. ��� ��������. ������� ���� - ", Cash);
	else
	   if (PlComb.priorcomb<DlComb.priorcomb)
		  WIN(&DlCash,MainForm->lblDialog,MainForm->lblDlCash,"��� ����� ����� �����, ��� �� ������. � ���� �� ���? ������� ���� - ",Cash);
	   else
		   if (PlComb.signcomb>DlComb.signcomb)
			  WIN(&PlCash,MainForm->lblDialog,MainForm->lblPlCash,"��� ����� ���� �����, ���. ������ ������� �� �������. ���� ",Cash);
		   else
			  if (PlComb.signcomb<DlComb.signcomb)
				 WIN(&DlCash,MainForm->lblDialog,MainForm->lblDlCash,"��� ����� ����� �����. �� ��� ������. ��� ����� ",Cash);
			  else
			  {
				 WIN(&PlCash,MainForm->lblDialog,MainForm->lblPlCash,"���� ����� ���������. �������� ���� �������. ������� �� ",(Cash/2));
				 WIN(&DlCash,MainForm->lblDialog,MainForm->lblDlCash,"���� ����� ���������. �������� ���� �������. ������� �� " ,(Cash/2));
			  }
	Cash = 0;
	MainForm->lblCash->Caption=IntToStr(Cash);
	MainForm->btnStartGame->Visible = True;
	NoMoney();
}

void DlCall()
{
   DlCash-=Bet;
   Cash +=Bet;
   MainForm->lblDialog->Caption="������. �������� �����...";
   MainForm->lblCash->Caption=IntToStr(Cash);
   MainForm->lblDlCash->Caption=IntToStr(DlCash);
   MainForm->btnCheck->Visible = False;
   MainForm->btnRaise->Visible = False;
   MainForm->btnFold->Visible = False;
   MainForm->Time_Open->Tag = 0;
   MainForm->Time_Open->Enabled=True;
}



void DlChange()
{
   MainForm->lblDialog->Caption="����� ����� �������� �����. ������...";
   DlCash-=Bet;
   Cash +=Bet;
   MainForm->lblCash->Caption=IntToStr(Cash);
   MainForm->lblDlCash->Caption=IntToStr(DlCash);
   MainForm->btnCheck->Visible = False;
   MainForm->btnRaise->Visible = False;
   MainForm->btnFold->Visible = False;
   MainForm->btnChange->Visible = True;
   IfRaise = True;
   CardCh = False;
}

void GameTurn(int bank,int coeff,Str Word)
{
	if (Cash>bank)
	{
	   if (IfRaise)
		  DlCall();
	   else
		  DlChange();
	}
	else
	   DlRaise(coeff,Word);
}

void PartnerTurn(int choise)
{
   int Comb = DlComb.priorcomb, prob; bool Noth = False;
   Str Words[] = {"������ �������� ����? ","�� �����, ���������.. ",
   "������-�� � � �����. ","��� ��� �����, �������? ","����� �������? ",
   "�������, ���� ������? "};
   srand(time(0));
   prob=random(6);
   switch (choise) {
	  case 0:
	  {
		 switch (Comb)
		 {
			case 1:
			   if (MFlush>=0)
				  PlNoth(1,Words[prob]);
			   else
			   {
				  MainForm->lblDialog->Caption="�������... � � ����. �������� ������?";
				  Retry(Cash);
			   }
			break;
			case 2: PlNoth(random(2)+1,Words[prob]); break;
			case 3: PlNoth(random(3)+1,Words[prob]); break;
			default: PlNoth(random(5)+1,Words[prob]);
		 }
	  }
	  break;
	  case 1:
	  {
		 switch (Comb) {
			case 1:
			   if (MFlush>=0)
				  GameTurn(40,random(2)+1,Words[prob]);
			   else
				  GameTurn(30,random(2)+1,Words[prob]);
			break;
			case 2: GameTurn(60,random(2)+1,Words[prob]); break;
			case 3: GameTurn(90,random(3)+1,Words[prob]); break;
			case 4: GameTurn(120,random(3)+1,Words[prob]); break;
			default: GameTurn(50*Comb,random(5)+1,Words[prob]);
		  }
	  }
	  break;
	  case 2:
	  {
		 switch (Comb)
		 {
			case 1:
			{
			   if ((MFlush>=0)||(IfRaise==True))
				  PlNoth(random(2)+1,Words[prob]);
				else
				{
				   MainForm->lblDialog->Caption="� ���� ������. ��� � ����?";
				   Noth = True;
				}
			}
			break;
			case 2: PlNoth(random(2)+1,Words[prob]); break;
			case 3: PlNoth(random(3)+1,Words[prob]); break;
			default: PlNoth(random(5)+1,Words[prob]);
		 }
		 ChNames("Call","Raise");
		 if (Noth==True)
			ChNames("Check","Bet");
	  }
   }
}

void PlRaise()
{
   MainForm->PRaise->Visible=True;
   MainForm->btnCheck->Enabled=False;
   MainForm->btnRaise->Enabled=False;
   MainForm->btnFold->Enabled=False;
}

void BackGame()
{
   MainForm->PRaise->Visible=False;
   MainForm->btnCheck->Enabled=True;
   MainForm->btnRaise->Enabled=True;
   MainForm->btnFold->Enabled=True;
}

void Blind(int PlBet)
{
   int NewBet;
   NewBet = Bet+PlBet;
   PlCash -= NewBet;
   Cash += NewBet;
   MainForm->lblPlCash->Caption = IntToStr(PlCash);
   MainForm->lblCash->Caption = IntToStr(Cash);
   Bet = PlBet;
   BackGame();
   PartnerTurn(1);
}

void Trade(Cards Partner[], int value, Combination *PartnerComb, TImage *Cards[], TLabel *Label)
{
   int Card, index;
   for (index = 0; index < 5; index++)
	  if (ChCards[index]!=-1) {
		 srand(time(0));
		 Card = random(52);
		 while (InHands(Hands,Card))
			Card = random(52);
		 Hands[value+index] = Card;
		 RecordCards(Partner,index,Card);
	  }
   SortCards(Partner);
   Search(Partner,PartnerComb);
   if (value==10)
	  for (index = 0; index < 5; index++)
		 (Cards[index])->Picture->LoadFromFile(Deck[Partner[index].number]);
   Label->Caption = (*PartnerComb).comb;
}

void ChangeCards(bool Player)
{  TImage *PlayerCard[5] ={MainForm->imgPlayerCard1, MainForm->imgPlayerCard2,
   MainForm->imgPlayerCard3, MainForm->imgPlayerCard4, MainForm->imgPlayerCard5};
   TImage *DealerCard[5] = {MainForm->imgDealerCard1, MainForm->imgDealerCard2,
   MainForm->imgDealerCard3, MainForm->imgDealerCard4, MainForm->imgDealerCard5};
	if (Player) {
	   MainForm->lblCh1->Visible=False;
	   MainForm->lblCh2->Visible=False;
	   MainForm->lblCh3->Visible=False;
	   MainForm->lblCh4->Visible=False;
	   MainForm->lblCh5->Visible=False;
	   Trade(PlCards,10,&PlComb,PlayerCard,MainForm->lblPlComb);
	}
	else
		Trade(DlCards,15,&DlComb,DealerCard,MainForm->lblDlComb);
}

void Check()
{
	if ((Bet==0) && (IfRaise==False))
	   PartnerTurn(0);
	else
	{
	   PlCash-=Bet;
	   Cash+=Bet;
	   MainForm->lblCash->Caption=IntToStr(Cash);
	   MainForm->lblPlCash->Caption=IntToStr(PlCash);
	   MainForm->btnCheck->Visible = False;
	   MainForm->btnRaise->Visible = False;
	   MainForm->btnFold->Visible = False;
	   if (IfRaise==False) {
		  MainForm->btnChange->Visible = True;
		  MainForm->lblDialog->Caption="�� ��� ��? ����� ������ �����...";
		  CardCh = True;
		  IfRaise = True;
	   }
	   else
	   {
		   MainForm->lblDialog->Caption = "�������. ������� ���� �����...";
		   MainForm->Time_Open->Tag = 0;
		   MainForm->Time_Open->Enabled = True;
	   }
	}
}

void ChangeDealerCards()
{
	int index,count,prob,amount;
	Str word;
	count = 0;
	srand(time(0));
	for (index = 0; index < 5; index++)
	   ChCards[index]=-1;
	switch (DlComb.priorcomb) {
	   case 1:
		  if (MFlush>=0)
		  {
			 ChCards[MFlush]=MFlush;
			 count++;
		  }
		  else
		  {
			 prob = random(3)+3;
			 amount = prob;
			 for (index=0; index < amount; index++) {
				prob = random(5);
				while (ChCards[prob] == prob)
				   prob = random(5);
				ChCards[prob] = prob;
				count++;
			 }
		  }
	   break;
	   case 2:
		  if ((MFlush>=0)&&(DlComb.signcomb<6))
		  {
			 ChCards[MFlush]=MFlush;
			 count++;
		  }
		  else
		  {
			 for (index = 0; index < 5; index++)
				if (DlCards[index].priority!=DlComb.signcomb)
				{
					prob = random(3);
					if (count!=prob)
					   ChCards[index]=index;
					count++;
				}
			 count--;
		  }
	   break;
	   case 3:
		  for (index = 0; index < 5; index++)
			 if ((DlCards[index].priority!=DlComb.signcomb/100)&&(DlCards[index].priority!=DlComb.signcomb%100)) {
				ChCards[index]=index;
				count++;
			 }
	   break;
	   case 4:
	   {
		  for (index = 0; index < 5; index++)
			 if (DlCards[index].priority!=DlComb.signcomb) {
				prob = random(2);
				if (count!=prob)
				   ChCards[index]=index;
				count++;
			  }
		  count--;
	   }
	   break;
	}
	switch (count) {
	   case 0: word = " ����!"; break;
	   case 1: word = " �����!"; break;
	   case 5: word = " ����!"; break;
	   default : word = " �����!";
	}
	MainForm->lblDialog->Caption = "� ������� "+IntToStr(count)+word;
	IfRaise = True;
	Bet = 0;
}

void CleanHands( )
{
   for (int index = 0; index < 20; index++)
	  Hands[index]=-1;
	for (int i = 0; i < 5; i++)
	   ChCards[i] = -1;
}

void IsBegin()
{
   if (AmountGames == 0) {
	  PlCash = 250;
	  DlCash = 250;
   }
   AmountGames++;
}

void Make_Deck()
{
   for (int index = 0; index < 52; index++)
	  Deck[index] = ExtractFilePath(Application->ExeName)+"Cards\\"+IntToStr(index)+".jpg";
   Deck[52] = ExtractFilePath(Application->ExeName)+"Cards\\Back.jpg";
}

void ChangeCard(TSpeedButton *Button, TLabel *Label, int index)
{
   if (Button->Visible) {
	  Label->Visible = !Label->Visible;
	  if (Label->Visible)
		 ChCards[index] = index;
	  else
		 ChCards[index] = -1;
	}
}

void FirstBlind()
{
   PlCash -=5;
   DlCash -=5;
   Cash += 10;
   MainForm->lblCash->Caption = IntToStr(Cash);
   MainForm->lblPlCash->Caption = IntToStr(PlCash);
   MainForm->lblDlCash->Caption = IntToStr(DlCash);
}

void FirstTurn()
{
   MainForm->lblDialog->Caption = "";
   MainForm->btnStartGame->Visible = False;
   MainForm->btnFold->Visible = True;
   MainForm->btnCheck->Visible = True;
   MainForm->btnRaise->Visible = True;
   if (AmountGames%2!=0)
	   ChNames("Check","Bet");
   else
	   PartnerTurn(2);
}

void StartGame()
{
   CardsOnTable();
   FirstBlind();
   FirstTurn();
}

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
   Make_Deck();
}

void __fastcall TMainForm::btnStartGameClick(TObject *Sender)
{
   Retry(Cash);
   CleanHands();
   IsBegin();
   StartGame();
}

void __fastcall TMainForm::NewGameClick(TObject *Sender)
{
   Retry(0);
   CleanHands();
   AmountGames = 0;
   IsBegin();
   StartGame();
}

void __fastcall TMainForm::btnRaiseClick(TObject *Sender)
{
   PlRaise();
}

void __fastcall TMainForm::btnChangeClick(TObject *Sender)
{
   ChangeCards(True);
   ChangeDealerCards();
   ChangeCards(False);
   MainForm->btnChange->Visible = False;
   Application->ProcessMessages();
   Sleep(1200);
   MainForm->btnFold->Visible = True;
   MainForm->btnCheck->Visible = True;
   MainForm->btnRaise->Visible = True;
   if (CardCh==True)
	  PartnerTurn(2);
   else
	  ChNames("Check","Bet");
}

void Fold()
{
	Str Phrases[] = {"������, ��� ������� - � ���� ��� ��� ����� ������!",
	"�������? ������ ��� �����!"};
	if (DlComb.priorcomb>1)
	   MainForm->lblDialog->Caption = Phrases[1];
	else
	   MainForm->lblDialog->Caption = Phrases[0];
	DlCash += Cash;
	MainForm->lblDlCash->Caption = IntToStr(DlCash);
	Cash = 0;
	Retry(Cash);
	NoMoney();
}

void __fastcall TMainForm::btnFoldClick(TObject *Sender)
{
   Fold();
}

void __fastcall TMainForm::imgPlayerCard1Click(TObject *Sender)
{
   ChangeCard(MainForm->btnChange,MainForm->lblCh1,0);
}

void __fastcall TMainForm::imgPlayerCard2Click(TObject *Sender)
{
   ChangeCard(MainForm->btnChange,MainForm->lblCh2,1);
}

void __fastcall TMainForm::imgPlayerCard3Click(TObject *Sender)
{
   ChangeCard(MainForm->btnChange,MainForm->lblCh3,2);
}

void __fastcall TMainForm::imgPlayerCard4Click(TObject *Sender)
{
   ChangeCard(MainForm->btnChange,MainForm->lblCh4,3);
}

void __fastcall TMainForm::imgPlayerCard5Click(TObject *Sender)
{
   ChangeCard(MainForm->btnChange,MainForm->lblCh5,4);
}

void __fastcall TMainForm::btnCheckClick(TObject *Sender)
{
   Check();
}

void __fastcall TMainForm::ExitGameClick(TObject *Sender)
{
   Application->Terminate();
}

void __fastcall TMainForm::Time_OpenTimer(TObject *Sender)
{
	TImage *DealerCard[5] = {MainForm->imgDealerCard1, MainForm->imgDealerCard2,
	MainForm->imgDealerCard3, MainForm->imgDealerCard4, MainForm->imgDealerCard5};
   if (Tag!=5)
   {
	  DealerCard[Tag]->Picture->LoadFromFile(Deck[DlCards[Tag].number]);
	  Tag++;
   }
   else
   {
	  Tag = 0;
	  Time_Open->Enabled=False;
	  EndGame();
   }
}

void __fastcall TMainForm::btnBackClick(TObject *Sender)
{
   BackGame();
}

void __fastcall TMainForm::btnR5Click(TObject *Sender)
{
   Blind(5);
}

void __fastcall TMainForm::btnR10Click(TObject *Sender)
{
   Blind(10);
}

void __fastcall TMainForm::btnR15Click(TObject *Sender)
{
   Blind(15);
}

void __fastcall TMainForm::btnR20Click(TObject *Sender)
{
   Blind(20);
}

void __fastcall TMainForm::btnR25Click(TObject *Sender)
{
   Blind(25);
}

void __fastcall TMainForm::AllCombinationsClick(TObject *Sender)
{
   FormCombinations->ShowModal();
}

void __fastcall TMainForm::GameRulesClick(TObject *Sender)
{
   FormRules->ShowModal();
}

