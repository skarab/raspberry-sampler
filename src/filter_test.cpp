#include "filter_test.h"

FilterTest::FilterTest()
{
    _Initialize(_Left);
    _Initialize(_Right);
}

void FilterTest::Compute(float& left, float& right, const vector<int>& params)
{
    //if ((_CutOff!=params[PARAM_NotchCutOff]) || (_BandWidth!=params[PARAM_NotchBandWidth]))
    //{
    //}

    _Compute(left, _Left);
    _Compute(right, _Right);
}

void FilterTest::_Initialize(Data& data)
{
    memset(&data, 0, sizeof(Data));
}

void FilterTest::_Compute(float& value, Data& data)
{

}
