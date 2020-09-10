#pragma once

#include "pch.h"
#include "format.h"


DECLARE_INTERFACE_(IAvsFilterSettings, IUnknown) {
    virtual auto STDMETHODCALLTYPE SaveSettings() const -> void = 0;

    virtual auto STDMETHODCALLTYPE GetAvsFile() const -> std::wstring = 0;
    virtual auto STDMETHODCALLTYPE SetAvsFile(const std::wstring & avsFile) -> void = 0;

    virtual auto STDMETHODCALLTYPE ReloadAvsFile() -> void = 0;
    virtual auto STDMETHODCALLTYPE GetAvsError() const -> std::string = 0;

    virtual auto STDMETHODCALLTYPE IsRemoteControlled() -> bool = 0;

    virtual auto STDMETHODCALLTYPE GetInputFormats() const -> DWORD = 0;
    virtual auto STDMETHODCALLTYPE SetInputFormats(DWORD formatBits) -> void = 0;
};

DECLARE_INTERFACE_(IAvsFilterStatus, IUnknown) {
    enum PlayState {
        StateInitializing,
        StateError,
        StatePlaying,
        StatePaused
    };

    virtual auto STDMETHODCALLTYPE GetBufferSize() -> int = 0;
    virtual auto STDMETHODCALLTYPE GetCurrentPrefetch() const -> int = 0;
    virtual auto STDMETHODCALLTYPE GetInitialPrefetch() const -> int = 0;

    virtual auto STDMETHODCALLTYPE GetPlayState() const -> PlayState = 0;
    virtual auto STDMETHODCALLTYPE GetFrameNumbers() const -> std::pair<int, int> = 0;
    virtual auto STDMETHODCALLTYPE GetInputFrameRate() const -> double = 0;
    virtual auto STDMETHODCALLTYPE GetOutputFrameRate() const -> double = 0;

    virtual auto STDMETHODCALLTYPE GetSourcePath() const -> std::wstring = 0;
    virtual auto STDMETHODCALLTYPE GetMediaInfo() const -> Format::VideoFormat = 0;
    virtual auto STDMETHODCALLTYPE GetFiltersList() const -> std::list<std::wstring> = 0;
};