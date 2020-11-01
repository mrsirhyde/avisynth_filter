// License: https://github.com/CrendKing/avisynth_filter/blob/master/LICENSE

#pragma once

#include "pch.h"
#include "version.h"


namespace AvsFilter {

// {E5E2C1A6-C90F-4247-8BF5-604FB180A932}
DEFINE_GUID(inline CLSID_AviSynthFilter,
            0xe5e2c1a6, 0xc90f, 0x4247, 0x8b, 0xf5, 0x60, 0x4f, 0xb1, 0x80, 0xa9, 0x32);

// {90C56868-7D47-4AA2-A42D-06406A6DB35F}
DEFINE_GUID(inline CLSID_AvsPropSettings,
            0x90c56868, 0x7d47, 0x4aa2, 0xa4, 0x2d, 0x06, 0x40, 0x6a, 0x6d, 0xb3, 0x5f);

// {E58206EF-C9F2-4F8C-BF0B-975C28552700}
DEFINE_GUID(inline CLSID_AvsPropStatus,
            0xe58206ef, 0xc9f2, 0x4f8c, 0xbf, 0x0b, 0x97, 0x5c, 0x28, 0x55, 0x27, 0x00);

// {85C582FE-5B6F-4BE5-A0B0-57A4FDAB4412}
DEFINE_GUID(inline IID_IAvsFilter,
            0x85c582fe, 0x5b6f, 0x4be5, 0xa0, 0xb0, 0x57, 0xa4, 0xfd, 0xab, 0x44, 0x12);

// {53820DBC-A7B8-49C4-B17B-E511591A790C}
DEFINE_GUID(inline IID_MediaSideDataHDR,
            0x53820dbc, 0xa7b8, 0x49c4, 0xb1, 0x7b, 0xe5, 0x11, 0x59, 0x1a, 0x79, 0x0c);

// {ED6AE576-7CBE-41A6-9DC3-07C35DC13EF9}
DEFINE_GUID(inline IID_MediaSideDataHDRContentLightLevel,
            0xed6ae576, 0x7cbe, 0x41a6, 0x9d, 0xc3, 0x07, 0xc3, 0x5d, 0xc1, 0x3e, 0xf9);

// {F169B76C-75A3-49E6-A23A-14983EBF4370}
DEFINE_GUID(inline IID_MediaSideData3DOffset,
            0xf169b76c, 0x75a3, 0x49e6, 0xa2, 0x3a, 0x14, 0x98, 0x3e, 0xbf, 0x43, 0x70);

static const GUID MEDIASUBTYPE_I420  = FOURCCMap('024I');
static const GUID MEDIASUBTYPE_RGB48 = FOURCCMap('0BGR');

#define WidenHelper(str)                  L##str
#define Widen(str)                        WidenHelper(str)

#ifdef _DEBUG
#define FILTER_NAME_SUFFIX                " [Debug]"
#else
#define FILTER_NAME_SUFFIX
#endif // DEBUG
#define SETTINGS_SUFFIX                   " Settings"
#define STATUS_SUFFIX                     " Status"

#define FILTER_NAME_FULL FILTER_NAME_BASE FILTER_NAME_SUFFIX
#define SETTINGS_FULL FILTER_NAME_BASE    SETTINGS_SUFFIX FILTER_NAME_SUFFIX
#define STATUS_FULL FILTER_NAME_BASE      STATUS_SUFFIX FILTER_NAME_SUFFIX

#define FILTER_NAME_WIDE                  Widen(FILTER_NAME_FULL)
#define SETTINGS_WIDE                     Widen(SETTINGS_FULL)
#define STATUS_WIDE                       Widen(STATUS_FULL)

// interface version 7 = AviSynth+ 3.5
static constexpr int MINIMUM_AVISYNTH_PLUS_INTERFACE_VERSION = 7;

/*
 * Max number of frames received before blocking upstream from flooding the input queue.
 * Once reached, it must wait until the output thread delivers and GC source frames.
 */
static constexpr int MAX_SOURCE_FRAMES_AHEAD_OF_DELIVERY = 7;

static constexpr int DEFAULT_OUTPUT_SAMPLE_WORKER_THREAD_COUNT = 1;

/*
 * Some source filter may not set the VIDEOINFOHEADER::AvgTimePerFrame field.
 * Default to 25 FPS in such cases.
 */
static constexpr REFERENCE_TIME DEFAULT_AVG_TIME_PER_FRAME     = 400000;
static constexpr int STATUS_PAGE_TIMER_INTERVAL_MS             = 1000;
static constexpr wchar_t *UNAVAILABLE_SOURCE_PATH              = L"N/A";

/*
 * Stream could last forever. Use a large power as the fake number of frames.
 * Avoid using too large number because some AviSynth filters allocate memory based on the number of frames.
 * Also, some filters may perform calculation on it, resulting overflow.
 * Same as ffdshow, uses a highly composite number 10810800, which could last 50 hours for a 60fps stream.
 */
static constexpr int NUM_FRAMES_FOR_INFINITE_STREAM            = 10810800;

static constexpr wchar_t *REGISTRY_KEY_NAME                    = L"Software\\AviSynthFilter";
static constexpr wchar_t *SETTING_NAME_AVS_FILE                = L"AvsFile";
static constexpr wchar_t *SETTING_NAME_LOG_FILE                = L"LogFile";
static constexpr wchar_t *SETTING_NAME_FORMATS                 = L"Formats";
static constexpr wchar_t *SETTING_NAME_OUTPUT_THREADS          = L"OutputThreads";
static constexpr wchar_t *SETTING_NAME_REMOTE_CONTROL          = L"RemoteControl";

static constexpr int REMOTE_CONTROL_SMTO_TIMEOUT_MS            = 1000;

}
