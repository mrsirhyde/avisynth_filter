// License: https://github.com/CrendKing/avisynth_filter/blob/master/LICENSE

#pragma once

#include "pch.h"


namespace AvsFilter {

class CAviSynthFilter;

class RemoteControl {
public:
    explicit RemoteControl(CAviSynthFilter &filter);
    virtual ~RemoteControl();

    auto Start() -> void;

private:
    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;

    auto Run() -> void;
    auto SendString(HWND hReceiverWindow, ULONG_PTR msgId, const std::string &data) const -> void;
    auto SendString(HWND hReceiverWindow, ULONG_PTR msgId, const std::wstring &data) const -> void;
    auto HandleCopyData(HWND hSenderWindow, const COPYDATASTRUCT *copyData) const -> LRESULT;

    std::thread _msgThread;
    HWND _hWnd;

    CAviSynthFilter &_filter;
};

}
