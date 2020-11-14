// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "core/hle/service/nvdrv/devices/nvhost_nvdec_common.h"

namespace Service::Nvidia::Devices {
class nvmap;

class nvhost_vic final : public nvhost_nvdec_common {
public:
    explicit nvhost_vic(Core::System& system, std::shared_ptr<nvmap> nvmap_dev);
    ~nvhost_vic();

    NvResult Ioctl1(Ioctl command, const std::vector<u8>& input, std::vector<u8>& output) override;
    NvResult Ioctl2(Ioctl command, const std::vector<u8>& input,
                    const std::vector<u8>& inline_input, std::vector<u8>& output) override;
    NvResult Ioctl3(Ioctl command, const std::vector<u8>& input, std::vector<u8>& output,
                    std::vector<u8>& inline_output) override;
};
} // namespace Service::Nvidia::Devices
