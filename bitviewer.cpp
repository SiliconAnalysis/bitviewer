//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("MainFrm.cpp", MainForm);
USEFORM("..\bitract\ExportFrm.cpp", ExportForm);
USEFORM("..\bitract\HexFrm.cpp", HexForm);
USEFORM("..\bitract\SelPatFrm.cpp", SelPatForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "Bitviewer: The ROM bit manipulation tool";
         Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->CreateForm(__classid(TExportForm), &ExportForm);
         Application->CreateForm(__classid(THexForm), &HexForm);
         Application->CreateForm(__classid(TSelPatForm), &SelPatForm);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
