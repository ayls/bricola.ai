//---------------------------------------------------------------------------
//
//  Briscola Game
//    - April 2000: Ales Daneu, Slavko Drnovscek, Miha Kralj
//
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#define WIN32

#include "BriscolaMain.h"
#include "Dialog.h"
#include "About.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

TfrmBriscola *frmBriscola;
FILE		*F;
MyString *ccase;
char tempString[8][5];
char *cComputerTurn="Computer's turn";
char *cPlayerTurn="Player's turn";
char *cPlayerPointsPrefix="Player's points: ";
char *cComputerPointsPrefix="Computer's points: ";
char *cCardsLeftPrefix="Cards left: ";
char *cPlayerWon="Player won!";
char *cComputerWon="Computer won!";
char *cNoneWon="It's a tie!";
boolean SettingsChanged=false;
boolean UseCombinatoricalFinalPlay=true;
boolean playerDidntPlayedYet=true;

//---------------------------------------------------------------------------
//  Constructor
//---------------------------------------------------------------------------
__fastcall TfrmBriscola::TfrmBriscola(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
//  Exit menu choice code
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::ExitItemClick(TObject *Sender)
{
  FreeGlobals();
  Application->Terminate();
}

//---------------------------------------------------------------------------
//  Check if settings are in registry, otherwise save default values
//---------------------------------------------------------------------------
void TfrmBriscola::CheckSettings(void)
{
  if (AreSettingsInRegistry()==false) WriteToRegistry("0","briscola","cards.dll","0");
}

//---------------------------------------------------------------------------
//  Load card bitmaps
//---------------------------------------------------------------------------
void TfrmBriscola::LoadCardBitmaps(void)
{
  void *Handle=NULL;
  int i;
  Boolean DoExit;
  char tempBitmapName[10];
  char *cBitmapPrefix = "BITMAP_";
  char cardsDLL[30];
  Graphics::TBitmap *tempBitmapObj = new Graphics::TBitmap();

  DoExit = false;
  try {
    strcpy(cardsDLL,ReadCardsFileFromRegistry()); // cards dll file name is retrieved from registry
    Handle = LoadLibrary(cardsDLL);
    if (Handle != NULL) {
      imlCards->Clear();
      for(i=1;i<=40;i++) { //load all 40 cards into image list
        StrCopy(tempBitmapName,cBitmapPrefix);
        StrCat(tempBitmapName,IntToStr(i).c_str());
        tempBitmapObj->LoadFromResourceName((int)Handle,tempBitmapName);
        imlCards->Add(tempBitmapObj,tempBitmapObj);
      }
    }
    else {
      ShowMessage("Could not open DLL with card images!\nSee readme file for troubleshooting!"); //if handle is NULL then exit application
      DoExit = true;
    }
  }
  catch (...){
    ShowMessage("Error while loading card images!\nSee readme file for troubleshooting!"); //exception occured, exit application
    DoExit = true;
  }
  FreeLibrary(Handle);
  delete tempBitmapObj;
  if (DoExit==true) Application->Terminate();
}

//---------------------------------------------------------------------------
//  Initialize card values
//---------------------------------------------------------------------------
void TfrmBriscola::InitializeGameVariables(void)
{
  int i;

  for(i=0;i<40;i++) {
    switch(i) { //initialize card values
      case 0:
      case 10:
      case 20:
      case 30:CardValues[i] = 11;
              break;
      case 2:
      case 12:
      case 22:
      case 32:CardValues[i] = 10;
              break;
      case 7:
      case 17:
      case 27:
      case 37:CardValues[i] = 2;
              break;
      case 8:
      case 18:
      case 28:
      case 38:CardValues[i] = 3;
              break;
      case 9:
      case 19:
      case 29:
      case 39:CardValues[i] = 4;
              break;
      default:CardValues[i] = 0;
    }
    ShuffledCards[i] = i; //start order of cards
  }
}

//---------------------------------------------------------------------------
//  Initialization when game is first run
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::FormCreate(TObject *Sender)
{
  CheckSettings();
  InitializeGameVariables();
  LoadCardBitmaps();
  LoadLogic();
}

//---------------------------------------------------------------------------
//  Start new game
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::NewgameItemClick(TObject *Sender)
{
  if (SettingsChanged==true) {  // If settings have changed we have to load new cards and/or logic
    LoadCardBitmaps();
    LoadLogic();
    if (strcmp(ReadUseCombFinalPlayFromRegistry(),"1")==0) UseCombinatoricalFinalPlay=true;
    else UseCombinatoricalFinalPlay=false;
    SettingsChanged=false;
  }
  Initialize();
  ShuffleCards();
  Deal();
}

//---------------------------------------------------------------------------
//  Deal cards
//---------------------------------------------------------------------------
void TfrmBriscola::Deal(void)
{
  int i;

  //Shuffled cards array starts with index 0
  for (i=0;i<3;i++) {
    PlayerCardsInHand[i]=ShuffledCards[i]; //first three cards go to player
    ComputerCardsInHand[i]=ShuffledCards[i+3]; //next three cards are computer's
  }

  Briscola=ShuffledCards[6]; //the card under the deck
  CardsTakenFromDeck=6;
  imCardOne->Picture=NULL;  //initialize pictures to make sure that they are displayed correctly
  imCardTwo->Picture=NULL;
  imCardThree->Picture=NULL;
  imCompCardOne->Picture=NULL;
  imCompCardTwo->Picture=NULL;
  imCompCardThree->Picture=NULL;
  imBriscola->Picture=NULL;
  imlCards->GetBitmap(PlayerCardsInHand[0],imCardOne->Picture->Bitmap);  //load card images
  imlCards->GetBitmap(PlayerCardsInHand[1],imCardTwo->Picture->Bitmap);
  imlCards->GetBitmap(PlayerCardsInHand[2],imCardThree->Picture->Bitmap);
  imlCards->GetBitmap(ComputerCardsInHand[0],imCompCardOne->Picture->Bitmap);
  imlCards->GetBitmap(ComputerCardsInHand[1],imCompCardTwo->Picture->Bitmap);
  imlCards->GetBitmap(ComputerCardsInHand[2],imCompCardThree->Picture->Bitmap);
  imlCards->GetBitmap(Briscola,imBriscola->Picture->Bitmap);
  sbGame->Panels->Items[3]->Text=cCardsLeftPrefix+IntToStr(40-CardsTakenFromDeck);
}

//---------------------------------------------------------------------------
//  Shuffle cards
//---------------------------------------------------------------------------
void TfrmBriscola::ShuffleCards(void)
{
  int i,NewPos,currentValue;

  randomize();
  for(i=0;i<40;i++) {
    NewPos=random(40);
    currentValue=ShuffledCards[i];
    ShuffledCards[i]=ShuffledCards[NewPos];
    ShuffledCards[NewPos]=currentValue;
  }
}

//---------------------------------------------------------------------------
//  Initialize a new game
//---------------------------------------------------------------------------
void TfrmBriscola::Initialize(void)
{
  int i;

  CollectTimer->Enabled=false;
  Timer->Enabled=false;
  PlayerCardDown=-1;
  ComputerCardDown=-1;
  PlayerCardDownIndex=0; //index of card played by player
  ComputerCardDownIndex=0; //index of card played by computer
  PlayerCollectedCount=0;
  ComputerCollectedCount=0;
  PlayerPoints=0; //player's points
  ComputerPoints=0; //computer's points
  PlayerLost=false;
  CardsTakenFromDeck=0;
  LastSixCardsCount=0; //counter used in final stage of the game
  for(i=0;i<40;i++) { //initialize array of collected cards
    PlayerCollected[i]=0;
    ComputerCollected[i]=0;
  }
  // Initialize game text and card images
  sbGame->Panels->Items[0]->Text=cPlayerPointsPrefix+IntToStr(PlayerPoints);
  sbGame->Panels->Items[1]->Text=cComputerPointsPrefix+IntToStr(ComputerPoints);
  if (PlayerLost==true) sbGame->Panels->Items[2]->Text=cComputerTurn;
  else sbGame->Panels->Items[2]->Text=cPlayerTurn;
  sbGame->Panels->Items[3]->Text=cCardsLeftPrefix+IntToStr(40-CardsTakenFromDeck);
  imPlayerCardDown->Picture=NULL;
  imComputerCardDown->Picture=NULL;
  imBriscola->Picture=NULL;
}

//---------------------------------------------------------------------------
//  Play a card
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::CardClick(TObject *Sender)
{
  // if we are not if final stage of game and we haven't played yet...
  if ((LastSixCardsCount!=6)&&(playerDidntPlayedYet==true)) {
    // If it is our turn then go on
    if (((PlayerCardDown==-1)&&(PlayerLost==false))||
        ((ComputerCardDown>=0)&&(PlayerLost==true))) {
      // Remember if we played first in this turn
      if (ComputerCardDown==-1) PlayerWasFirst=true;
      else PlayerWasFirst=false;
      // Card one selected
      if ((((TImage*)(Sender))->Name=="imCardOne")&&(PlayerCardsInHand[0]>=0)) {
        PlayerCardDownIndex=0;
        PlayerCardDown=PlayerCardsInHand[0];
        imlCards->GetBitmap(PlayerCardsInHand[0],imPlayerCardDown->Picture->Bitmap);
        imCardOne->Picture=NULL;
        sbGame->Panels->Items[2]->Text=cComputerTurn;
        playerDidntPlayedYet=false;
      }
      // Card two
      if ((((TImage*)(Sender))->Name=="imCardTwo")&&(PlayerCardsInHand[1]>=0)) {
        PlayerCardDownIndex=1;
        PlayerCardDown=PlayerCardsInHand[1];
        imlCards->GetBitmap(PlayerCardsInHand[1],imPlayerCardDown->Picture->Bitmap);
        imCardTwo->Picture=NULL;
        sbGame->Panels->Items[2]->Text=cComputerTurn;
        playerDidntPlayedYet=false;
      }
      // Card three
      if ((((TImage*)(Sender))->Name=="imCardThree")&&(PlayerCardsInHand[2]>=0)) {
        PlayerCardDownIndex=2;
        PlayerCardDown=PlayerCardsInHand[2];
        imlCards->GetBitmap(PlayerCardsInHand[2],imPlayerCardDown->Picture->Bitmap);
        imCardThree->Picture=NULL;
        sbGame->Panels->Items[2]->Text=cComputerTurn;
        playerDidntPlayedYet=false;
      }
    }
    // Restart timer
    Timer->Enabled=false;
    Timer->Enabled=true;
  }
}

//---------------------------------------------------------------------------
//  Computer plays his move
//---------------------------------------------------------------------------
void TfrmBriscola::ComputerPlayCard(void)
{
  int tempCompCards[3],tempPlayerCards[3];

  // Remebmer if player played first in this turn
  if (PlayerCardDown==-1) PlayerWasFirst=false;
  else PlayerWasFirst=true;
  if (40-CardsTakenFromDeck==0) {
    //determine the best option to throw (settings determine how it will be determined)
    if (UseCombinatoricalFinalPlay==true) {
      for (int i=0;i<3;i++) {
        tempCompCards[i]=ComputerCardsInHand[i];
        tempPlayerCards[i]=PlayerCardsInHand[i];
      }
      PlayCombinatorical(tempCompCards,tempPlayerCards);
    }
    else PlayBestCard();
  } else ComputerCardDownIndex=DetermineCardToPlay()-1; // decision tree (or rules) used
  ComputerCardDown=ComputerCardsInHand[ComputerCardDownIndex];
  switch (ComputerCardDownIndex) {
    case(0):imCompCardOne->Picture=NULL;
            break;
    case(1):imCompCardTwo->Picture=NULL;
            break;
    case(2):imCompCardThree->Picture=NULL;
  }
  imlCards->GetBitmap(ComputerCardsInHand[ComputerCardDownIndex],imComputerCardDown->Picture->Bitmap);
  sbGame->Panels->Items[2]->Text=cPlayerTurn;
}

//---------------------------------------------------------------------------
//  End of turn, time for the winner to take cards on table
//---------------------------------------------------------------------------
void TfrmBriscola::CollectCards(void)
{
  int low,high,lowP,highP;

  // Save range for cards of the same type as briscola
  if ((Briscola>=0)&&(Briscola<=9)) {low=0; high=9;}
  else if ((Briscola>=10)&&(Briscola<=19)) {low=10; high=19;}
  else if ((Briscola>=20)&&(Briscola<=29)) {low=20; high=29;}
  else {low=30; high=39;}
  // If player played a crad of same type as briscola...
  if ((PlayerCardDown>=low)&&(PlayerCardDown<=high)) {
    // Computer also played card of the same type as briscola...
    if ((ComputerCardDown>=low)&&(ComputerCardDown<=high)) {
      // Check if player's card is of greater value
      if (CardValues[PlayerCardDown]>CardValues[ComputerCardDown]) PlayerCollects();
      else {
        // Cards are of the same value (zero), the one with higher number wins
        if (CardValues[PlayerCardDown]==CardValues[ComputerCardDown]) {
          if (PlayerCardDown>ComputerCardDown) PlayerCollects();
          else ComputerCollects();
        }
        else ComputerCollects();
      }
    }
    else PlayerCollects();
  }
  // Player did not play a card of the same type as briscola
  else {
    // If computer played a card of the same type as briscola he wins turn
    if ((ComputerCardDown>=low)&&(ComputerCardDown<=high)) ComputerCollects();
    else {
      if (PlayerLost==true) {
        // Computer played first in this turn
        if ((ComputerCardDown>=0)&&(ComputerCardDown<=9)) {lowP=0; highP=9;}
        else if ((ComputerCardDown>=10)&&(ComputerCardDown<=19)) {lowP=10; highP=19;}
        else if ((ComputerCardDown>=20)&&(ComputerCardDown<=29)) {lowP=20; highP=29;}
        else {lowP=30; highP=39;}
        // If player's card is of the same type as computer's
        if ((PlayerCardDown>=lowP)&&(PlayerCardDown<=highP)) {
          if (CardValues[PlayerCardDown]>CardValues[ComputerCardDown]) PlayerCollects();
          else {
            if (CardValues[PlayerCardDown]==CardValues[ComputerCardDown]) {
              if (PlayerCardDown>ComputerCardDown) PlayerCollects();
              else ComputerCollects();
            }
            else ComputerCollects();
          }
        }
        else ComputerCollects();
      }
      // Player played first
      else {
        if ((PlayerCardDown>=0)&&(PlayerCardDown<=9)) {lowP=0; highP=9;}
        else if ((PlayerCardDown>=10)&&(PlayerCardDown<=19)) {lowP=10; highP=19;}
        else if ((PlayerCardDown>=20)&&(PlayerCardDown<=29)) {lowP=20; highP=29;}
        else {lowP=30; highP=39;}
        if ((ComputerCardDown>=lowP)&&(ComputerCardDown<=highP)) {
          if (CardValues[PlayerCardDown]>CardValues[ComputerCardDown]) PlayerCollects();
          else {
            if (CardValues[PlayerCardDown]==CardValues[ComputerCardDown]) {
              if (PlayerCardDown>ComputerCardDown) PlayerCollects();
              else ComputerCollects();
            }
            else ComputerCollects();
          }
        }
        else PlayerCollects();
      }
    }
  }
}

//---------------------------------------------------------------------------
//  Take a new card form deck
//---------------------------------------------------------------------------
void TfrmBriscola::TakeCards(void)
{
  if ((40-CardsTakenFromDeck)>0) {
    if (PlayerLost==true) {
      ComputerTakeCard();
      if (CardsTakenFromDeck==39) PlayerTakeBriscola();
      else PlayerTakeCard();
    }
    else {
      PlayerTakeCard();
      if (CardsTakenFromDeck==39) ComputerTakeBriscola();
      else ComputerTakeCard();
    }
    if (CardsTakenFromDeck==40) imBriscola->Picture=NULL;
  }
  else LastSixCardsCount=LastSixCardsCount+2;
}

//---------------------------------------------------------------------------
//  Player takes cards played in last turn
//---------------------------------------------------------------------------
void TfrmBriscola::PlayerCollects(void)
{
  PlayerCollected[PlayerCollectedCount]=PlayerCardDown;
  PlayerCollectedCount++;
  PlayerCollected[PlayerCollectedCount]=ComputerCardDown;
  PlayerCollectedCount++;
  PlayerPoints=PlayerPoints+CardValues[PlayerCardDown]+CardValues[ComputerCardDown];
  PlayerLost=false;
}

//---------------------------------------------------------------------------
//  Computer takes cards played in last turn
//---------------------------------------------------------------------------
void TfrmBriscola::ComputerCollects(void)
{
  ComputerCollected[ComputerCollectedCount]=PlayerCardDown;
  ComputerCollectedCount++;
  ComputerCollected[ComputerCollectedCount]=ComputerCardDown;
  ComputerCollectedCount++;
  ComputerPoints=ComputerPoints+CardValues[PlayerCardDown]+CardValues[ComputerCardDown];
  PlayerLost=true;
}

//---------------------------------------------------------------------------
//  Computer takes card form deck
//---------------------------------------------------------------------------
void TfrmBriscola::ComputerTakeCard(void)
{
  CardsTakenFromDeck++;
  ComputerCardsInHand[ComputerCardDownIndex]=ShuffledCards[CardsTakenFromDeck];
  switch (ComputerCardDownIndex) {
    case(0):imlCards->GetBitmap(ComputerCardsInHand[ComputerCardDownIndex],imCompCardOne->Picture->Bitmap);
            break;
    case(1):imlCards->GetBitmap(ComputerCardsInHand[ComputerCardDownIndex],imCompCardTwo->Picture->Bitmap);
            break;
    case(2):imlCards->GetBitmap(ComputerCardsInHand[ComputerCardDownIndex],imCompCardThree->Picture->Bitmap);
  }
}

//---------------------------------------------------------------------------
//  Player takes card from deck
//---------------------------------------------------------------------------
void TfrmBriscola::PlayerTakeCard(void)
{
  CardsTakenFromDeck++;
  PlayerCardsInHand[PlayerCardDownIndex]=ShuffledCards[CardsTakenFromDeck];
  switch (PlayerCardDownIndex) {
    case(0):imlCards->GetBitmap(PlayerCardsInHand[PlayerCardDownIndex],imCardOne->Picture->Bitmap);
            break;
    case(1):imlCards->GetBitmap(PlayerCardsInHand[PlayerCardDownIndex],imCardTwo->Picture->Bitmap);
            break;
    case(2):imlCards->GetBitmap(PlayerCardsInHand[PlayerCardDownIndex],imCardThree->Picture->Bitmap);
  }
}

//---------------------------------------------------------------------------
//  Player gets the briscola card
//---------------------------------------------------------------------------
void TfrmBriscola::PlayerTakeBriscola(void)
{
  CardsTakenFromDeck++;
  PlayerCardsInHand[PlayerCardDownIndex]=ShuffledCards[6];
  switch (PlayerCardDownIndex) {
    case(0):imlCards->GetBitmap(PlayerCardsInHand[PlayerCardDownIndex],imCardOne->Picture->Bitmap);
            break;
    case(1):imlCards->GetBitmap(PlayerCardsInHand[PlayerCardDownIndex],imCardTwo->Picture->Bitmap);
            break;
    case(2):imlCards->GetBitmap(PlayerCardsInHand[PlayerCardDownIndex],imCardThree->Picture->Bitmap);
  }
}

//---------------------------------------------------------------------------
//  Computer gets briscola card
//---------------------------------------------------------------------------
void TfrmBriscola::ComputerTakeBriscola(void)
{
  CardsTakenFromDeck++;
  ComputerCardsInHand[ComputerCardDownIndex]=ShuffledCards[6];
  switch (ComputerCardDownIndex) {
    case(0):imlCards->GetBitmap(ComputerCardsInHand[ComputerCardDownIndex],imCompCardOne->Picture->Bitmap);
            break;
    case(1):imlCards->GetBitmap(ComputerCardsInHand[ComputerCardDownIndex],imCompCardTwo->Picture->Bitmap);
            break;
    case(2):imlCards->GetBitmap(ComputerCardsInHand[ComputerCardDownIndex],imCompCardThree->Picture->Bitmap);
  }
}

//---------------------------------------------------------------------------
//  End current turn, both players played their move
//---------------------------------------------------------------------------
void TfrmBriscola::EndTurn(void)
{
  imPlayerCardDown->Picture=NULL;
  imComputerCardDown->Picture=NULL;
  if (LastSixCardsCount>0) {
    PlayerCardsInHand[PlayerCardDownIndex]=-1;
    ComputerCardsInHand[ComputerCardDownIndex]=-1;
  }
  PlayerCardDown=-1;
  ComputerCardDown=-1;
  sbGame->Panels->Items[0]->Text=cPlayerPointsPrefix+IntToStr(PlayerPoints);
  sbGame->Panels->Items[1]->Text=cComputerPointsPrefix+IntToStr(ComputerPoints);
  if (PlayerLost==true) sbGame->Panels->Items[2]->Text=cComputerTurn;
  else sbGame->Panels->Items[2]->Text=cPlayerTurn;
  sbGame->Panels->Items[3]->Text=cCardsLeftPrefix+IntToStr(40-CardsTakenFromDeck);
}

//---------------------------------------------------------------------------
//  Determine if we have a winner
//---------------------------------------------------------------------------
Boolean TfrmBriscola::DeclareWinner(void)
{
  if (LastSixCardsCount==6) {
    if (PlayerPoints>ComputerPoints) {
      ShowMessage(cPlayerWon);
      sbGame->Panels->Items[2]->Text=cPlayerWon;
    }
    if (PlayerPoints<ComputerPoints) {
      ShowMessage(cComputerWon);
      sbGame->Panels->Items[2]->Text=cComputerWon;
    }
    if (PlayerPoints==ComputerPoints) {
      ShowMessage(cNoneWon);
      sbGame->Panels->Items[2]->Text=cNoneWon;
    }
    return(true);
  }
  else return(false);
}

//---------------------------------------------------------------------------
//  Timeout after which computer plays his move
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::TimerTimer(TObject *Sender)
{
if (LastSixCardsCount<6) {
    if (((ComputerCardDown==-1)&&(PlayerLost==true))||
        ((PlayerCardDown>=0)&&(PlayerLost==false))) {
      if (ComputerCardDown==-1) ComputerPlayCard();
    }
    if ((PlayerCardDown>=0)&&(ComputerCardDown>=0)) CollectTimer->Enabled=true;
  }
}

//---------------------------------------------------------------------------
//  Timeout after which cards from last turn are collected
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::CollectTimerTimer(TObject *Sender)
{
  if (LastSixCardsCount<6) {
    CollectCards();
    TakeCards();
    EndTurn();
    DeclareWinner();
    CollectTimer->Enabled=false;
    playerDidntPlayedYet=true;
  }
}

//---------------------------------------------------------------------------
//  Load decision tree (rules)
//---------------------------------------------------------------------------
void TfrmBriscola::LoadLogic(void)
{
   /* set RULES variable to true if you want to use Rules instead of Tree */
   try {
     if (strcmp(ReadUseRulesFromRegistry(),"1")==0) RULES=1;
     else RULES=0;
     /* Process options  */
     strcpy(FileStem,ReadFileNameFromRegistry());
     /* Read information on attribute names, values, and classes  */
     if ( ! (F = GetFile(".names", "r")) )
       Error(0, Fn, "");
     GetNames(F);
     /* See whether there is a costs file  */
     if ( (F = GetFile(".costs", "r")) )
       GetMCosts(F);
     /* Read the appropriate classifier file.  Call CheckFile() to
        determine the number of trials, then allocate space for
        trees or rulesets  */
     if ( RULES ) {
       CheckFile(".rules", false);
       RuleSet = AllocZero(TRIALS+1, CRuleSet);
       ForEach(Trial, 0, TRIALS-1) {
         RuleSet[Trial] = GetRules(".rules");
       }
     }
     else {
       CheckFile(".tree", false);
       Pruned = AllocZero(TRIALS+1, Tree);
       ForEach(Trial, 0, TRIALS-1) {
         Pruned[Trial] = GetTree(".tree");
       }
     }
     /* Close the classifier file and reset the file variable  */
     fclose(TRf);
     TRf = 0;
     /* create ccase */
     ccase=new MyString[MaxAtt];
   }
   catch (...) {
     ShowMessage("Could not read rules!\nSee readme file for troubleshooting!");
     Application->Terminate();
   }
}

//---------------------------------------------------------------------------
//  Get case description from our array
//---------------------------------------------------------------------------
Description GetDescriptionFromArray(MyBoolean Train)
/* Gets description from array */
{
  Attribute	Att;
  char name[1000], *endname;
  int  Dv;
  float	Cv;
  Description Dummy, DVec;
  MyBoolean	FirstValue=true;

  strcpy(name,ccase[0]); //start
  if ( name!="" ) {
	Dummy = AllocZero(MaxAtt+2, AttValue);
	DVec = &Dummy[1];
	ForEach(Att, 1, MaxAtt) {
      strcpy(name,ccase[Att-1]);
      if ( AttDef[Att] ) {
		DVec[Att] = EvaluateDef(AttDef[Att], DVec);
		continue;
	  }
      /* Get the attribute value if don't already have it */
      if ( ! FirstValue && ! (name!="") ) {
		Error(HITEOF, Fn, "");
		FreeCase(DVec);
		return Nil;
	  }
	  FirstValue = false;
      if ( SpecialStatus[Att] == EXCLUDE ) {
    	/* Record the value as a string */
        SVal(DVec,Att) = Alloc(strlen(name)+1, char);
		strcpy(SVal(DVec,Att), name);
	  }
	  else
	    if ( Discrete(Att) || SpecialStatus[Att] == DISCRETE ) {
          if ( ! ( strcmp(name, "?") ) ) {
		    Dv = 0;
		  }
          else {
		    Dv = Which(name, AttValName[Att], 1, MaxAttVal[Att]);
		    if ( ! Dv ) {
              if ( SpecialStatus[Att] == DISCRETE ) {
			    if ( Train ) {
			      /* Add value to list */
                  Dv = ++MaxAttVal[Att];
				  if ( Dv > (int) AttValName[Att][0] ) {
				    Error(TOOMANYVALS, AttName[Att],(char *) AttValName[Att][0]);
                  }
                  AttValName[Att][Dv] = CopyString(name);
			    }
			  }
			  else {
			    Error(BADATTVAL, AttName[Att], name);
			  }
		    }
		  }
          DVal(DVec, Att) = Dv;
	    }
	    else {
		  /* Continuous value */
          if ( ! ( strcmp(name, "?") ) ) {
		    Cv = UNKNOWN;
		  }
		  else
		    if ( SpecialStatus[Att] == DATEVAL ) {
		      Cv = DateToDay(name);
		      if ( Cv < 1 ) {
			    Error(BADDATE, AttName[Att], name);
			    Cv = UNKNOWN;
		      }
		    }
		    else {
    		  Cv = strtod(name, &endname);
		      if ( endname == name || *endname != '\0' )
			    Error(BADATTVAL, AttName[Att], name);
		    }
		    CVal(DVec, Att) = Cv;
	    }
	}
    if ( ClassAtt ) {
      Class(DVec) = DVal(DVec, ClassAtt);
    }
    else {
      if ( ! (name!="") ) {
        Error(HITEOF, Fn, "");
        FreeCase(DVec);
        return Nil;
      }
      Class(DVec) = Dv = Which(name, ClassName, 1, MaxClass);
    }
    return DVec;
  }
  else {
    return Nil;
  }
}

//---------------------------------------------------------------------------
//  Determine computers card to play
//---------------------------------------------------------------------------
int TfrmBriscola::DetermineCardToPlay(void)
{
  Description Case;
  ClassNo Predict,SelPredict;
  float maxConfidence=0;
  int tempCard,SelCard,iCount;

  /* Now classify the case in ccase array
     This has the same format as a .data file except that
     the class can be "?" to indicate that it is unknown. */
  iCount=2;
  for (int i=0;i<6;i++) {  //cards can be in hand in 6 possible orders try all of them and pick the best result
    tempCard=ComputerCardsInHand[iCount];
    ComputerCardsInHand[iCount]=ComputerCardsInHand[iCount-1];
    ComputerCardsInHand[iCount-1]=tempCard;
    FillCaseArray();
    ClassSum = Alloc(MaxClass+1, float);  /* used in classification */
    Vote = Alloc(MaxClass+1, float);  /* used with boosting */
    Case = GetDescriptionFromArray(false);
    Predict = Classify(Case);
    /* you can use Confidence too */
    if (Confidence>maxConfidence) {
      SelCard=ComputerCardsInHand[Predict-1];
      maxConfidence=Confidence;
    }
    FreeCase(Case); //free case
    iCount--;
    if (iCount==0) iCount=2;
  }
  for (int i=0;i<3;i++) {
    if (ComputerCardsInHand[i]==SelCard) SelPredict=i+1;
  }
  return (SelPredict);
}

//---------------------------------------------------------------------------
//  Application termination
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  FreeGlobals();
  Application->Terminate();
}

