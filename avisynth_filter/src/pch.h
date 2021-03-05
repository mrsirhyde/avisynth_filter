#pragma once

#include <codeanalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)

#include <array>
#include <clocale>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#include <atlbase.h>

#include <commctrl.h>
#include <commdlg.h>
#include <cguid.h>
#include <immintrin.h>
#include <initguid.h>
#include <intrin.h>
#include <processthreadsapi.h>
#include <shellapi.h>

// DirectShow BaseClasses
#include <streams.h>
#include <dvdmedia.h>

// AviSynth
#include <avisynth.h>

#include <SimpleIni.h>

#pragma warning(pop)

#include "resource.h"

#pragma warning(push)
#pragma warning(disable: 26495 26812)
