// License: https://github.com/CrendKing/avisynth_filter/blob/master/LICENSE

#pragma once

#include "hdr.h"


namespace SynthFilter {

class CSynthFilter;

class FrameHandler {
public:
    explicit FrameHandler(CSynthFilter &filter);

    DISABLE_COPYING(FrameHandler)

    auto AddInputSample(IMediaSample *inputSample) -> HRESULT;
    auto GetSourceFrame(int frameNb) -> PVideoFrame;
    auto BeginFlush() -> void;
    auto EndFlush(const std::function<void ()> &interim) -> void;
    auto Start() -> void;
    auto Stop() -> void;
    auto GetInputBufferSize() const -> int;
    constexpr auto GetSourceFrameNb() const -> int { return _nextSourceFrameNb; }
    constexpr auto GetOutputFrameNb() const -> int { return _nextOutputFrameNb; }
    constexpr auto GetDeliveryFrameNb() const -> int { return _nextOutputFrameNb; }
    constexpr auto GetCurrentInputFrameRate() const -> int { return _currentInputFrameRate; }
    constexpr auto GetCurrentOutputFrameRate() const -> int { return _currentOutputFrameRate; }

private:
    struct SourceFrameInfo {
        PVideoFrame frame;
        REFERENCE_TIME startTime;
        std::shared_ptr<HDRSideData> hdrSideData;
    };

    static auto RefreshFrameRatesTemplate(int sampleNb, REFERENCE_TIME startTime,
                                          int &checkpointSampleNb, REFERENCE_TIME &checkpointStartTime,
                                          int &currentFrameRate) -> void;

    auto ResetInput() -> void;
    auto PrepareOutputSample(ATL::CComPtr<IMediaSample> &sample, REFERENCE_TIME startTime, REFERENCE_TIME stopTime) -> bool;
    auto WorkerProc() -> void;
    auto GarbageCollect(int srcFrameNb) -> void;
    auto ChangeOutputFormat() -> bool;
    auto RefreshInputFrameRates(int frameNb, REFERENCE_TIME startTime) -> void;
    auto RefreshOutputFrameRates(int frameNb, REFERENCE_TIME startTime) -> void;

    static constexpr const int NUM_SRC_FRAMES_PER_PROCESSING = 3;

    CSynthFilter &_filter;

    std::map<int, SourceFrameInfo> _sourceFrames;

    mutable std::shared_mutex _sourceMutex;

    std::condition_variable_any _addInputSampleCv;
    std::condition_variable_any _newSourceFrameCv;

    int _nextSourceFrameNb;
    std::atomic<int> _maxRequestedFrameNb;
    int _nextOutputFrameNb;
    REFERENCE_TIME _nextOutputFrameStartTime;
    bool _notifyChangedOutputMediaType = false;

    std::thread _workerThread;

    std::atomic<bool> _isFlushing = false;
    std::atomic<bool> _isStopping = false;
    std::atomic<bool> _isWorkerLatched = false;

    int _frameRateCheckpointInputSampleNb;
    REFERENCE_TIME _frameRateCheckpointInputSampleStartTime;
    int _frameRateCheckpointOutputFrameNb;
    REFERENCE_TIME _frameRateCheckpointOutputFrameStartTime;
    int _currentInputFrameRate;
    int _currentOutputFrameRate;
};

}