//---------------------------------------------------------------------------
//  Create an array with case that is passed to decision tree (rule set)
//---------------------------------------------------------------------------
void TfrmBriscola::FillCaseArray(void)
{
  int Range;
  char k_p[5],s_p[5],b_p[5],d_p[5],a_p[5];

  /* starting filling ccase array */
  if (PlayerWasFirst==true) ccase[0]="f";
  else ccase[0]="t";

  if ((Briscola>=0)&&(Briscola<=9)) Range=0;
  if ((Briscola>=10)&&(Briscola<=19)) Range=10;
  if ((Briscola>=20)&&(Briscola<=29)) Range=20;
  if ((Briscola>=30)&&(Briscola<=39)) Range=30;

  // fill array with cards in hand
  for (int k=0;k<3;k++) {
    if (ComputerCardsInHand[k]==-1) ccase[k+1]="-1"; //no card, write -1
    else
    {
      // set temp markings for cards (k - coppa, s - spada, b - baston, d - danari, a - cards of same type as briscola)
      strcpy(k_p,"k"); strcpy(s_p,"s"); strcpy(b_p,"b"); strcpy(d_p,"d"); strcpy(a_p,"a");
      // check if card is of the same type as briscola
      if ((ComputerCardsInHand[k]>=Range)&&(ComputerCardsInHand[k]<=Range+9))
        strcpy(tempString[k],strcat(a_p,IntToStr(ComputerCardsInHand[k]-Range+1).c_str()));
      else {
        if (ComputerCardsInHand[k]>=30) strcpy(tempString[k],strcat(k_p,IntToStr(ComputerCardsInHand[k]-30+1).c_str()));
        else if (ComputerCardsInHand[k]>=20) strcpy(tempString[k],strcat(s_p,IntToStr(ComputerCardsInHand[k]-20+1).c_str()));
        else if (ComputerCardsInHand[k]>=10) strcpy(tempString[k],strcat(b_p,IntToStr(ComputerCardsInHand[k]-10+1).c_str()));
        else strcpy(tempString[k],strcat(d_p,IntToStr(ComputerCardsInHand[k]+1).c_str()));
      }
      ccase[k+1]=tempString[k];
    }
  }
  // store players move (if there was already)
  if (PlayerWasFirst==true) {
    strcpy(k_p,"k"); strcpy(s_p,"s"); strcpy(b_p,"b"); strcpy(d_p,"d"); strcpy(a_p,"a");
    if ((PlayerCardDown>=Range)&&(PlayerCardDown<=Range+9))
      strcpy(tempString[3],strcat(a_p,IntToStr(PlayerCardDown-Range+1).c_str()));
    else {
      if (PlayerCardDown>=30) strcpy(tempString[3],strcat(k_p,IntToStr(PlayerCardDown-30+1).c_str()));
      else if (PlayerCardDown>=20) strcpy(tempString[3],strcat(s_p,IntToStr(PlayerCardDown-20+1).c_str()));
      else if (PlayerCardDown>=10) strcpy(tempString[3],strcat(b_p,IntToStr(PlayerCardDown-10+1).c_str()));
      else strcpy(tempString[3],strcat(d_p,IntToStr(PlayerCardDown+1).c_str()));
    }
    ccase[4]=tempString[3];
  }
  else ccase[4]="-1";
  strcpy(tempString[4],IntToStr(40-cardsTakenFromDeck).c_str());
  ccase[5]=tempString[4];
  strcpy(tempString[5],IntToStr(ComputerPoints).c_str());
  ccase[6]=tempString[5];
  strcpy(tempString[6],IntToStr(PlayerPoints).c_str());
  ccase[7]=tempString[6];
  strcpy(a_p,"a");
  strcpy(tempString[7],strcat(a_p,IntToStr(Briscola-Range+1).c_str()));
  ccase[8]=tempString[7];
  ccase[9]=IsCardInGame(Range);
  ccase[10]=IsCardInGame(Range+1);
  ccase[11]=IsCardInGame(Range+2);
  ccase[12]=IsCardInGame(Range+3);
  ccase[13]=IsCardInGame(Range+4);
  ccase[14]=IsCardInGame(Range+5);
  ccase[15]=IsCardInGame(Range+6);
  ccase[16]=IsCardInGame(Range+7);
  ccase[17]=IsCardInGame(Range+9);
  ccase[18]=IsCardInGame(Range+9);
  ccase[19]="?";
  /* ended filling ccase array */
}

