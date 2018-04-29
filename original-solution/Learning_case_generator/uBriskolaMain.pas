////////////////////////////////////////////////////////////////////////////////
//
//  Learning case generator for briscola card game
//    April 2000: Ales Daneu, Slavko Drnovscek, Miha Kralj
//
////////////////////////////////////////////////////////////////////////////////
unit uBriskolaMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, ComCtrls, Menus, ImgList;

type
  TBriscolaMain = class(TForm)
    MainMenu: TMainMenu;
    Game: TMenuItem;
    Newgame: TMenuItem;
    N1: TMenuItem;
    Exitchoice: TMenuItem;
    StatusBar: TStatusBar;
    gbPlayerOne: TGroupBox;
    imPlayerOneCardOne: TImage;
    imPlayerOneCardThree: TImage;
    imPlayerOneCardTwo: TImage;
    gbPlayerTwo: TGroupBox;
    imPlayerTwoCardOne: TImage;
    imPlayerTwoCardThree: TImage;
    imPlayerTwoCardTwo: TImage;
    gbTable: TGroupBox;
    imPlayerTwoCardDown: TImage;
    imPlayerOneCardDown: TImage;
    imBriscola: TImage;
    imlCards: TImageList;
    procedure ExitchoiceClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure CardClick(Sender: TObject);
    procedure NewgameClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
    procedure ShuffleCards;
    procedure Deal;
    procedure CollectCards;
    procedure GiveCardsToWinner;
    procedure POneCollects;
    procedure PTwoCollects;
    procedure TakeCards;
    procedure POneTakeCard;
    procedure PTwoTakeCard;
    procedure POneTakeBriscola;
    procedure PTwoTakeBriscola;
    procedure EndTurn;
    Procedure Initialize;
    function DeclareWinner:boolean;
    procedure AppendGameData(OpenFile,CloseOpenedFile,POne:boolean);
  public
    { Public declarations }
  end;

var
  BriscolaMain: TBriscolaMain;

implementation

{$R *.DFM}

type  CardsArr=array [1..40] of integer;
      PlayerCards=array [1..3] of integer;

const cCardsLibName='cards.dll';
      cBitmapPrefix='BITMAP_';
      cPOnePointsPrefix='Points Player One: ';
      cPTwoPointsPrefix='Points Player Two: ';
      cCardsLeftPrefix='Cards left: ';
      cPOneTurn='Player One''s turn';
      cPTwoTurn='Player Two''s turn';
      cPOneWon='Player One won.';
      cPTwoWon='Player Two won.';
      cNoneWon='Players are tied.';
      cGameDataFile='Briscola.data';
      cCardNum=40;
      cPlayerCards=3;
      endline=#13+#10;

var   ShuffledCards:CardsArr;
      CardValues:CardsArr;
      POneCollected:CardsArr;
      PTwoCollected:CardsArr;
      POneCardsInHand:PlayerCards;
      PTwoCardsInHand:PlayerCards;
      Briscola:integer;
      cardsTakenFromDeck:integer;
      LastSixCardsCount:integer;
      POneCardDown:integer;
      PTwoCardDown:integer;
      POneCardDownIndex:integer;
      PTwoCardDownIndex:integer;
      POneCollectedCount:integer;
      PTwoCollectedCount:integer;
      POnePoints:integer;
      PTwoPoints:integer;
      POneLost:boolean;
      AllCardsTaken:boolean;
      CantPlay:boolean;
      POnewasFirst:boolean;
      dataFile:TextFile;

// Exit application
procedure TBriscolaMain.ExitchoiceClick(Sender: TObject);
begin
  AppendGameData(false,true,false);
  Application.Terminate;
end;

// Program initialization
procedure TBriscolaMain.FormCreate(Sender: TObject);
var
  Handle:Thandle;
  i:integer;
  tempBitmapName:string;
  tempBitmapObj:TBitmap;
