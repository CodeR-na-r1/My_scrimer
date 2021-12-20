#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

using namespace std;

float ChangeVolume(double nVolume, bool bScalar)
{
    HRESULT hr = NULL;
    bool decibels = false;
    bool scalar = false;
    double newVolume = nVolume;

    CoInitialize(NULL);
    IMMDeviceEnumerator* deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
    IMMDevice* defaultDevice = NULL;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    IAudioEndpointVolume* endpointVolume = NULL;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
    defaultDevice->Release();
    defaultDevice = NULL;

    // -------------------------
    float currentVolume = 0;
    endpointVolume->GetMasterVolumeLevel(&currentVolume);
    //printf("Current volume in dB is: %f\n", currentVolume);

    hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
    //CString strCur=L"";
    //strCur.Format(L"%f",currentVolume);
    //AfxMessageBox(strCur);

     // printf("Current volume as a scalar is: %f\n", currentVolume);
    if (bScalar == false)
    {
        hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
    }
    else if (bScalar == true)
    {
        hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
    }
    endpointVolume->Release();

    CoUninitialize();

    return currentVolume;
}

HWND GetConsoleHwnd()
{
    wstring title = L"Scrimer";
    SetConsoleTitle(title.c_str());
    Sleep(50);
    HWND hwnd = FindWindow(NULL, title.c_str());
    return hwnd;
}

unsigned long make_req(const wstring& path)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    DWORD dwProcessId = 0;
    DWORD dwThreadId = 0;

    ZeroMemory(&si, sizeof(si));
    si.wShowWindow = SW_HIDE;

    ZeroMemory(&pi, sizeof(pi));

    bool succes_creation_process = false;

    succes_creation_process = CreateProcess(path.c_str(), NULL, NULL, NULL, NULL, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    
    if (!succes_creation_process)
    {
        cout << "Error launch 'make_req' process!" << endl;
        return 0;
    }

    WaitForSingleObject(pi.hProcess, INFINITE); // ждем-с...

    unsigned long ExitCode;
    GetExitCodeProcess(pi.hProcess, &ExitCode);  // код завершения

    CloseHandle(pi.hProcess);  // не забываем закрывать дескрипторы
    CloseHandle(pi.hThread);

    return ExitCode;
}

int read_answ(const wstring& path)
{
    ifstream in(path);

    int answ;

    in >> answ;

    in.close();

    return answ;
}

int make_scrimer(const wstring& path)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    DWORD dwProcessId = 0;
    DWORD dwThreadId = 0;

    ZeroMemory(&si, sizeof(si));
    si.wShowWindow = SW_HIDE;

    ZeroMemory(&pi, sizeof(pi));

    bool succes_creation_process = false;

    succes_creation_process = CreateProcess(path.c_str(), NULL, NULL, NULL, NULL, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    if (!succes_creation_process)
    {
        cout << "Error launch 'make_scrimer' process!" << endl;
        return -1;
    }

    HWND hWindow = FindWindowEx(0, 0, 0, L"tk");

    while (!hWindow)
    {
        hWindow = FindWindowEx(0, 0, 0, L"tk");
    }
    LockSetForegroundWindow(2);
    SetForegroundWindow(GetConsoleHwnd());
    SetForegroundWindow(hWindow);
    float old_volume = ChangeVolume(1.0, true);
    
    int counter(0);
    while (counter < 10)
    {
        cout << SetForegroundWindow(hWindow) << endl;
        Sleep(100);
        ++counter;
    }

    PostMessage(hWindow, WM_CLOSE, NULL, NULL);
    ChangeVolume(old_volume, true);

    CloseHandle(pi.hProcess);  // не забываем закрывать дескрипторы
    CloseHandle(pi.hThread);

    return 0;
}

int main()
{
    long exit_code = 0;

    wstring path_make_requests = L"monit/monitoting_req.exe";
    wstring path_answ_file = L"answ.data";
    wstring path_scrimer = L"scrimer.exe";

    HWND hConsole = GetConsoleHwnd();
    ShowWindow(hConsole, SW_HIDE);

    while (true)
    {
        make_req(path_make_requests);

        exit_code = read_answ(path_answ_file);

        if (exit_code == 2)
            Sleep(1000);
        else if (exit_code == 3)
            Sleep(5000);
        else if (exit_code == 0)
            break;
        else if (exit_code == 1)
            make_scrimer(path_scrimer);
    }

    ShowWindow(hConsole, SW_SHOW);

    cout << "Programm finished!" << endl;

    getchar();

    return 0;
}