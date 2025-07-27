# VirtualHIDivinations (see bottom for use)
a kmdf driver to turn a raphnet adapter device into a keyboard or mouse by using a [virtual HID device](https://learn.microsoft.com/en-us/windows-hardware/drivers/hid/virtual-hid-framework--vhf-)

I use a continuous reader to read hid reports from the device and then use that data to send a keyboard and/or mouse inputs (it can also just send the regular gamepad report too)

> *"This time -- this time, I TOLD you, I told you -- we weren't done with drivers" - 7/7/25*

## Story
 Back in september of LAST year (2024), I had the brilliant idea to attempt to emulate an hid device. I wanted to use my controller like a keyboard.

 BOY was that 1000 times harder than it sounded at the time. Really, the main problem was understanding how to write a driver (and really what drivers did in general). The "emulating an hid device" part was actually pretty easy with [VHF](https://learn.microsoft.com/en-us/windows-hardware/drivers/hid/virtual-hid-framework--vhf-).

 Anyways, I didn't understand anything back then and I couldn't figure out how I could easily emulate an HID device. Another thing that also contributed to me giving up was the fact that provisioning my (virtual box) test computer **never** worked. (more about that later)

## Round 2 - July of 2025
 By this time I knew I was ready for round 2 (up to this point i had done NO research and i just figured it was time to try again)
 
 This time I caved and got the [driver development book](https://learn.microsoft.com/en-us/windows-hardware/drivers/wdf/developing-drivers-with-wdf) (ISBN: 9780735645851) that they mention in the docs and I also stumbled upon an explanation page for the [kmdf usb project template](https://learn.microsoft.com/en-us/windows-hardware/drivers/usbcon/understanding-the-kmdf-template-code-for-usb).
 
 I spent a good week upping my knowledge through the book and other places. Since I wanted to read from the device using USB and wanted to send hid reports, I had to learn about these things too.

 I read a little of the [USB hid spec](https://www.usb.org/sites/default/files/hid1_12.pdf) to learn how USB devices can tell the OS that it is, in fact, a HID. I also read the [hid usage tables spec](https://usb.org/sites/default/files/hut1_6.pdf) to understand report descriptors.

## Consistent problems
 Provisioning my (virtual box) test machine for deployment **never** worked. Not once. Even if i had the same version of windows AND the WDK, it would never complete successfully. Foolishly, I thought that it HAD to be successful for deployment to work. (If you're wondering how to get your virtual machine on the same version of windows as your main computer, download and run the [media creation tool](https://www.microsoft.com/en-us/software-download/windows11) on your main computer)
 
 helpful affirmation:
 (**even though provisioning was never successful, i am able to deploy my driver**)

 Deploying the driver also never completes without at least 1 failure but the driver DOES get loaded so it's good enough (it seems like the driver removal doesn't work, unless you're supposed to unplug/uninstall the device first)

## What I've learned
 Now that i've written two drivers (at the time of committing this), i'll share somethings that I think are important for drivers in general if you are interested in writing one. 
 
 (**some of the things i say here could be wrong and all of these tips apply mostly to kmdf function drivers**)

 * Most objects usually have a related config structure that's required for the creation of that object. Objects can also have attributes like object-specific data (typically called a context space).
 * Every driver needs to create a driver object and a device object
    * Driver objects should be created in `DriverEntry` - this object represents the driver.
    * Device objects should be created in `EvtDeviceAdd` - they represent the devices your driver controls.
 * Your driver needs a device interface (maybe?) if you want applications to be able to interact with your driver. (i.e receiving **IOCTLs**)
   * For example, you might want your driver to be able to tell applications information about a device. That's when you'd need an IOCTL! (probably)
   * For applications to talk with your driver, you need a device interface and an Io queue. Apps can find your device using the winapi's `CM_Get_Device_Interface_List_Size` and `CM_Get_Device_Interface_List` functions, then passing the same GUID that you used when creating the device interface in your driver.
   * For usage of device interfaces and io queues, read the `VHIDCreateDevice` function in `Device.c`, and the `VHIDQueueInitialize` and `VHIDEvtIoDeviceControl` functions in `Queue.c`.
 * You don't need to worry about plug and play unless you driver works on real hardware
    * The default hardware id for an empty kmdf driver is something like `Root\...`
    * The default hardware id for a kmdf usb driver is `USB\VID_XXXX&PID_XXXX` where the `VID` is the vendor id and the `PID` is the product id (both are in hexadecimal)
    * To specify the device your driver works on, you must edit the *.inf file. Typically after the [`Manufacturer` section](https://learn.microsoft.com/en-us/windows-hardware/drivers/install/inf-manufacturer-section) there's [another section](https://learn.microsoft.com/en-us/windows-hardware/drivers/install/inf-models-section) that contains something like `%...DeviceDesc% = ...`. The hardware id at the end of that line is what you need to change
 * HID report descriptor sizes are in **bits**
 * [This handy website](https://eleccelerator.com/usbdescreqparser/) can help you parse hid report descriptors but to actually fully understand them, you may have to [read the HID spec](https://usb.org/sites/default/files/hut1_6.pdf)
 * When using `VhfReadReportSubmit`, the `PHID_XFER_PACKET`'s `reportBufferLen` and `reportBuffer` must be correct and in the same format as the report descriptor you passed into `VHF_CONFIG_INIT`. If they are incorrect, the function will still return `STATUS_SUCCESS` (for some reason).

## Use
 To change which device mode this driver is using you'll have to get my configuration script!

 Download the latest release of (my) [JBS3](https://github.com/MagicQuest/JBS3) and run the scripts/ignore/[customdriverconfigure.js](https://github.com/MagicQuest/JBS3/raw/refs/heads/main/scripts/ignore/customdriverconfigure.js) file with it

### IOCTLS - defined in Queue.h