//---------------------------------------------------------------------------
#include <vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "Dialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSetDialog *SetDialog;
//---------------------------------------------------------------------------
__fastcall TSetDialog::TSetDialog(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSetDialog::btnCloseClick(TObject *Sender)
{
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
boolean __fastcall AreSettingsInRegistry(void)
{
  TRegistry *reg=new TRegistry();
  boolean temp=true;

  reg->OpenKey("Software\\Briscola",true);
  if (reg->ValueExists("UseRules")==false) temp=false;
  if (reg->ValueExists("FileName")==false) temp=false;
  if (reg->ValueExists("CardsDLL")==false) temp=false;
  delete reg;
  return(temp);
}
//---------------------------------------------------------------------------
void __fastcall WriteToRegistry(char* userules,char* filename,char* cardsfile,char* useCombPlay)
{
  TRegistry *reg=new TRegistry();

  reg->OpenKey("Software\\Briscola",true);
  reg->WriteString("UseRules",userules);
  reg->WriteString("FileName",filename);
  reg->WriteString("CardsDLL",cardsfile);
  reg->WriteString("CombinatoricalPlay",useCombPlay);
  delete reg;
}
//---------------------------------------------------------------------------
char* __fastcall ReadUseCombFinalPlayFromRegistry(void)
{
  TRegistry *reg=new TRegistry();
  char temp[30]="";

  reg->OpenKey("Software\\Briscola",true);
  strcpy(temp,reg->ReadString("CombinatoricalPlay").c_str());
  delete reg;
  return(temp);
}
//---------------------------------------------------------------------------
char* __fastcall ReadFileNameFromRegistry(void)
{
  TRegistry *reg=new TRegistry();
  char temp[30]="";

  reg->OpenKey("Software\\Briscola",true);
  strcpy(temp,reg->ReadString("FileName").c_str());
  delete reg;
  return(temp);
}
//---------------------------------------------------------------------------
char* __fastcall ReadUseRulesFromRegistry(void)
{
  TRegistry *reg=new TRegistry();
  char temp[2]="";

  reg->OpenKey("Software\\Briscola",true);
  strcpy(temp,reg->ReadString("UseRules").c_str());
  delete reg;
  return(temp);
}
//---------------------------------------------------------------------------
char* __fastcall ReadCardsFileFromRegistry(void)
{
  TRegistry *reg=new TRegistry();
  char temp[30]="";

  reg->OpenKey("Software\\Briscola",true);
  strcpy(temp,reg->ReadString("CardsDLL").c_str());
  delete reg;
  return(temp);
}
//---------------------------------------------------------------------------
boolean TSetDialog::DoCardsExist(void)
{
  return(FileExists(edCardsFile->Text));
}
//---------------------------------------------------------------------------
boolean TSetDialog::DoRulesExist(void)
{
  char tempFileName1[30]="", tempFileName2[30]="";

  strcat(tempFileName1,edFileName->Text.c_str());
  strcat(tempFileName1,".*");
  if (!FileExists(tempFileName1)) {
    ShowMessage("File set does not exist!");
    return(false);
  }
  if (cbUseRules->Checked==true) {
    strcat(tempFileName2,edFileName->Text.c_str());
    strcat(tempFileName2,".rules");
    if (!FileExists(tempFileName2)) {
      ShowMessage("Rules file does not exist!");
      return(false);
    }
  }
  return(true);
}
//---------------------------------------------------------------------------
void __fastcall TSetDialog::btnApplyClick(TObject *Sender)
{
  if (DoCardsExist()==true) {
    if (DoRulesExist()==true) {
      if (cbUseRules->Checked) {
        if (cbUseCombPlay->Checked) WriteToRegistry("1",edFileName->Text.c_str(),edCardsFile->Text.c_str(),"1");
        else WriteToRegistry("1",edFileName->Text.c_str(),edCardsFile->Text.c_str(),"0");
      }
      else {
        if (cbUseCombPlay->Checked) WriteToRegistry("0",edFileName->Text.c_str(),edCardsFile->Text.c_str(),"1");
        else WriteToRegistry("0",edFileName->Text.c_str(),edCardsFile->Text.c_str(),"0");
      }
      ModalResult=mrOk;
    }
  }
  else ShowMessage("Cards DLL does not exist!");
}
//---------------------------------------------------------------------------

void __fastcall TSetDialog::FormShow(TObject *Sender)
{
  char userules[2]="",filename[30]="",cardsfile[30]="",useCombPlay[2]="";

  strcpy(userules,ReadUseRulesFromRegistry());
  strcpy(filename,ReadFileNameFromRegistry());
  strcpy(cardsfile,ReadCardsFileFromRegistry());
  strcpy(useCombPlay,ReadUseCombFinalPlayFromRegistry());
  if (strcmp(userules,"1")==0) cbUseRules->Checked=true;
  else cbUseRules->Checked=false;
  if (strcmp(useCombPlay,"1")==0) cbUseCombPlay->Checked=true;
  else cbUseCombPlay->Checked=false;
  edFileName->Text=filename;
  edCardsFile->Text=cardsfile;    
}
//---------------------------------------------------------------------------

