/*++

Module Name:

    queue.h

Abstract:

    This file contains the queue definitions.

Environment:

    Kernel-mode Driver Framework

--*/

EXTERN_C_START

#define VHID_FUNCTION_NEXT_MODE 0x800
#define VHID_FUNCTION_SET_MODE 0x801
#define VHID_FUNCTION_GET_MODE 0x802

//Input: N/A
//Output: [OPTIONAL] (UCHAR mode)
#define IOCTL_VHID_NEXT_MODE CTL_CODE(FILE_DEVICE_CONTROLLER,        \
                                      VHID_FUNCTION_NEXT_MODE, \
                                      METHOD_BUFFERED,               \
                                      FILE_WRITE_DATA | FILE_READ_DATA)

//Input: (UCHAR mode)
//Output: N/A
#define IOCTL_VHID_SET_MODE CTL_CODE(FILE_DEVICE_CONTROLLER,         \
                                      VHID_FUNCTION_SET_MODE,  \
                                      METHOD_BUFFERED,               \
                                      FILE_WRITE_DATA)

//Input: N/A
//Output: (UCHAR mode)
#define IOCTL_VHID_GET_MODE CTL_CODE(FILE_DEVICE_CONTROLLER,         \
                                      VHID_FUNCTION_GET_MODE,  \
                                      METHOD_BUFFERED,               \
                                      FILE_READ_DATA)

//
// This is the context that can be placed per queue
// and would contain per queue information.
//
typedef struct _QUEUE_CONTEXT {

    ULONG PrivateDeviceData;  // just a placeholder

} QUEUE_CONTEXT, *PQUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, QueueGetContext)

NTSTATUS
VHIDQueueInitialize(
    _In_ WDFDEVICE Device
    );

//
// Events from the IoQueue object
//
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL VHIDEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP VHIDEvtIoStop;

EXTERN_C_END