begin
  CantPlay:=True;  // Flag indicating that a new game has to be started
  for i:=1 to cCardNum do begin
    // Set card values
    case i of
      1,11,21,31:CardValues[i]:=11;
      2,12,22,32:CardValues[i]:=0;
      3,13,23,33:CardValues[i]:=10;
      4,14,24,34:CardValues[i]:=0;
      5,15,25,35:CardValues[i]:=0;
      6,16,26,36:CardValues[i]:=0;
      7,17,27,37:CardValues[i]:=0;
      8,18,28,38:CardValues[i]:=2;
      9,19,29,39:CardValues[i]:=3;
      10,20,30,40:CardValues[i]:=4;
    end;
    ShuffledCards[i]:=i;  // Initialize deck
  end;
  // Load images for cards
  Handle := LoadLibrary(cCardsLibName);
  tempBitmapObj:=TBitmap.Create;
  try
    if Handle <> 0 then begin
      for i:=1 to cCardNum do begin
        tempBitmapName:=cBitmapPrefix+inttostr(i);
        tempBitmapObj.LoadFromResourceName(Handle,tempBitmapName);
        imlCards.add(tempBitmapObj,tempBitmapObj);
      end;
    end;
  finally
    tempBitmapObj.Free;
    FreeLibrary(Handle);
  end;
end;

//  Play a card (either player one or two)
procedure TBriscolaMain.CardClick(Sender: TObject);
begin
  if CantPlay=False then begin  // Is game started?
    // Is player one's turn?
    if (((POneCardDown=0)and(POneLost=False))or((PTwoCardDown>0)and(POneLost=True))) then begin
      // Remember if player one played first in this turn
      if PTwoCardDown=0 then POnewasFirst:=True
      else POneWasFirst:=False;
      // Card one was clicked
      if ((TImage(Sender).Name='imPlayerOneCardOne')and(POneCardsInHand[1]>0)) then begin
        // Remember what card was played
        POneCardDown:=POneCardsInHand[1];
        POneCardDownIndex:=1;
        // Update shown bitmaps
        imlCards.GetBitmap(POneCardDown-1,imPlayerOneCardDown.picture.bitmap);
        imPlayerOneCardOne.Picture:=nil;
        StatusBar.Panels[2].Text:=cPTwoTurn;  // Game messages
        AppendGameData(False,False,True);  // Save the move to a file
      end;
      // Card two
      if ((TImage(Sender).Name='imPlayerOneCardTwo')and(POneCardsInHand[2]>0)) then begin
        POneCardDown:=POneCardsInHand[2];
        POneCardDownIndex:=2;
        imlCards.GetBitmap(POneCardDown-1,imPlayerOneCardDown.picture.bitmap);
        imPlayerOneCardTwo.Picture:=nil;
        StatusBar.Panels[2].Text:=cPTwoTurn;
        AppendGameData(False,False,True);
      end;
      // Card three
      if ((TImage(Sender).Name='imPlayerOneCardThree')and(POneCardsInHand[3]>0)) then begin
        POneCardDown:=POneCardsInHand[3];
        POneCardDownIndex:=3;
        imlCards.GetBitmap(POneCardDown-1,imPlayerOneCardDown.picture.bitmap);
        imPlayerOneCardThree.Picture:=nil;
        StatusBar.Panels[2].Text:=cPTwoTurn;
        AppendGameData(False,False,True);
      end;
    end else begin // Player two's turn
      // Remember if player two played first in this turn
      if POneCardDown=0 then POnewasFirst:=False
      else POneWasFirst:=True;
      // Card one
      if ((TImage(Sender).Name='imPlayerTwoCardOne')and(PTwoCardsInHand[1]>0)) then begin
        PTwoCardDown:=PTwoCardsInHand[1];
        PTwoCardDownIndex:=1;
        imlCards.GetBitmap(PTwoCardDown-1,imPlayerTwoCardDown.picture.bitmap);
        imPlayerTwoCardOne.Picture:=nil;
        StatusBar.Panels[2].Text:=cPOneTurn;
        AppendGameData(False,False,False);
      end;
      // Card two
      if ((TImage(Sender).Name='imPlayerTwoCardTwo')and(PTwoCardsInHand[2]>0)) then begin
        PTwoCardDown:=PTwoCardsInHand[2];
        PTwoCardDownIndex:=2;
        imlCards.GetBitmap(PTwoCardDown-1,imPlayerTwoCardDown.picture.bitmap);
        imPlayerTwoCardTwo.Picture:=nil;
        StatusBar.Panels[2].Text:=cPOneTurn;
        AppendGameData(False,False,False);
      end;
      // card three
      if ((TImage(Sender).Name='imPlayerTwoCardThree')and(PTwoCardsInHand[3]>0)) then begin
        PTwoCardDown:=PTwoCardsInHand[3];
        PTwoCardDownIndex:=3;
        imlCards.GetBitmap(PTwoCardDown-1,imPlayerTwoCardDown.picture.bitmap);
        imPlayerTwoCardThree.Picture:=nil;
        StatusBar.Panels[2].Text:=cPOneTurn;
        AppendGameData(False,False,False);
      end;
    end;
    // Have both players made their move? If it so calculate the results, draw cards and
    // get ready for next turn (or declare winner).
    if ((POneCardDown>0)and(PTwoCardDown>0)) then begin
      CollectCards;
      TakeCards;
      EndTurn;
      if DeclareWinner then AppendGameData(False,True,False);  // Save info of who won
    end;
  end;