//---------------------------------------------------------------------------
//  Check if card has been played already
//---------------------------------------------------------------------------
MyString TfrmBriscola::IsCardInGame(int Card)
{
  MyString tempResult="t";

  for (int j=0;j<40;j++) {
    if ((Card==PlayerCollected[j])||(Card==ComputerCollected[j])) tempResult="f";
  }
  return (tempResult);
}

//---------------------------------------------------------------------------
//  Final stage of play - safe play
//---------------------------------------------------------------------------
void TfrmBriscola::PlayBestCard(void)
{
  int i,minValIndx,minVal,maxValIndx,maxVal,Range,Range2;

  if ((Briscola>=0)&&(Briscola<=9)) Range=10;
  if ((Briscola>=10)&&(Briscola<=19)) Range=20;
  if ((Briscola>=20)&&(Briscola<=29)) Range=30;
  if ((Briscola>=30)&&(Briscola<=39)) Range=40;
  if (PlayerCardDown==-1) {  // player has not played yet
    minVal=12;
    minValIndx=-1;
    for (i=0;i<3;i++) {
      if (ComputerCardsInHand[i]!=-1) { // check if this slot is not empty
        if (CardValues[ComputerCardsInHand[i]]<=minVal) { //search for minimum value card
          minVal=CardValues[ComputerCardsInHand[i]];
          minValIndx=i;
        }
      }
    }
    // Play this move if card is not of the same type as briscola card and it's value is less than 10
    if ((minVal<10)&&(!((ComputerCardsInHand[minValIndx]>=Range-10)&&(ComputerCardsInHand[minValIndx]<=Range-1))))
      ComputerCardDownIndex=minValIndx;
    else { // try to find a better move
      minVal=12;
      minValIndx=-1;
      for (i=0;i<3;i++) { // search for minimum value card of the same type as briscola
        if (ComputerCardsInHand[i]!=-1) {
          if ((CardValues[ComputerCardsInHand[i]]<=minVal)&&((ComputerCardsInHand[i]>=Range-10)&&(ComputerCardsInHand[i]<=Range-1))) {
            minVal=CardValues[ComputerCardsInHand[i]];
            minValIndx=i;
          }
        }
      }
      // play selected card if such a card was found
      if (minVal<12) ComputerCardDownIndex=minValIndx;
      else { // else just play the minimum value card in hand
        minVal=12;
        minValIndx=-1;
        for (i=0;i<3;i++) {
          if (ComputerCardsInHand[i]!=-1) {
            if (CardValues[ComputerCardsInHand[i]]<=minVal) {
              minVal=CardValues[ComputerCardsInHand[i]];
              minValIndx=i;
            }
          }
        }
        ComputerCardDownIndex=minValIndx;
      }
    }
  }
  else { // Player already made his move
    if ((PlayerCardDown>=0)&&(PlayerCardDown<=9)) Range2=10;
    if ((PlayerCardDown>=10)&&(PlayerCardDown<=19)) Range2=20;
    if ((PlayerCardDown>=20)&&(PlayerCardDown<=29)) Range2=30;
    if ((PlayerCardDown>=30)&&(PlayerCardDown<=39)) Range2=40;
    maxVal=-1;
    maxValIndx=-1;
    for (i=0;i<3;i++) { //search for maximum card (of same type as players card) needed to get players card
      if (ComputerCardsInHand[i]!=-1) {
        if ((CardValues[ComputerCardsInHand[i]]>CardValues[PlayerCardDown])&&((ComputerCardsInHand[i]>=Range2-10)&&(ComputerCardsInHand[i]<=Range2-1))) {
          maxVal=CardValues[ComputerCardsInHand[i]];
          maxValIndx=i;
        }
      }
    }
    // if computer's card is not pointless -> play it
    if (maxVal>0) ComputerCardDownIndex=maxValIndx;
    else { //play minimum card (of briscola type) needed to get players card
      minVal=12;
      minValIndx=-1;
      for (i=0;i<3;i++) {
        if (ComputerCardsInHand[i]!=-1) {
          if ((CardValues[ComputerCardsInHand[i]]<=minVal)&&((ComputerCardsInHand[i]>=Range-10)&&(ComputerCardsInHand[i]<=Range-1))) {
            minVal=CardValues[ComputerCardsInHand[i]];
            minValIndx=i;
          }
        }
      }
      // if such card was found play it
      if (minVal<12) ComputerCardDownIndex=minValIndx;
      else { // just do damage limitation
        minVal=12;
        minValIndx=-1;
        for (i=0;i<3;i++) {
          if (ComputerCardsInHand[i]!=-1) {
            if (CardValues[ComputerCardsInHand[i]]<=minVal) {
              minVal=CardValues[ComputerCardsInHand[i]];
              minValIndx=i;
            }
          }
        }
        ComputerCardDownIndex=minValIndx;
      }
    }
  }
}

