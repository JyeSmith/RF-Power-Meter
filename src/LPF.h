#pragma once
#include "stdint.h"

/////////// Super Simple Fixed Point Lowpass ////////////////

class LPF
{
public:
    int32_t SmoothDataINT;
    int32_t SmoothDataFP;
    int32_t Beta = 3;     // Length
    int32_t FP_Shift = 5; // Number of fractional bits

    LPF(int Beta_, int FP_Shift_)
    {
        Beta = Beta_;
        FP_Shift = FP_Shift_;
    }

    LPF(int Beta_)
    {
        Beta = Beta_;
    }

    LPF()
    {
        Beta = 3;
        FP_Shift = 5;
    }

    int32_t update(int32_t Indata)
    {
        int RawData;
        RawData = Indata;
        RawData <<= FP_Shift; // Shift to fixed point
        SmoothDataFP = (SmoothDataFP << Beta) - SmoothDataFP;
        SmoothDataFP += RawData;
        SmoothDataFP >>= Beta;
        // Don't do the following shift if you want to do further
        // calculations in fixed-point using SmoothData
        SmoothDataINT = SmoothDataFP >> FP_Shift;
        return SmoothDataINT;
    }

    void init(int32_t Indata)
    {
        SmoothDataINT = Indata;
        SmoothDataFP = SmoothDataINT << FP_Shift;
        // for (int i = 0; i < 50; i++)
        // {
        //     this->update(Indata);
        // }
    }
};