end;

// Start a new game
procedure TBriscolaMain.NewgameClick(Sender: TObject);
begin
  Initialize;  // Initialize game
  ShuffleCards;  // Shuffle cards
  Deal;  // Draw cards
  AppendGameData(True,False,False);  // Save starting positions
end;

// Draw cards
procedure TBriscolaMain.Deal;
var i:integer;
begin
  // Draw cards
  for i:=1 to cPlayerCards do begin
    POneCardsInHand[i]:=ShuffledCards[i];
    PTwoCardsInHand[i]:=ShuffledCards[i+3];
  end;
  Briscola:=ShuffledCards[2*cPlayerCards+1]; // Determine which card is briscola
  cardsTakenFromDeck:=6;
  // Display drawn cards
  imPlayerOneCardOne.picture:=nil;
  imPlayerTwoCardOne.picture:=nil;
  imPlayerOneCardTwo.picture:=nil;
  imPlayerTwoCardTwo.picture:=nil;
  imPlayerOneCardThree.picture:=nil;
  imPlayerTwoCardThree.picture:=nil;
  imBriscola.picture:=nil;
  imlCards.GetBitmap(POneCardsInHand[1]-1,imPlayerOneCardOne.picture.bitmap);
  imlCards.GetBitmap(PTwoCardsInHand[1]-1,imPlayerTwoCardOne.picture.bitmap);
  imlCards.GetBitmap(POneCardsInHand[2]-1,imPlayerOneCardTwo.picture.bitmap);
  imlCards.GetBitmap(PTwoCardsInHand[2]-1,imPlayerTwoCardTwo.picture.bitmap);
  imlCards.GetBitmap(POneCardsInHand[3]-1,imPlayerOneCardThree.picture.bitmap);
  imlCards.GetBitmap(PTwoCardsInHand[3]-1,imPlayerTwoCardThree.picture.bitmap);
  imlCards.GetBitmap(Briscola-1,imBriscola.picture.bitmap);
end;

// Shuffle cards
procedure TBriscolaMain.ShuffleCards;
var i:integer;
    NewPos:integer;
    currentValue:integer;
begin
  randomize;
  for i:=1 to cCardNum do begin
    NewPos:=Random(40)+1;
    currentValue:=ShuffledCards[i];
    ShuffledCards[i]:=ShuffledCards[NewPos];
    ShuffledCards[NewPos]:=currentValue;
  end;
end;

// The winner of the turn collects the cards
procedure TBriscolaMain.CollectCards;
begin
  GiveCardsToWinner;
end;

// Used when player one collects cards
procedure TBriscolaMain.POneCollects;
begin
  inc(POneCollectedCount);
  POneCollected[POneCollectedCount]:=POneCardDown;
  inc(POneCollectedCount);
  POneCollected[POneCollectedCount]:=PTwoCardDown;
  // Calculate new points and mark player one as winner of last turn
  POnePoints:=POnePoints+CardValues[POneCardDown]+CardValues[PTwoCardDown];
  POneLost:=False;
end;

// Used when player two collects cards
procedure TBriscolaMain.PTwoCollects;
begin
  inc(PTwoCollectedCount);
  PTwoCollected[PTwoCollectedCount]:=POneCardDown;
  inc(PTwoCollectedCount);
  PTwoCollected[PTwoCollectedCount]:=PTwoCardDown;
  PTwoPoints:=PTwoPoints+CardValues[POneCardDown]+CardValues[PTwoCardDown];
  POneLost:=True;
end;

// Used to draw new card after end of turn
procedure TBriscolaMain.TakeCards;
begin
  if AllCardsTaken=False then begin  // If game is not yet in phase two... (last six cards)
    if POneLost=True then begin
      PTwoTakeCard;  // Winner takes his card first
      if (cardsTakenFromDeck=cCardNum-1) then POneTakeBriscola  // If this is the last card in deck it means that briscola is taken
      else POneTakeCard;
    end else begin
      POneTakeCard;
      if (cardsTakenFromDeck=cCardNum-1) then PTwoTakeBriscola
      else PTwoTakeCard;
    end;
    // All cards were taken...
    if cardsTakenFromDeck=cCardNum then begin
      AllCardsTaken:=True;
      imBriscola.Picture:=nil;
    end;
  end else inc(LastSixCardsCount,2);
