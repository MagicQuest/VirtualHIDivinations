/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that app can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_VirtualHIDivinations,
    0x941b6880,0xa120,0x4a9a,0x9a,0x6b,0x4b,0x40,0xd1,0xe6,0xfc,0x5d);
// {941b6880-a120-4a9a-9a6b-4b40d1e6fc5d}
