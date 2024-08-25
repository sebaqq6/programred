//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "trayicon.h"
#include "CGRID.h"
#include <OleCtrls.hpp>
#include <VCFI.hpp>
#include <AppEvnts.hpp>
#include "CSPIN.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdFTP.hpp>
#include <OleCtnrs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TTimer *HideWindowOnStart;
        TButton *Button1;
        TMemo *memo_console;
        TTimer *ProgramTimer;
        TEdit *editFLASH;
        TButton *buttonFLASHSet;
        TLabel *Label2;
        TButton *buttonANULUJ;
        TTimer *WindowVisibleControl;
        TCheckBox *checkWORK;
        TCSpinEdit *spinCheck;
        TLabel *Label1;
        TCSpinEdit *spinPause;
        TLabel *Label3;
        TCSpinEdit *spinMaxFile;
        TLabel *Label4;
        TMemo *ext_list;
        TLabel *Label5;
        TButton *buttonSaveExt;
        TIdHTTP *HTTP;
        TIdFTP *FTP;
        TMemo *queue_ftp_send;
        TLabel *Label6;
        TTimer *T_FTPSend;
        TMemo *banlist;
        TLabel *Label7;
        TLabel *Label8;
        TCSpinEdit *spinFTPTimer;
        void __fastcall HideWindowOnStartTimer(TObject *Sender);
        void __fastcall ProgramTimerTimer(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall buttonFLASHSetClick(TObject *Sender);
        void __fastcall DownloadDir(String Dir, String sciezka_local);
        void __fastcall buttonANULUJClick(TObject *Sender);
        void __fastcall WindowVisibleControlTimer(TObject *Sender);
        void __fastcall checkWORKClick(TObject *Sender);
        void __fastcall spinCheckChange(TObject *Sender);
        void __fastcall spinCheckKeyPress(TObject *Sender, char &Key);
        void __fastcall spinPauseChange(TObject *Sender);
        void __fastcall spinPauseKeyPress(TObject *Sender, char &Key);
        void __fastcall spinMaxFileKeyPress(TObject *Sender, char &Key);
        void __fastcall spinMaxFileChange(TObject *Sender);
        void __fastcall buttonSaveExtClick(TObject *Sender);
        void __fastcall FTPConnected(TObject *Sender);
        void __fastcall FTPDisconnected(TObject *Sender);
        void __fastcall ListStorage(String Dir);
        void __fastcall FTPSendTimer(TObject *Sender);
        void __fastcall spinFTPTimerKeyPress(TObject *Sender, char &Key);
        void __fastcall spinFTPTimerChange(TObject *Sender);
private:	// User declarations

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
