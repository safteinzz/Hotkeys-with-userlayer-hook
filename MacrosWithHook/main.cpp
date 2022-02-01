#define WINVER 0x0500
#include <windows.h>
#include <stdlib.h>

// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally
// or you will get problems since every function uses this variable.
HHOOK _hook;

// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code. (described later)
KBDLLHOOKSTRUCT kbdStruct;


HDC dng = GetDC(NULL);
const int posX = 1282;
const int posY = 1291;
const int pistolaBHigh = 40;
const int escopetaBLow = 180;
const int sniperGHigh = 60;
int g;
int b;

void SetHook();
void ReleaseHook();
void daleN();
void daleP();
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

void SetHook()
{
    // Set the hook and set it to use the callback function above.
    // WH_KEYBOARD_LL means it will set a low level keyboard hook. More information can be found on MSDN.
    // The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
    // function that sets and releases the hook. If you create a hack you will not need the callback function
    // in another place then your own code file anyway. Read more about it at MSDN.
    if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
    {
	// Display a messagebox that notifies the user of an error. At this point, the hook is NOT installed!
        MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
    }
}

void ReleaseHook()
{
    // Uninstall the hook you have set. If the hook wasn't yet installed then this function will fail ofcourse.
    UnhookWindowsHookEx(_hook);
}

// This is the callback function. Consider it the event that is raised when, in this case,
// a key is pressed.
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        // the action is valid: HC_ACTION.
        if (wParam == WM_KEYDOWN)
        {
            COLORREF c;
            c = GetPixel(dng,posX,posY);
            g = (int)GetGValue(c);
            b = (int)GetBValue(c);
            // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
            kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
            // a key (non-system) is pressed.
            //0x4E n
            //0x50 p
            switch(kbdStruct.vkCode)
            {
                case 0x39: //CAMBIO A PISTOLA
                    if(b < pistolaBHigh)return -1;
                    else if(b > escopetaBLow)daleN();
                    else if(g < sniperGHigh)daleP();
                    else break;
                    return -1;
                    break;
                case 0x43: //CAMBIO A ESCOPETA
                    if(b > escopetaBLow)return -1;
                    else if(b < pistolaBHigh)daleP();
                    else if(g < sniperGHigh)daleN();
                    else break;
                    return -1;
                    break;
                case 0x56: //CAMBIO A SNIPER
                    if(g < sniperGHigh)return -1;
                    else if(b > escopetaBLow)daleP();
                    else if(b < pistolaBHigh)daleN();
                    else break;
                    return -1;
                    break;
            }
        }
    }
    // call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}


int main()
{
    // Set the hook
    SetHook();
    // Don't mind this, it is a meaningless loop to keep a console application running.
    // I used this to test the keyboard hook functionality. If you want to test it, keep it in ;)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) { }
}

void daleN(){
    keybd_event(0x4E, 0x1C, KEYEVENTF_EXTENDEDKEY | 0, 0);
    Sleep(10);
    keybd_event(0x4E, 0x1C, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    return;
}
void daleP(){
//    SHORT key;
//    UINT mappedkey;
//    INPUT input = { 0 };
//    key = VkKeyScan('p');
//    mappedkey = MapVirtualKey(LOBYTE(key), 0);
//    input.type = INPUT_KEYBOARD;
//    input.ki.dwFlags = KEYEVENTF_SCANCODE;
//    input.ki.wScan = mappedkey;
//    SendInput(1, &input, sizeof(input));
//    Sleep(10);
//    input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
//    SendInput(1, &input, sizeof(input));
    keybd_event(0x50, 0x1C, KEYEVENTF_EXTENDEDKEY | 0, 0);
    Sleep(10);
    keybd_event(0x50, 0x1C, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    return;
}
