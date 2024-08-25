//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Unit1.cpp", Form1);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    	HANDLE mtx;
    	mtx = OpenMutex (MUTEX_ALL_ACCESS, false, "RScriptBySebaqq6");
        if (mtx == NULL)
        {
            mtx = CreateMutex (NULL, true, "RScriptBySebaqq6");
        }
        else
        {
            Application->MessageBox("Tylko jedna kopia programu może pracować ;*", "Błąd - kopia programu pracuje!", MB_OK | MB_ICONWARNING);
            return 0;
        }
        try
        {
                 Application->Initialize();
                 Application->Title = "RedScript by sebaqq6";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 //Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         //throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         //Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
