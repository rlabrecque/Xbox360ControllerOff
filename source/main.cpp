/*
* Copyright 2011 Riley Labrecque. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are
* permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of
* conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, this list
* of conditions and the following disclaimer in the documentation and/or other materials
* provided with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY RILEY LABRECQUE ''AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the
* authors and should not be interpreted as representing official policies, either expressed
* or implied, of Riley Labrecque.
*/

#include <windows.h>
#include <iostream>

// We use the June DirectX SDK version to get XInput 1.3.
// The Windows 8 SDK only ships with 1.4 (Win8 only) and 9.1.0 (Does not have the function that we require).
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XInput.h"
#pragma comment(lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/XInput.lib")

using std::cout;
using std::endl;

static const char g_szBatteryLevel[][6] = { "[   ]", "[-  ]", "[-- ]", "[---]" };
typedef int (*g_pfnPowerOffController)(int);

int wmain() {
	HMODULE hXInputDLL = LoadLibraryA("XInput1_3.dll");
	if (hXInputDLL == nullptr) {
		cout << "ERROR: Unable to load XInput1_3.dll!" << endl;
		fgetc(stdin);
		return 1;
	}
	g_pfnPowerOffController XInputPowerOffController;
	XInputPowerOffController = g_pfnPowerOffController(GetProcAddress(hXInputDLL, reinterpret_cast<char*>(103)));

	unsigned result;
	for (short i = 0; i < XUSER_MAX_COUNT; ++i) {
		XINPUT_STATE state = {};

		result = XInputGetState(i, &state);

		if(result == ERROR_SUCCESS) {
			cout << "Controller " << i << " is on." << endl;
			XINPUT_BATTERY_INFORMATION batteryinfo = {};
			result = XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &batteryinfo);

			if(result == ERROR_SUCCESS) {
				cout << " Battery Info: ";

				cout << g_szBatteryLevel[batteryinfo.BatteryLevel];

				// I do not like this switch but I can not map BATTERY_TYPE_* to a char array easily.
				switch(batteryinfo.BatteryType) {
				case BATTERY_TYPE_WIRED:
					cout << " Wired." << endl;
					break;
				case BATTERY_TYPE_ALKALINE:
					cout << " Alkaline." << endl;
					XInputPowerOffController(i);
					break;
				case BATTERY_TYPE_NIMH:
					cout << " Nickel Metal Hydride." << endl;
					XInputPowerOffController(i);
					break;
				case BATTERY_TYPE_UNKNOWN:
				default:
					cout << " Unknown." << endl;
				}
			}
		}
		else {
			cout << "Controller " << i << " is not connected." << endl;			
		}
	}
	
	FreeLibrary(hXInputDLL);
	return 0;
}