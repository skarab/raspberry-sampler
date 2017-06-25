#include "includes.h"
#include "sample.h"

typedef struct
{
    double Data[10];
} FILTER_FORMANT;

inline void FILTER_FORMANT_Initialize(FILTER_FORMANT& filter)
{
    memset(filter.Data, 0, sizeof(double)*10);
}

inline void FILTER_FORMANT_Compute(int& value, const vector<int>& params, FILTER_FORMANT& filter)
{
    float formant = params[PARAM_Formant]/100.0f;
    int id = params[PARAM_FormantID]-1;

    static const double coeff[5][11]= {
        { 8.11044e-06, 8.943665402, -36.83889529, 92.01697887, -154.337906, 181.6233289, -151.8651235, 89.09614114, -35.10298511, 8.388101016, -0.923313471 },  // A
        { 4.36215e-06, 8.90438318, -36.55179099, 91.05750846, -152.422234, 179.1170248, -149.6496211, 87.78352223, -34.60687431, 8.282228154, -0.914150747 },   // E
        { 3.33819e-06, 8.893102966, -36.49532826, 90.96543286, -152.4545478, 179.4835618, -150.315433, 88.43409371, -34.98612086, 8.407803364, -0.932568035 },  // I
        { 1.13572e-06, 8.994734087, -37.2084849, 93.22900521, -156.6929844, 184.596544, -154.3755513, 90.49663749, -35.58964535, 8.478996281, -0.929252233 },   // O
        { 4.09431e-07, 8.997322763, -37.20218544, 93.11385476, -156.2530937, 183.7080141, -153.2631681, 89.59539726, -35.12454591, 8.338655623, -0.910251753 }  // U
    };

    double* memory = filter.Data;
    double res = coeff[id][0]*(value/32767.0)
        +coeff[id][1]*memory[0]
        +coeff[id][2]*memory[1]
        +coeff[id][3]*memory[2]
        +coeff[id][4]*memory[3]
        +coeff[id][5]*memory[4]
        +coeff[id][6]*memory[5]
        +coeff[id][7]*memory[6]
        +coeff[id][8]*memory[7]
        +coeff[id][9]*memory[8]
        +coeff[id][10]*memory[9];

    memory[9] = memory[8];
    memory[8] = memory[7];
    memory[7] = memory[6];
    memory[6] = memory[5];
    memory[5] = memory[4];
    memory[4] = memory[3];
    memory[3] = memory[2];
    memory[2] = memory[1];
    memory[1] = memory[0];
    memory[0] = res;

    value = (int)(res*32767.0*formant+value*(1.0-formant));
}
