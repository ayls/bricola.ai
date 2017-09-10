//---------------------------------------------------------------------------
#ifndef DialogH
#define DialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
char* __fastcall ReadUseCombFinalPlayFromRegistry(void);
char* __fastcall ReadUseRulesFromRegistry(void);
char* __fastcall ReadFileNameFromRegistry(void);
char* __fastcall ReadCardsFileFromRegistry(void);
boolean __fastcall AreSettingsInRegistry(void);
void __fastcall WriteToRegistry(char* userules,char* filename,char* cardsfile,char* useCombPlay);
//---------------------------------------------------------------------------
class TSetDialog : public TForm
{
__published:	// IDE-managed Components
    TCheckBox *cbUseRules;
    TLabel *lbFileName;
    TButton *btnApply;
    TButton *btnClose;
    TEdit *edFileName;
    TLabel *lbCardFile;
    TEdit *edCardsFile;
    TCheckBox *cbUseCombPlay;
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall btnApplyClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TSetDialog(TComponent* Owner);
  boolean DoCardsExist(void);
  boolean DoRulesExist(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TSetDialog *SetDialog;
//---------------------------------------------------------------------------
#endif
