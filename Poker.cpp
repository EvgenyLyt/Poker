//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("UnMain.cpp", MainForm);
USEFORM("UnRules.cpp", FormRules);
USEFORM("UnWarning.cpp", FormExit);
USEFORM("UnCombinations.cpp", FormCombinations);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TFormExit), &FormExit);
		Application->CreateForm(__classid(TFormCombinations), &FormCombinations);
		Application->CreateForm(__classid(TFormRules), &FormRules);
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
