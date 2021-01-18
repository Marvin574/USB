#ifndef EMAI_HARDWARE_USB_V1_1_USB_H
#define EMAI_HARDWARE_USB_V1_1_USB_H

#include <EMAI/hardware/usb/1.1/IUsb.h>
#include <EMAI/hardware/usb/1.1/types.h>
#include <EMAI/hardware/usb/1.1/IUsbCallback.h>
#include <hidl/Status.h>
#include <utils/Log.h>

#define UEVENT_MSG_LEN 2048
// The type-c stack waits for 4.5 - 5.5 secs before declaring a port non-pd.
// The -partner directory would not be created until this is done.
// Having a margin of ~3 secs for the directory and other related bookeeping
// structures created and uvent fired.
#define PORT_TYPE_TIMEOUT 8

namespace EMAI {
namespace hardware {
namespace usb {
namespace V1_1 {

namespace implementation {
using ::EMAI::hardware::usb::V1_0::PortRole;
using ::EMAI::hardware::usb::V1_0::PortRoleType;
using ::EMAI::hardware::usb::V1_0::PortDataRole;
using ::EMAI::hardware::usb::V1_0::PortPowerRole;
using ::EMAI::hardware::usb::V1_0::Status;
using ::EMAI::hardware::usb::V1_1::IUsb;
using ::EMAI::hardware::usb::V1_1::IUsbCallback;
using ::EMAI::hardware::usb::V1_1::PortMode_1_1;
using ::EMAI::hardware::usb::V1_1::PortStatus_1_1;
using ::EMAI::hidl::base::V1_0::DebugInfo;
using ::EMAI::hidl::base::V1_0::IBase;
using ::EMAI::hardware::hidl_array;
using ::EMAI::hardware::hidl_memory;
using ::EMAI::hardware::hidl_string;
using ::EMAI::hardware::hidl_vec;
using ::EMAI::hardware::Return;
using ::EMAI::hardware::Void;
using ::EMAI::sp;

struct Usb : public IUsb {
    Usb();
    
    Return<void> switchRole(const hidl_string& portName, const V1_0::PortRole& role) override;
    Return<void> setCallback(const sp<V1_0::IUsbCallback>& callback) override;
    Return<void> queryPortStatus() override;
    
    
    sp<V1_0::IUsbCallback> mCallback_1_0;
    // Protects mCallback variable
    pthread_mutex_t mLock;
    // Protects roleSwitch operation
    pthread_mutex_t mRoleSwitchLock;
    // Threads waiting for the partner to come back wait here
    pthread_cond_t mPartnerCV;
    // lock protecting mPartnerCV
    pthread_mutex_t mPartnerLock;
    // Variable to signal partner coming back online after type switch
    bool mPartnerUp;
    private:
    
        pthread_t mPoll;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace usb
}  // namespace hardware
}  // namespace EMAI

#endif  // EMAI_HARDWARE_USB_V1_1_USB_H
