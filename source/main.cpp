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

typedef int (*FnOff)(int);

int wmain() {
	HINSTANCE hXInputDLL = LoadLibraryA("XInput1_3.dll");
	if (hXInputDLL == NULL) {
		cout << "ERROR: Unable to load XInput1_3.dll!" << endl;
		fgetc(stdin);
		return 1;
	}
	FnOff pOff;
	pOff = FnOff(GetProcAddress(hXInputDLL, reinterpret_cast<char*>(103)));

	unsigned result;
	for (short i = 0; i < XUSER_MAX_COUNT; ++i) {
		XINPUT_STATE state;
		memset(&state, 0, sizeof(XINPUT_STATE));

		result = XInputGetState(i, &state);

		if(result == ERROR_SUCCESS) {
			cout << "Controller " << i << " is on. (" << state.dwPacketNumber << ")" << endl;
			XINPUT_BATTERY_INFORMATION batinfo;
			memset(&batinfo, 0, sizeof(XINPUT_BATTERY_INFORMATION));
			result = XInputGetBatteryInformation(i, BATTERY_DEVTYPE_GAMEPAD, &batinfo);

			if(result == ERROR_SUCCESS) {
				cout << " Battery Info: ";
				switch(batinfo.BatteryType) {
				case BATTERY_TYPE_WIRED:
					cout << "Wired ";
					break;
				case BATTERY_TYPE_ALKALINE:
					cout << "Alkaline ";
					break;
				case BATTERY_TYPE_NIMH:
					cout << "Nickel Metal Hydride ";
					break;
				case BATTERY_TYPE_UNKNOWN:
				default:
					cout << "Unknown ";
				}

				switch(batinfo.BatteryLevel) {
				case BATTERY_LEVEL_EMPTY:
					cout << "[   ]" << endl;
					break;
				case BATTERY_LEVEL_LOW:
					cout << "[-  ]" << endl;
					break;
				case BATTERY_LEVEL_MEDIUM:
					cout << "[-- ]" << endl;
					break;
				case BATTERY_LEVEL_FULL:
					cout << "[---]" << endl;
					break;
				default:
					cout << "[???]" << endl;
				}
			}
			pOff(i);
		}
		else {
			cout << "Controller " << i << " is not connected." << endl;			
		}
	}
	
	FreeLibrary(hXInputDLL);
	return 0;
}