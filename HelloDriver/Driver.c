// I don't like NTSTATUS, it like shit
typedef NTSTATUS _nt;

/*
your driver must implement two basic event callback fucnions at least
which is:
 - DRIVER_INITIALIZE DriverEntry
 - EVT_WDF_DRIVER_DEVICE_ADD KmdfHelloWorldEvtDeviceAdd
*/

// these two head files must be included in your driver code
// contains win kernel definitions for all drivers
#include <ntddk.h>	
// contains win kernel definitions for drivers those who are based on WDF
#include <wdf.h>

// both DRIVER_INITIALIZE and EVT_WDF_DRIVER_DEVICE_ADD are function type with parameter and return value declared, see this:  https://blog.csdn.net/ma_de_hao_mei_le/article/details/126246225
DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD KmdfHelloWorldEvtDeviceAdd;

// then DriverEntry function
// this is the main function in user mode code
// everytime we want to analyse a drvier, we need to check this function to know how does it work
_nt DriverEntry(
	_In_ PDRIVER_OBJECT		DriverObject,
	_In_ PUNICODE_STRING	RegistryPath
)
{
	// this is the return value
	// well STATUS_SUCCESS macro sucks, it is too long, I prefer to use 0
	_nt status = 0;

	// idk what the fuck is this
	// the document from MS says it is driver configuration object, which indeed is a struct
	WDF_DRIVER_CONFIG config;

	// let's print some thing, only visible for debugger
	// idk what is the usage of commentid and level
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KmdfHelloWorld: DriverEntry\n"));

	// initialize driver configuration object to register entry point for one of the call back function we mentioned at the beginning
	// the other call back is EntryPoint
	// it doesn't need to be registered
	// it will be called automatically if certain event comes
	WDF_DRIVER_CONFIG_INIT(&config,
		KmdfHelloWorldEvtDeviceAdd);

	// at last, we create driver object
	status = WdfDriverCreate(
		DriverObject,
		RegistryPath,
		WDF_NO_OBJECT_ATTRIBUTES,
		&config,
		WDF_NO_HANDLE);
	return status;
}

// now we need to implement our KmdfHelloWorldEvtDeviceAdd callback function
// EvtDeviceAdd will be invoked by system when your device has arrived
// what does the doc mean when it say "your device"?
// what the fuck is "your device"? 
// idk what is my device and how it arrives
// this function will initialize structures and resources that this device will use, and create device object for it
// MS doc recommends to name this EvtDeviceAdd function with a prefix, and the driver name should be this prefix
_nt KmdfHelloWorldEvtDeviceAdd(
	_In_	WDFDRIVER			Driver,
	_Inout_ PWDFDEVICE_INIT		DeviceInit
)
{
	// the first parameter is useless because we're not going to use it
	// so we need to mark it as unreferenced
	// I'm not sure what is the point to do this
	// because I've never seen this in user mode C code
	UNREFERENCED_PARAMETER(Driver);

	// allocate device object
	// here is how WDFDEVICE is declared https://blog.csdn.net/ma_de_hao_mei_le/article/details/126247434
	WDFDEVICE hDevice;

	// print some thing in debugger so we know that this function is executed
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KmdfHelloWorld: KmdfHelloWorldEvtDeviceAdd\n"));

	_nt status = WdfDeviceCreate(
		&DeviceInit,
		WDF_NO_OBJECT_ATTRIBUTES,
		&hDevice);
	return status;
}