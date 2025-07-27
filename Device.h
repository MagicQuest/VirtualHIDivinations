/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include "public.h"

EXTERN_C_START

//extern GAMEPAD_REPORT;

typedef unsigned char      uint8_t;
typedef unsigned short WORD;
typedef unsigned long long QWORD;

#pragma pack(1)

typedef struct {
    BYTE id; //ReportId?

    WORD x;
    WORD y;
    WORD Rx;
    WORD Ry;
    WORD Rz;
    WORD z;

    WORD buttons;
} GAMEPAD_REPORT, *PGAMEPAD_REPORT;

//#define GAMEPAD_REPORT_SIZE 15
#define GAMEPAD_REPORT_SIZE sizeof(GAMEPAD_REPORT)

typedef struct {
    BYTE id; //ReportId?

    char x; //relative!
    char y;

    BYTE buttons;
} MOUSE_REPORT, * PMOUSE_REPORT;

//#define MOUSE_REPORT_SIZE 4
#define MOUSE_REPORT_SIZE sizeof(MOUSE_REPORT)

#define MAX_KEYS 6

typedef struct {
    BYTE id;

    BYTE modifierKeys;
    BYTE padding; //for no reason?

    BYTE keys[MAX_KEYS]; //im not sure... (wait i think this means i can press 6 keys at the same time and it will still work!)
} KEYBOARD_REPORT, *PKEYBOARD_REPORT;

#define KEYBOARD_REPORT_SIZE sizeof(KEYBOARD_REPORT)

#pragma pack()

//this actually works but to access any word you have to use .value :(
typedef union {
    struct { BYTE high, low; } word; //could be switched lol
    WORD value;
} WORD_AS_BYTES;

typedef struct {
    BYTE id; //ReportId?

    WORD_AS_BYTES x;
    WORD_AS_BYTES y;
    WORD_AS_BYTES Rx;
    WORD_AS_BYTES Ry;
    WORD_AS_BYTES Rz;
    WORD_AS_BYTES z;

    WORD_AS_BYTES buttons;
} UGAMEPAD_REPORT, * PUGAMEPAD_REPORT;

//#define UGAMEPAD_REPORT_SIZE sizeof(UGAMEPAD_REPORT)

#define GAMEPAD_MODE 0
#define MOUSE_MODE 1
#define KEYBOARD_MODE 2
#define GAMEPAD_MOUSE_MODE 3
#define ISOCRONOUS_MODE 4
#define MAX_SUPPORTED_MODES 4

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
//jarvis: https://learn.microsoft.com/en-us/windows-hardware/drivers/wdf/framework-object-context-space
typedef struct _DEVICE_CONTEXT
{
    WDFUSBDEVICE UsbDevice;
    WDFUSBINTERFACE UsbInterface;
    WDFUSBPIPE UsbPipe;
    size_t resHeaderLength;
    VHFHANDLE VhfHandle;
    //GAMEPAD_REPORT VhfHidReport;
    UCHAR VhfGamepadReport[GAMEPAD_REPORT_SIZE];
    UCHAR VhfMouseReport[MOUSE_REPORT_SIZE];
    UCHAR VhfKeyboardReport[KEYBOARD_REPORT_SIZE];
    HID_XFER_PACKET VhfGamepadReportPacket;
    HID_XFER_PACKET VhfMouseReportPacket;
    HID_XFER_PACKET VhfKeyboardReportPacket;
    UCHAR mode;
    //ULONG PrivateDeviceData;  // just a placeholder

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

//
// Function to initialize the device's queues and callbacks
//
NTSTATUS
VHIDCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

//
// Function to select the device's USB configuration and get a WDFUSBDEVICE
// handle
//
EVT_WDF_DEVICE_PREPARE_HARDWARE VHIDEvtDevicePrepareHardware;
EVT_WDF_DEVICE_D0_ENTRY VHIDEvtDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT VHIDEvtDeviceD0Exit;
EVT_WDF_USB_READER_COMPLETION_ROUTINE VHIDEvtUsbTargetPipeReadComplete;
EVT_WDF_DEVICE_CONTEXT_CLEANUP VHIDEvtDeviceContextCleanup;

EXTERN_C_END
