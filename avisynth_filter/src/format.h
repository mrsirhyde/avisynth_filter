// License: https://github.com/CrendKing/avisynth_filter/blob/master/LICENSE

#pragma once

#include "pch.h"


namespace AvsFilter {

class Format {
public:
    struct Definition {
        const CLSID &mediaSubtype;
        int avsType;
        uint8_t bitCount;

        // in case of planar formats, these numbers are for the default plane
        uint8_t componentsPerPixel;
    };

    struct VideoFormat {
        int definition;
        VideoInfo videoInfo;
        int pixelAspectRatio;
        int hdrType;
        int hdrLuminance;

        BITMAPINFOHEADER bmi;
        VIDEOINFOHEADER *vih;

        auto GetCodecFourCC() const -> DWORD;
        auto GetCodecName() const -> std::string;
    };

    static auto LookupMediaSubtype(const CLSID &mediaSubtype) -> std::optional<int>;
    static auto LookupAvsType(int avsType) -> std::vector<int>;

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<AM_MEDIA_TYPE, std::decay_t<T>>>>
    static auto GetBitmapInfo(T &mediaType) {
        if (SUCCEEDED(CheckVideoInfoType(&mediaType))) {
            return HEADER(mediaType.pbFormat);
        }

        if (SUCCEEDED(CheckVideoInfo2Type(&mediaType))) {
            return &reinterpret_cast<VIDEOINFOHEADER2 *>(mediaType.pbFormat)->bmiHeader;
        }

        return static_cast<BITMAPINFOHEADER *>(nullptr);
    }

    static auto GetVideoFormat(const AM_MEDIA_TYPE &mediaType) -> VideoFormat;
    static auto WriteSample(const VideoFormat &format, PVideoFrame srcFrame, BYTE *dstBuffer, IScriptEnvironment *avsEnv) -> void;
    static auto CreateFrame(const VideoFormat &format, const BYTE *srcBuffer, IScriptEnvironment *avsEnv) -> PVideoFrame;
    static auto CopyFromInput(const VideoFormat &format, const BYTE *srcBuffer, BYTE *dstSlices[], const int dstStrides[], int dstRowSize, int dstHeight, IScriptEnvironment *avsEnv) -> void;
    static auto CopyToOutput(const VideoFormat &format, const BYTE *srcSlices[], const int srcStrides[], BYTE *dstBuffer, int srcRowSize, int srcHeight, IScriptEnvironment *avsEnv) -> void;

    static const std::vector<Definition> DEFINITIONS;

private:
    template <typename Component>
    static auto Deinterleave(const BYTE *src, int srcStride, BYTE *dst1, BYTE *dst2, int dstStride, int rowSize, int height, __m128i mask1, __m128i mask2) -> void {
        const int iterations = rowSize / sizeof(__m128i);
        const int remainderStart = iterations * sizeof(__m128i);

        for (int y = 0; y < height; ++y) {
            const __m128i *src_128 = reinterpret_cast<const __m128i *>(src);
            __int64 *dst1_64 = reinterpret_cast<__int64 *>(dst1);
            __int64 *dst2_64 = reinterpret_cast<__int64 *>(dst2);

            for (int i = 0; i < iterations; ++i) {
                const __m128i n = *src_128++;
                _mm_storeu_si64(dst1_64++, _mm_shuffle_epi8(n, mask1));
                _mm_storeu_si64(dst2_64++, _mm_shuffle_epi8(n, mask2));
            }

            // copy remaining unaligned bytes (rowSize % sizeof(__m128i) != 0)
            for (int i = remainderStart; i < rowSize; i += 2 * sizeof(Component)) {
                *reinterpret_cast<Component *>(dst1 + i / 2) = *reinterpret_cast<const Component *>(src + i + 0);
                *reinterpret_cast<Component *>(dst2 + i / 2) = *reinterpret_cast<const Component *>(src + i + sizeof(Component));
            }

            src += srcStride;
            dst1 += dstStride;
            dst2 += dstStride;
        }
    }

    template <typename Component>
    static auto Interleave(const BYTE *src1, const BYTE *src2, int srcStride, BYTE *dst, int dstStride, int rowSize, int height) -> void {
        const int iterations = rowSize / sizeof(__m128i);
        const int remainderStart = iterations * sizeof(__m128i);

        for (int y = 0; y < height; ++y) {
            const __m128i *src1_128 = reinterpret_cast<const __m128i *>(src1);
            const __m128i *src2_128 = reinterpret_cast<const __m128i *>(src2);
            __m128i *dst_128 = reinterpret_cast<__m128i *>(dst);

            for (int i = 0; i < iterations; ++i) {
                const __m128i s1 = *src1_128++;
                const __m128i s2 = *src2_128++;

                if constexpr (sizeof(Component) == 1) {
                    *dst_128++ = _mm_unpacklo_epi8(s1, s2);
                    *dst_128++ = _mm_unpackhi_epi8(s1, s2);
                } else {
                    *dst_128++ = _mm_unpacklo_epi16(s1, s2);
                    *dst_128++ = _mm_unpackhi_epi16(s1, s2);
                }
            }

            for (int i = remainderStart; i < rowSize; i += sizeof(Component)) {
                *reinterpret_cast<Component *>(dst + static_cast<size_t>(i) * 2 + 0) = *reinterpret_cast<const Component *>(src1 + i);
                *reinterpret_cast<Component *>(dst + static_cast<size_t>(i) * 2 + sizeof(Component)) = *reinterpret_cast<const Component *>(src2 + i);
            }

            src1 += srcStride;
            src2 += srcStride;
            dst += dstStride;
        }
    }
};

}
