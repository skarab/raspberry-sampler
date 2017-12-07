#ifndef __SAMPLER_USB_KEY__
#define __SAMPLER_USB_KEY__

#include "includes.h"

class UsbKey
{
public:

    UsbKey();
    ~UsbKey();

    static UsbKey& Get() { return *_Instance; }

    bool IsOK() const { return _IsMounted; }

    void Update();

private:

    bool _CheckDevice();
    bool _CheckFolder() const;
    void _Mount();
    void _Unmount();

    static UsbKey* _Instance;
    bool _HasDevice;
    string _DevicePath;
    bool _IsMounted;
};

#endif
