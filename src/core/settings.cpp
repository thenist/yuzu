// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <string_view>

#include "common/file_util.h"
#include "core/core.h"
#include "core/gdbstub/gdbstub.h"
#include "core/hle/service/hid/hid.h"
#include "core/settings.h"
#include "video_core/renderer_base.h"

namespace Settings {

Values values = {};
static bool configuring_global = true;

std::string GetTimeZoneString() {
    static constexpr std::array timezones{
        "auto",      "default",   "CET", "CST6CDT", "Cuba",    "EET",    "Egypt",     "Eire",
        "EST",       "EST5EDT",   "GB",  "GB-Eire", "GMT",     "GMT+0",  "GMT-0",     "GMT0",
        "Greenwich", "Hongkong",  "HST", "Iceland", "Iran",    "Israel", "Jamaica",   "Japan",
        "Kwajalein", "Libya",     "MET", "MST",     "MST7MDT", "Navajo", "NZ",        "NZ-CHAT",
        "Poland",    "Portugal",  "PRC", "PST8PDT", "ROC",     "ROK",    "Singapore", "Turkey",
        "UCT",       "Universal", "UTC", "W-SU",    "WET",     "Zulu",
    };

    const auto time_zone_index = static_cast<std::size_t>(values.time_zone_index.GetValue());
    ASSERT(time_zone_index < timezones.size());
    return timezones[time_zone_index];
}

void Apply() {
    GDBStub::SetServerPort(values.gdbstub_port);
    GDBStub::ToggleServer(values.use_gdbstub);

    auto& system_instance = Core::System::GetInstance();
    if (system_instance.IsPoweredOn()) {
        system_instance.Renderer().RefreshBaseSettings();
    }

    Service::HID::ReloadInputDevices();
}

void LogSettings() {
    const auto log_setting = [](std::string_view name, const auto& value) {
        LOG_INFO(Config, "{}: {}", name, value);
    };

    LOG_INFO(Config, "yuzu Configuration:");
    log_setting("Controls_UseDockedMode", values.use_docked_mode.GetValue());
    log_setting("System_RngSeed", values.rng_seed.GetValue().value_or(0));
    log_setting("System_CurrentUser", values.current_user);
    log_setting("System_LanguageIndex", values.language_index.GetValue());
    log_setting("System_RegionIndex", values.region_index.GetValue());
    log_setting("System_TimeZoneIndex", values.time_zone_index.GetValue());
    log_setting("Core_UseMultiCore", values.use_multi_core.GetValue());
    log_setting("CPU_Accuracy", values.cpu_accuracy);
    log_setting("Renderer_UseResolutionFactor", values.resolution_factor.GetValue());
    log_setting("Renderer_UseFrameLimit", values.use_frame_limit.GetValue());
    log_setting("Renderer_FrameLimit", values.frame_limit.GetValue());
    log_setting("Renderer_UseDiskShaderCache", values.use_disk_shader_cache.GetValue());
    log_setting("Renderer_GPUAccuracyLevel", values.gpu_accuracy.GetValue());
    log_setting("Renderer_UseAsynchronousGpuEmulation",
                values.use_asynchronous_gpu_emulation.GetValue());
    log_setting("Renderer_UseNvdecEmulation", values.use_nvdec_emulation.GetValue());
    log_setting("Renderer_UseVsync", values.use_vsync.GetValue());
    log_setting("Renderer_UseAssemblyShaders", values.use_assembly_shaders.GetValue());
    log_setting("Renderer_UseAsynchronousShaders", values.use_asynchronous_shaders.GetValue());
    log_setting("Renderer_AnisotropicFilteringLevel", values.max_anisotropy.GetValue());
    log_setting("Audio_OutputEngine", values.sink_id);
    log_setting("Audio_EnableAudioStretching", values.enable_audio_stretching.GetValue());
    log_setting("Audio_OutputDevice", values.audio_device_id);
    log_setting("DataStorage_UseVirtualSd", values.use_virtual_sd);
    log_setting("DataStorage_NandDir", Common::FS::GetUserPath(Common::FS::UserPath::NANDDir));
    log_setting("DataStorage_SdmcDir", Common::FS::GetUserPath(Common::FS::UserPath::SDMCDir));
    log_setting("Debugging_UseGdbstub", values.use_gdbstub);
    log_setting("Debugging_GdbstubPort", values.gdbstub_port);
    log_setting("Debugging_ProgramArgs", values.program_args);
    log_setting("Services_BCATBackend", values.bcat_backend);
    log_setting("Services_BCATBoxcatLocal", values.bcat_boxcat_local);
}

bool IsConfiguringGlobal() {
    return configuring_global;
}

void SetConfiguringGlobal(bool is_global) {
    configuring_global = is_global;
}

bool IsGPULevelExtreme() {
    return values.gpu_accuracy.GetValue() == GPUAccuracy::Extreme;
}

bool IsGPULevelHigh() {
    return values.gpu_accuracy.GetValue() == GPUAccuracy::Extreme ||
           values.gpu_accuracy.GetValue() == GPUAccuracy::High;
}

float Volume() {
    if (values.audio_muted) {
        return 0.0f;
    }
    return values.volume.GetValue();
}

void RestoreGlobalState() {
    // If a game is running, DO NOT restore the global settings state
    if (Core::System::GetInstance().IsPoweredOn()) {
        return;
    }

    // Audio
    values.enable_audio_stretching.SetGlobal(true);
    values.volume.SetGlobal(true);

    // Core
    values.use_multi_core.SetGlobal(true);

    // Renderer
    values.renderer_backend.SetGlobal(true);
    values.vulkan_device.SetGlobal(true);
    values.aspect_ratio.SetGlobal(true);
    values.max_anisotropy.SetGlobal(true);
    values.use_frame_limit.SetGlobal(true);
    values.frame_limit.SetGlobal(true);
    values.use_disk_shader_cache.SetGlobal(true);
    values.gpu_accuracy.SetGlobal(true);
    values.use_asynchronous_gpu_emulation.SetGlobal(true);
    values.use_nvdec_emulation.SetGlobal(true);
    values.use_vsync.SetGlobal(true);
    values.use_assembly_shaders.SetGlobal(true);
    values.use_asynchronous_shaders.SetGlobal(true);
    values.use_fast_gpu_time.SetGlobal(true);
    values.bg_red.SetGlobal(true);
    values.bg_green.SetGlobal(true);
    values.bg_blue.SetGlobal(true);

    // System
    values.language_index.SetGlobal(true);
    values.region_index.SetGlobal(true);
    values.time_zone_index.SetGlobal(true);
    values.rng_seed.SetGlobal(true);
    values.custom_rtc.SetGlobal(true);
    values.sound_index.SetGlobal(true);

    // Controls
    values.players.SetGlobal(true);
    values.use_docked_mode.SetGlobal(true);
    values.vibration_enabled.SetGlobal(true);
    values.motion_enabled.SetGlobal(true);
}

void Sanitize() {
    values.use_asynchronous_gpu_emulation.SetValue(
        values.use_asynchronous_gpu_emulation.GetValue() || values.use_multi_core.GetValue());
}

} // namespace Settings