end;

// Used to take a card for player one
procedure TBriscolaMain.POneTakeCard;
begin
  inc(cardsTakenFromDeck);
  POneCardsInHand[POneCardDownIndex]:=ShuffledCards[cardsTakenFromDeck+1];
  // Display new card in hand
  case POneCardDownIndex of
    1:imlCards.GetBitmap(POneCardsInHand[POneCardDownIndex]-1,imPlayerOneCardOne.picture.bitmap);
    2:imlCards.GetBitmap(POneCardsInHand[POneCardDownIndex]-1,imPlayerOneCardTwo.picture.bitmap);
    3:imlCards.GetBitmap(POneCardsInHand[POneCardDownIndex]-1,imPlayerOneCardThree.picture.bitmap);
  end;
end;

// Used to take a card for player two
procedure TBriscolaMain.PTwoTakeCard;
begin
  inc(cardsTakenFromDeck);
  PTwoCardsInHand[PTwoCardDownIndex]:=ShuffledCards[cardsTakenFromDeck+1];
  case PTwoCardDownIndex of
    1:imlCards.GetBitmap(PTwoCardsInHand[PTwoCardDownIndex]-1,imPlayerTwoCardOne.picture.bitmap);
    2:imlCards.GetBitmap(PTwoCardsInHand[PTwoCardDownIndex]-1,imPlayerTwoCardTwo.picture.bitmap);
    3:imlCards.GetBitmap(PTwoCardsInHand[PTwoCardDownIndex]-1,imPlayerTwoCardThree.picture.bitmap);
  end;
end;

// End turn
procedure TBriscolaMain.EndTurn;
begin
  imPlayerOneCardDown.Picture:=nil;
  imPlayerTwoCardDown.Picture:=nil;
  // Initialize everything needed for next turn
  if LastSixCardsCount>0 then begin
    POneCardsInHand[POneCardDownIndex]:=0;
    PTwoCardsInHand[PTwoCardDownIndex]:=0;
  end;
  POneCardDown:=0;
  PTwoCardDown:=0;
  StatusBar.Panels[0].Text:=cPOnePointsPrefix+inttostr(POnePoints);
  StatusBar.Panels[1].Text:=cPTwoPointsPrefix+inttostr(PTwoPoints);
  if POneLost then StatusBar.Panels[2].Text:=cPTwoTurn
  else StatusBar.Panels[2].Text:=cPOneTurn;
  StatusBar.Panels[3].Text:=cCardsLeftPrefix+inttostr(cCardNum-cardsTakenFromDeck);
end;

// Initialize a new game of briscola
procedure TBriscolaMain.Initialize;
var i:integer;
begin
  CantPlay:=False;
  POneCardDown:=0;
  PTwoCardDown:=0;
  POneCardDownIndex:=0;
  PTwoCardDownIndex:=0;
  POneCollectedCount:=0;
  PTwoCollectedCount:=0;
  POnePoints:=0;
  PTwoPoints:=0;
  POneLost:=False;
  AllCardsTaken:=False;
  CardsTakenFromDeck:=0;
  LastSixCardsCount:=0;
  for i:=1 to cCardNum do begin
    POneCollected[i]:=0;
    PTwoCollected[i]:=0;
  end;
  StatusBar.Panels[0].Text:=cPOnePointsPrefix+inttostr(POnePoints);
  StatusBar.Panels[1].Text:=cPTwoPointsPrefix+inttostr(PTwoPoints);
  if POneLost then StatusBar.Panels[2].Text:=cPTwoTurn
  else StatusBar.Panels[2].Text:=cPOneTurn;
  StatusBar.Panels[3].Text:=cCardsLeftPrefix+inttostr(cCardNum-cardsTakenFromDeck);
end;

// Used when player one gets the briscola
procedure TBriscolaMain.POneTakeBriscola;
begin
  inc(cardsTakenFromDeck);
  POneCardsInHand[POneCardDownIndex]:=ShuffledCards[7];
  case POneCardDownIndex of
    1:imlCards.GetBitmap(POneCardsInHand[POneCardDownIndex]-1,imPlayerOneCardOne.picture.bitmap);
    2:imlCards.GetBitmap(POneCardsInHand[POneCardDownIndex]-1,imPlayerOneCardTwo.picture.bitmap);
    3:imlCards.GetBitmap(POneCardsInHand[POneCardDownIndex]-1,imPlayerOneCardThree.picture.bitmap);
  end;