//---------------------------------------------------------------------------
//  Display settings dialog
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::SettingsMenuItemClick(TObject *Sender)
{
  if (SetDialog->ShowModal()==mrOk) SettingsChanged=true;
}

//---------------------------------------------------------------------------
//  Update screen when resizing
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::FormResize(TObject *Sender)
{
  sbGame->Panels->Items[0]->Width=(int)(sbGame->Width/4);
  sbGame->Panels->Items[1]->Width=(int)(sbGame->Width/4);
  sbGame->Panels->Items[2]->Width=(int)(sbGame->Width/4);
  if ((lbComputerPlayCard->Left+lbComputerPlayCard->Width)>(frmBriscola->ClientWidth))
    frmBriscola->ClientWidth=lbComputerPlayCard->Left+lbComputerPlayCard->Width+8;
  if ((sbGame->Top+sbGame->Height)>(frmBriscola->ClientHeight))
    frmBriscola->ClientHeight=sbGame->Top+sbGame->Height+8;
}

//---------------------------------------------------------------------------
//  Show/unhide panel with computer's cards
//---------------------------------------------------------------------------
void __fastcall TfrmBriscola::pnClickClick(TObject *Sender)
{
  if (strcmp(pnClick->Caption.c_str(),"Hide computer's cards")==0) {
    pnComputerCards->Visible=false;
    pnClick->Caption="Show computer cards";
  }
  else {
    pnComputerCards->Align=alNone;
    pnClick->Align=alNone;
    pnClick->Top=0;
    pnComputerCards->Visible=true;
    pnComputerCards->Align=alBottom;
    pnClick->Align=alBottom;
    pnClick->Caption="Hide computer's cards";
  }
  FormResize(this);
}

