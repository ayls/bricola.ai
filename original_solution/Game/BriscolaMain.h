//---------------------------------------------------------------------------
#ifndef BriscolaMainH
#define BriscolaMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "hooks.c"
//---------------------------------------------------------------------------
class TfrmBriscola : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *mmApp;
    TMenuItem *GameMainItem;
    TMenuItem *NewgameItem;
    TMenuItem *N1;
    TMenuItem *ExitItem;
    TStatusBar *sbGame;
    TImageList *imlCards;
    TImage *imCardThree;
    TImage *imCardTwo;
    TImage *imCardOne;
    TImage *imBriscola;
    TImage *imPlayerCardDown;
    TImage *imComputerCardDown;
    TTimer *Timer;
    TTimer *CollectTimer;
    TLabel *lbPlayerCards;
    TLabel *lbBriscola;
    TLabel *lbPlayerPlayCard;
    TLabel *lbComputerPlayCard;
    TMenuItem *SettingsMenuItem;
    TPanel *pnComputerCards;
    TImage *imCompCardThree;
    TImage *imCompCardTwo;
    TImage *imCompCardOne;
    TLabel *lbComputersCards;
    TPanel *pnClick;
    TMenuItem *AboutMenuItem;
    void __fastcall ExitItemClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall NewgameItemClick(TObject *Sender);
    void __fastcall CardClick(TObject *Sender);
    void __fastcall TimerTimer(TObject *Sender);
    void __fastcall CollectTimerTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall SettingsMenuItemClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall pnClickClick(TObject *Sender);
    
    void __fastcall AboutMenuItemClick(TObject *Sender);
private:	// User declarations
    Boolean PlayerLost;
    Boolean PlayerWasFirst;
    int PlayerCollectedCount;
    int ComputerCollectedCount;
    int LastSixCardsCount;
    int CardsTakenFromDeck; //number of cards taken from deck
    int PlayerCardDown;
    int ComputerCardDown;
    int PlayerPoints;
    int ComputerPoints;
    int PlayerCardDownIndex; //index of card from array
    int ComputerCardDownIndex;
    int cardsTakenFromDeck; //number of cards taken from deck
    int Briscola; //the card under the deck
    int CardValues[40]; //values of the cards
    int ShuffledCards[40];  //deck (shuffled)
    int PlayerCardsInHand[3]; //player's cards
    int ComputerCardsInHand[3]; //computer's cards
    int PlayerCollected[40]; //array of cards taken by player
    int ComputerCollected[40]; //array of cards taken by computer
    void LoadCardBitmaps(void);
    void InitializeGameVariables(void);
    void Deal(void);
    void ShuffleCards(void);
    void Initialize(void);
    void CollectCards(void);
    void PlayerCollects(void);
    void ComputerCollects(void);
    void TakeCards(void);
    void ComputerTakeCard(void);
    void PlayerTakeCard(void);
    void ComputerTakeBriscola(void);
    void PlayerTakeBriscola(void);
    void EndTurn(void);
    Boolean DeclareWinner(void);
    void ComputerPlayCard(void);
    void LoadLogic(void);
    int DetermineCardToPlay(void);
    void FillCaseArray(void);
    MyString IsCardInGame(int Card);
    void PlayBestCard(void);
    void CheckSettings(void);
    int PlayCombinatorical(int compCards[3], int playerCards[3]);
    int DoesComputerCollects(int compCard,int playerCard);
public:		// User declarations
    __fastcall TfrmBriscola(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBriscola *frmBriscola;
//---------------------------------------------------------------------------
#endif