end;

// Used when player two gets the briscola
procedure TBriscolaMain.PTwoTakeBriscola;
begin
  inc(cardsTakenFromDeck);
  PTwoCardsInHand[PTwoCardDownIndex]:=ShuffledCards[7];
  case PTwoCardDownIndex of
    1:imlCards.GetBitmap(PTwoCardsInHand[PTwoCardDownIndex]-1,imPlayerTwoCardOne.picture.bitmap);
    2:imlCards.GetBitmap(PTwoCardsInHand[PTwoCardDownIndex]-1,imPlayerTwoCardTwo.picture.bitmap);
    3:imlCards.GetBitmap(PTwoCardsInHand[PTwoCardDownIndex]-1,imPlayerTwoCardThree.picture.bitmap);
  end;
end;

// Determine if we have a winner and who it is?
function TBriscolaMain.DeclareWinner:boolean;
begin
  result:=False;
  if LastSixCardsCount=6 then begin  // All cards have been played
    if POnePoints>PTwoPoints then begin
      ShowMessage(cPOneWon);
      StatusBar.Panels[2].Text:=cPOneWon
    end;
    if POnePoints<PTwoPoints then begin
      ShowMessage(cPTwoWon);
      StatusBar.Panels[2].Text:=cPTwoWon
    end;
    if POnePoints=PTwoPoints then begin
      ShowMessage(cNoneWon);
      StatusBar.Panels[2].Text:=cNoneWon
    end;
    CantPlay:=True;
    Result:=True;
  end;
end;

// Determine who won the last turn
procedure TBriscolaMain.GiveCardsToWinner;
var low,high:integer;
    lowP,highP:integer;
begin
  // Initialize values that hold the range for the cards of same type as briscola card
  case Briscola of
    1..10:begin
            low:=1; high:=10;
          end;
    11..20:begin
            low:=11; high:=20;
          end;
    21..30:begin
            low:=21; high:=30;
          end;
    else begin
           low:=31; high:=40;
         end;
  end;
  // P1's card is of same type as briscola card
  if ((POneCardDown>=low)and(POneCardDown<=high)) then begin
    // P2's card is also of the same type as briscola card
    if ((PTwoCardDown>=low)and(PTwoCardDown<=high)) then begin
      // Check if P1 collects
      if CardValues[POneCardDown]>CardValues[PTwoCardDown] then POneCollects
      else begin
        // Both cards are of the same value (0 - zero)
        if CardValues[POneCardDown]=CardValues[PTwoCardDown] then begin
          if POneCardDown>PTwoCardDown then POneCollects
          else PTwoCollects;
        end else PTwoCollects; // P2's card has higher value than P1's card
      end;
    end else POneCollects;
  end else begin  // P1's card is not of the same type as briscola card
    // If P2's card is of the same type as briscola card he collects
    if ((PTwoCardDown>=low)and(PTwoCardDown<=high)) then PTwoCollects
    else begin
      if POneLost=True then begin  // P2 played first
        // Initialize internal values that determine in which range sholud P1's card be to win the turn
        case PTwoCardDown of
          1..10:begin
                  lowP:=1; highP:=10;
                end;
          11..20:begin
                   lowP:=11; highP:=20;
                 end;
          21..30:begin
                   lowP:=21; highP:=30;
                 end;
          else begin
                 lowP:=31; highP:=40;
               end;
        end;
        // P1's card is in of the same type as P2's card
        if ((POneCardDown>=lowP)and(POneCardDown<=highP)) then begin
          if CardValues[POneCardDown]>CardValues[PTwoCardDown] then POneCollects
          else begin
            if CardValues[POneCardDown]=CardValues[PTwoCardDown] then begin
              if POneCardDown>PTwoCardDown then POneCollects
              else PTwoCollects;
            end else PTwoCollects;
          end;
        end else PTwoCollects;
      end else begin  // P1 played first
        // Initialize internal values that determine in which range should P2's card be to win the turn
        case POneCardDown of
          1..10:begin
                  lowP:=1; highP:=10;
                end;
          11..20:begin
                   lowP:=11; highP:=20;
                 end;
          21..30:begin
                   lowP:=21; highP:=30;
                 end;
          else begin
                 lowP:=31; highP:=40;
               end;
        end;
        // P2's card is in of the same type as P1's card
        if ((PTwoCardDown>=lowP)and(PTwoCardDown<=highP)) then begin
          if CardValues[POneCardDown]>CardValues[PTwoCardDown] then POneCollects
          else begin
            if CardValues[POneCardDown]=CardValues[PTwoCardDown] then begin
              if POneCardDown>PTwoCardDown then POneCollects
              else PTwoCollects;
            end else PTwoCollects;
          end;
        end else POneCollects;
      end;
    end;
  end;