//---------------------------------------------------------------------------
//  Combinatorical final play
//---------------------------------------------------------------------------
int TfrmBriscola::PlayCombinatorical(int compCards[3], int playerCards[3])
{
  int maxPointsCardIndex=-1;
  int maxPoints=-121;
  int maxFirstCardValue=11;
  int maxNegPoints=121;
  int Points=0;
  int negPoints=0;
  int temp;
  int result=0;
  int tempCompCards[3],tempPlayerCards[3];
  boolean allCardsPlayed=true;

  // Check if all cards were played during recursion
  for (int i=0;i<3;i++) {
    if ((compCards[i]>=0)||(playerCards[i]>=0)) {
      allCardsPlayed=false;
      break;
    }
  }
  if (allCardsPlayed==true) result=0; //end condition for recursion
  else {
    // play all combinations of player and computer cards
    for (int i=0;i<3;i++) {
      if (compCards[i]>0) {
        for(int j=0;j<3;j++) {
          if (playerCards[j]>0) {
            Points=DoesComputerCollects(compCards[i],playerCards[j]); // Get points in this turn
            if (Points<0) {
              negPoints=Points;
              Points=0;
            }
            else negPoints=0;
            // Reinitialize temporary variables
            for (int k=0;k<3;k++) {
              tempCompCards[k]=compCards[k];
              tempPlayerCards[k]=playerCards[k];
            }
            tempCompCards[i]=-1; tempPlayerCards[j]=-1; // mark played cards
            temp=PlayCombinatorical(tempCompCards,tempPlayerCards); // recursion
            if (temp>0) Points=Points+temp;
            else negPoints=negPoints+temp;
            if ((Points>=maxPoints)&&(CardValues[compCards[i]]<=maxFirstCardValue)&&(negPoints<=maxNegPoints)) {
              maxPoints=Points;
              maxNegPoints=negPoints;
              maxPointsCardIndex=i;
              maxFirstCardValue=CardValues[compCards[i]];
            }
          }
        }
      }
    }
  }
  // just in case
  if (maxPointsCardIndex==-1) {
    for(int j=0;j<3;j++) {
      if (compCards[j]>=0) {
        maxPointsCardIndex=j;
        break;
      }
    }
  }
  ComputerCardDownIndex=maxPointsCardIndex;
  return(result);
}

