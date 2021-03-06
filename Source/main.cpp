#ifndef OS_IOS
	#ifndef _DLL
	#ifndef BUILD_STATICLIB
		#include "App.h"
		#include "Common.hpp"
		#include "Utilities/Macros.hpp"				
	#endif
#endif

#pragma once
#include "Leadwerks.h"
using namespace Leadwerks;

App*			gApp;
AppController*	gAppCtrl;

void DebugErrorHook(char* c)
{
	int n = 0;//<--------------------------- Add a breakpoint here to catch errors
}

#ifdef __APPLE__
int main_(int argc, const char *argv[])
{
#else
int main(int argc, const char *argv[])
{
#endif

	/*#ifdef _WIN32
	//Enable low-fragmentation heap
	HANDLE heaps[1025];
	DWORD nheaps = GetProcessHeaps((sizeof(heaps) / sizeof(HANDLE)) - 1, heaps);
	for (DWORD i = 0; i < nheaps; ++i) {
	ULONG  enableLFH = 2;
	HeapSetInformation(heaps[i], HeapCompatibilityInformation, &enableLFH, sizeof(enableLFH));
	}
	#endif*/

	//Load saved settings
	std::string settingsfile = std::string(argv[0]);
	settingsfile = FileSystem::StripAll(settingsfile);
	if (String::Right(settingsfile, 6) == ".debug") settingsfile = String::Left(settingsfile, settingsfile.length() - 6);
	System::AppName = settingsfile;
	std::string settingsdir = FileSystem::GetAppDataPath();
#ifdef __linux__
#ifndef __ANDROID__
	settingsdir = settingsdir + "/." + String::Lower(settingsfile);
#else
	settingsdir = settingsdir + "/" + settingsfile;
#endif
#else
	settingsdir = settingsdir + "/" + settingsfile;
#endif
	if (FileSystem::GetFileType(settingsdir) == 0) FileSystem::CreateDir(settingsdir);
	settingsfile = settingsdir + "/" + settingsfile + ".cfg";
	System::LoadSettings(settingsfile);

	//Set program path
	System::AppPath = FileSystem::ExtractDir(argv[0]);

	//Load command-line parameters
	System::ParseCommandLine(argc, argv);

	//Enable Lua sandboxing
	if (String::Int(System::GetProperty("sandbox")) != 0) Interpreter::sandboxmode = true;

	//Switch directory
	std::string gamepack = System::GetProperty("game");
	if (gamepack != "")
	{
		Package* package = Package::Load(gamepack);
		if (!package) return 1;
	}

	//Add debug hook for catching errors
	Leadwerks::System::AddHook(System::DebugErrorHook, (void*)DebugErrorHook);

	//Load any zip files in main directory
	Leadwerks::Directory* dir = Leadwerks::FileSystem::LoadDir(".");
	if (dir)
	{
		unsigned index = 0;
		while (index < dir->files.size()) {
			std::string file = dir->files[index];
			std::string ext = Leadwerks::String::Lower(Leadwerks::FileSystem::ExtractExt(file));
			if (ext == "zip" || ext == "pak")
			{
				Leadwerks::Package::Load(file);
			}

			index += 1;
		}
		delete dir;
	}

#ifdef DEBUG
	std::string debuggerhostname = System::GetProperty("debuggerhostname");
	if (debuggerhostname != "")
	{
		//Connect to the debugger
		int debuggerport = String::Int(System::GetProperty("debuggerport"));
		if (!Interpreter::Connect(debuggerhostname, debuggerport))
		{
			Print("Error: Failed to connect to debugger with hostname \"" + debuggerhostname + "\" and port " + String(debuggerport) + ".");
			return false;
		}
		Print("Successfully connected to debugger.");
		std::string breakpointsfile = System::GetProperty("breakpointsfile");
		if (breakpointsfile != "")
		{
			if (!Interpreter::LoadBreakpoints(breakpointsfile))
			{
				Print("Error: Failed to load breakpoints file \"" + breakpointsfile + "\".");
			}
		}
	}
	else
	{
		//    Print("No debugger hostname supplied in command line.");
	}
#endif

	/*if (FileSystem::GetFileType("Scripts/main.lua") == 1)
	{
	//Execute main script file
	if (Interpreter::ExecuteFile("Scripts/main.lua"))
	{
	#ifdef DEBUG
	Interpreter::Disconnect();
	#endif
	if (!System::SaveSettings(settingsfile)) System::Print("Error: Failed to save settings file \"" + settingsfile + "\".");
	Steamworks::Shutdown();
	return 0;
	}
	else
	{
	#ifdef DEBUG
	Interpreter::Disconnect();
	#endif
	Steamworks::Shutdown();
	return 1;
	}
	}
	else
	{*/
	//Execute mobile-style App script		

	gApp = new App();
	gAppCtrl = new AppController(gApp);
	bool bRunning = gAppCtrl->Initialize("Leadwerks Template Project", 0, 0, 1024, 768, Leadwerks::Window::Titlebar, 0);	

	if (bRunning) 
    {
		float deltaTime = 1.0f;
		
		do {
			Leadwerks::Time::Update(60);
			deltaTime = Leadwerks::Time::GetSpeed();

			bRunning = gAppCtrl->Update(deltaTime);
			
			gAppCtrl->Render();

			gAppCtrl->Draw();

		} while (bRunning);

#ifdef DEBUG
		Interpreter::Disconnect();
#endif
		// < Shutdown application controller.		
		SAFE_DELETE(gAppCtrl);
		SAFE_DELETE(gApp);		

        // < Shutdown the Steamworks system.
        Steamworks::Shutdown();

		//Save settings
		if (!System::SaveSettings(settingsfile)) System::Print("Error: Failed to save settings file \"" + settingsfile + "\".");
		System::Shutdown();
		return 0;
	}
	else
	{
#ifdef DEBUG
		Interpreter::Disconnect();
#endif
		Steamworks::Shutdown();
		return 1;
	}
	//}
}
#endif