end;

// Used to save game flow into a file
procedure TBriscolaMain.AppendGameData(OpenFile, CloseOpenedFile, POne:boolean);
var i:integer;

  function Find(const Card:integer):boolean;
  var j:integer;
  begin
    result:=false;
    for j:=1 to 40 do begin
      if ((Card=POneCollected[j])or(Card=PTwoCollected[j])) then result:=True;
    end;
  end;

  // Which cards of the same type as briscola are still in game?
  procedure CardsInGame(const Range:integer);
  var j:integer;
  begin
    for j:=Range-9 to Range do begin
      if not Find(j) then write(dataFile,'t,')
      else write(dataFile,'f,');
    end;
  end;

begin
  if OpenFile then begin
    if FileExists(cGameDataFile) then begin
      //open file for appending
      AssignFile(dataFile, cGameDataFile);
      Append(dataFile);
    end else begin
      //create file
      AssignFile(dataFile, cGameDataFile);
      Rewrite(dataFile);
    end;
  end;
  if CloseOpenedFile then //close file
    CloseFile(dataFile);
  //write current data to file, we save data for player one (keep track on player one's play)
  if ((CloseOpenedFile=False)and(OpenFile=False)) then begin
    if POne=True then begin //player one's turn
      if POnewasFirst then write(dataFile,'t,') //player one was first
      else write(dataFile,'f,'); //player one played second
      for i:=1 to 3 do begin //cards in hand
        if POneCardsInHand[i]=0 then write(dataFile,'-1,')
        else write(dataFile,POneCardsInHand[i],',');
      end;
      if POneWasFirst then begin
        write(dataFile,'-1',','); //last card of P2 (has not done his move yet)
        write(dataFile,cCardNum-cardsTakenFromDeck+6-LastSixCardsCount,','); //number of cards in game
      end else begin
        write(dataFile,PTwoCardDown,',');
        write(dataFile,cCardNum-cardsTakenFromDeck+6-LastSixCardsCount-1,','); //number of cards in game
      end;
      write(dataFile,POnePoints,','); //points (P1)
      write(dataFile,PTwoPoints,','); //points (P2)
      write(dataFile,Briscola,','); //briscola
      case Briscola of
        1..10:CardsInGame(10);
        11..20:CardsInGame(20);
        21..30:CardsInGame(30);
        31..40:CardsInGame(40);
      end;
      write(dataFile,POneCardDownIndex);
      write(dataFile,endline);
    end else begin  //player two's turn
      if POnewasFirst then write(dataFile,'f,') //player two was second
      else write(dataFile,'t,'); //player one played first
      for i:=1 to 3 do begin //cards in hand
        if PTwoCardsInHand[i]=0 then write(dataFile,'-1,')
        else write(dataFile,PTwoCardsInHand[i],',');
      end;
      if POneWasFirst then begin
        write(dataFile,POneCardDown,','); //card that P1 played
        write(dataFile,cCardNum-cardsTakenFromDeck+6-LastSixCardsCount-1,','); //number of cards in game
      end else begin
        write(dataFile,'-1',',');
        write(dataFile,cCardNum-cardsTakenFromDeck+6-LastSixCardsCount,','); //number of cards in game
      end;
      write(dataFile,PTwoPoints,','); //points (P2)
      write(dataFile,PonePoints,','); //points (P2)
      write(dataFile,Briscola,','); //briscola
      case Briscola of
        1..10:CardsInGame(10);
        11..20:CardsInGame(20);
        21..30:CardsInGame(30);
        31..40:CardsInGame(40);
      end;
      write(dataFile,PTwoCardDownIndex);
      write(dataFile,endline);
    end;
  end;
end;

// Used when app closed with the X button
procedure TBriscolaMain.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
  AppendGameData(false,true,false);
  Application.Terminate;
end;

end.