//---------------------------------------------------------------------------
//  Check if computer would collect in case of given two cards
//---------------------------------------------------------------------------
int TfrmBriscola::DoesComputerCollects(int compCard,int playerCard)
{
  int low,high,lowP,highP;

  if ((Briscola>=0)&&(Briscola<=9)) {low=0; high=9;}
  else if ((Briscola>=10)&&(Briscola<=19)) {low=10; high=19;}
  else if ((Briscola>=20)&&(Briscola<=29)) {low=20; high=29;}
  else {low=30; high=39;}
  if ((playerCard>=low)&&(playerCard<=high)) {
    if ((compCard>=low)&&(compCard<=high)) {
      // both cards are of same type as briscola card
      if (CardValues[playerCard]>CardValues[compCard]) return(-CardValues[compCard]);
      else {
        if (CardValues[playerCard]==CardValues[compCard]) {
          if (playerCard>compCard) return(-CardValues[compCard]);
          else return(CardValues[playerCard]+CardValues[compCard]);
        }
        else return(CardValues[playerCard]+CardValues[compCard]);
      }
    }
    // computers card is not of the same type as briscola card
    else return(-CardValues[compCard]);
  }
  else {
    // player's card is not of briscola card type
    if ((compCard>=low)&&(compCard<=high)) return(CardValues[playerCard]+CardValues[compCard]);
    else { // both cards are not of the same type as briscola card
      if (PlayerLost==true) { // computer played first
        if ((compCard>=0)&&(compCard<=9)) {lowP=0; highP=9;}
        else if ((compCard>=10)&&(compCard<=19)) {lowP=10; highP=19;}
        else if ((compCard>=20)&&(compCard<=29)) {lowP=20; highP=29;}
        else {lowP=30; highP=39;}
        if ((playerCard>=lowP)&&(playerCard<=highP)) {
          if (CardValues[playerCard]>CardValues[compCard]) return(-CardValues[compCard]);
          else {
            if (CardValues[playerCard]==CardValues[compCard]) {
              if (playerCard>compCard) return(-CardValues[compCard]);
              else return(CardValues[playerCard]+CardValues[compCard]);
            }
            else return(CardValues[playerCard]+CardValues[compCard]);
          }
        }
        else return(CardValues[playerCard]+CardValues[compCard]);
      }
      else {  // player played first
        if ((playerCard>=0)&&(playerCard<=9)) {lowP=0; highP=9;}
        else if ((playerCard>=10)&&(playerCard<=19)) {lowP=10; highP=19;}
        else if ((playerCard>=20)&&(playerCard<=29)) {lowP=20; highP=29;}
        else {lowP=30; highP=39;}
        if ((compCard>=lowP)&&(compCard<=highP)) {
          if (CardValues[playerCard]>CardValues[compCard]) return(-CardValues[compCard]);
          else {
            if (CardValues[playerCard]==CardValues[compCard]) {
              if (playerCard>compCard) return(-CardValues[compCard]);
              else return(CardValues[playerCard]+CardValues[compCard]);
            }
            else return(CardValues[playerCard]+CardValues[compCard]);
          }
        }
        else return(-CardValues[compCard]);
      }
    }
  }
}

//---------------------------------------------------------------------------
//  Display about window
//---------------------------------------------------------------------------

void __fastcall TfrmBriscola::AboutMenuItemClick(TObject *Sender)
{
  AboutBox->ShowModal();    
}
//---------------------------------------------------------------------------

