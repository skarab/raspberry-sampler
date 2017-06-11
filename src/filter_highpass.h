#ifndef __SAMPLER_FILTER_HIGHPASS__
#define __SAMPLER_FILTER_HIGHPASS__

#include "includes.h"
#include "filter.h"

class FilterHighPass : public Filter
{
public:

    FilterHighPass();

    virtual void Compute(float& left, float& right, const vector<int>& params);

protected:

    inline void _Compute(float& value, float* inputs, float* outputs, float a1, float a2, float b1, float b2)
    {
        float input = value;
        value = a1*input+a2*inputs[0]+a1*inputs[1]-b1*outputs[0]-b2*outputs[1];

        //if (value>10.0f || value<-10.0f)
        //    value = input;

        inputs[1] = inputs[0];
        inputs[0] = input;
        outputs[1] = outputs[0];
        outputs[0] = value;
    }

    float _InputLeft[2];
    float _OutputLeft[2];
    float _InputRight[2];
    float _OutputRight[2];
};

#endif
