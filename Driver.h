/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/
#ifdef _DEBUG
    #define __int3() __debugbreak()
#else
    #define __int3()
#endif

#include <ntddk.h>
#include <wdf.h>
#include <usb.h>
#include <usbdlib.h>
#include <wdfusb.h>
#include <initguid.h>
#include <vhf.h>

//#include "reports.h"
#include "device.h"
#include "queue.h"
#include "trace.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD VHIDEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP VHIDEvtDriverContextCleanup;

EXTERN_C_END
