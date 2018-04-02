//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Briscola.res");
USEFORM("BriscolaMain.cpp", frmBriscola);
USEFORM("Dialog.cpp", SetDialog);
USEFORM("About.cpp", AboutBox);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        Application->Initialize();
        Application->Title = "Briscola Game";
        Application->CreateForm(__classid(TfrmBriscola), &frmBriscola);
        Application->CreateForm(__classid(TSetDialog), &SetDialog);
        Application->CreateForm(__classid(TAboutBox), &AboutBox);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
