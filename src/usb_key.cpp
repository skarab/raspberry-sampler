#include "usb_key.h"
#include "controller.h"

UsbKey* UsbKey::_Instance = NULL;

UsbKey::UsbKey() :
    _HasDevice(false),
    _IsMounted(false)
{
    if (_Instance!=NULL)
        ERROR("UsbKey is already instantiated");
    _Instance = this;
}

UsbKey::~UsbKey()
{
}

void UsbKey::Update()
{
    bool was_mounted = _IsMounted;
    bool had_device = _HasDevice;
    _HasDevice = _CheckDevice();

    if (_HasDevice!=had_device)
    {
        if (_HasDevice)
        {
            LOG("USB key on");
            _Mount();
        }
        else
        {
            LOG("USB key off");
            _Unmount();
        }
    }
}

bool UsbKey::_CheckDevice()
{
    if (_HasDevice && (access(_DevicePath.c_str(), F_OK )!=-1))
        return true;

    bool found = false;
    DIR* dir = opendir(SAMPLER_USB_KEY_DEVICE);
    if (dir!=NULL)
    {
        dirent* entry = readdir(dir);

        while (entry!=NULL && !found)
        {
            if (entry->d_name[0]!='.')
            {
                _DevicePath = string("\"")+string(SAMPLER_USB_KEY_DEVICE)+entry->d_name+string("\"");
                found = true;
            }
            entry = readdir(dir);
        }
        closedir(dir);
    }
    return found;
}

bool UsbKey::_CheckFolder() const
{
    return access(SAMPLER_FOLDER, F_OK )!=-1;
}

void UsbKey::_Mount()
{
    string cmd = "mount ";
    cmd += _DevicePath;
    cmd += " ";
    cmd += SAMPLER_USB_KEY_MOUNT;
    system(cmd.c_str());
    _IsMounted = _CheckFolder();

    if (_IsMounted)
    {
        LOG("-> USB key ok!");
        Controller::Get().OnUsbKeyMounted();
    }
}

void UsbKey::_Unmount()
{
    string cmd = "umount -l -f ";
    cmd += SAMPLER_USB_KEY_MOUNT;
    system(cmd.c_str());
    _IsMounted = false;
}
