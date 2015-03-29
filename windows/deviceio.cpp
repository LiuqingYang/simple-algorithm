
#include "stdafx.h"
#include <Windows.h> 
#include <SetupAPI.h> 
#include <stdio.h> 
#include <devguid.h> 
#include <RegStr.h> 
#include <iostream>
using namespace std;

BOOL EnumDeviceInterface(const GUID * InterfaceClassGUID){ 
	HDEVINFO DeviceInfoSet; 
	HDEVINFO NewDeviceInfoSet; 
	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData; 
	PSP_DEVICE_INTERFACE_DETAIL_DATA lpDeviceInterfaceDetailData; 
	DWORD dwBufferSize = 0; 
	DWORD i; 

	DeviceInfoSet = SetupDiCreateDeviceInfoList(NULL,NULL); 
	if(DeviceInfoSet == INVALID_HANDLE_VALUE){ 
		printf("Create device interface failed.(%d)\n",GetLastError()); 
		return 0; 
	} 
	NewDeviceInfoSet = SetupDiGetClassDevsEx(InterfaceClassGUID,NULL,NULL,DIGCF_DEVICEINTERFACE|DIGCF_PRESENT,DeviceInfoSet,NULL,NULL); 
	if(NewDeviceInfoSet == INVALID_HANDLE_VALUE){ 
		printf("Get device interface infomation failed."); 
		return 0; 
	} 
	DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData); 
	for(i=0;;i++){ 
		BOOL bResult = SetupDiEnumDeviceInterfaces(NewDeviceInfoSet,NULL,InterfaceClassGUID,i,&DeviceInterfaceData); 
		if(!bResult){ 
			if(bResult != NO_ERROR && GetLastError()!=ERROR_NO_MORE_ITEMS){ 
				printf("ERROR:%d\n",GetLastError()); 
				return FALSE; 
			} 
			break; 
		}else{ 
			SetupDiGetDeviceInterfaceDetail(NewDeviceInfoSet, &DeviceInterfaceData, NULL, NULL, &dwBufferSize, NULL);
			if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
			{
				printf("Error:%d\n", GetLastError());
			}

			lpDeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapAlloc(GetProcessHeap(), 0, sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + dwBufferSize);
			lpDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			if (!SetupDiGetDeviceInterfaceDetail(NewDeviceInfoSet, &DeviceInterfaceData, lpDeviceInterfaceDetailData, dwBufferSize, &dwBufferSize, NULL)){
				printf("Error:%d\n", GetLastError());
			}
			printf("DevicePath:%S\n",lpDeviceInterfaceDetailData->DevicePath); 
			//wcout.imbue(locale("chs"));
			//wcout << L"DevicePath:" << lpDeviceInterfaceDetailData->DevicePath << endl;
			HeapFree(GetProcessHeap(),0,lpDeviceInterfaceDetailData); 
		} 
	} 
	return TRUE; 
} 

int main(){ 
	printf("Get device interface information.\n"); 
	EnumDeviceInterface(&GUID_DEVINTERFACE_VOLUME); 
	system("pause");
	return 0; 
} 

