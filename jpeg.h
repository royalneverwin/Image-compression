/********jpeg算法的有关定义*********/
// quantization tables from JPEG Standard, Annex K
const BYTE DefaultQuantLuminance[8][8] = {
        {16, 11, 10, 16, 24,  40,  51,  61},
        {12, 12, 14, 19, 26,  58,  60,  55},
        {14, 13, 16, 24, 40,  57,  69,  56},
        {14, 17, 22, 29, 51,  87,  80,  62},
        {18, 22, 37, 56, 68,  109, 103, 77},
        {24, 35, 55, 64, 81,  104, 113, 92},
        {49, 64, 78, 87, 103, 121, 120, 101},
        {72, 92, 95, 98, 112, 100, 103, 99}
};

const BYTE DefaultQuantChrominance[8][8] = {
        {17, 18, 24, 47, 99, 99, 99, 99},
        {18, 21, 26, 66, 99, 99, 99, 99},
        {24, 26, 56, 99, 99, 99, 99, 99},
        {47, 66, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99}
};


//DCT transformation matrix
const float T[8][8] = {
        0.35355,0.35355,0.35355,0.35355,0.35355,0.35355,0.35355,0.35355,
        0.49039,0.41573,0.27779,0.09755,-0.09755,-0.27779,-0.41573,-0.49039,
        0.46194,0.19134,-0.19134,-0.46194,-0.46194,-0.19134,0.19134,0.46194,
        0.41573,-0.09755,-0.49039,-0.27779,0.27779,0.49039,0.09755,-0.41573,
        0.35355,-0.35355,-0.35355,0.35355,0.35355,-0.35355,-0.35355,0.35355,
        0.27779,-0.49039,0.09755,0.41573,-0.41573,-0.09755,0.49039,-0.27779,
        0.19134,-0.46194,0.46194,-0.19134,-0.19134,0.46194,-0.46194,0.19134,
        0.09755,-0.27779,0.41573,-0.49039,0.49039,-0.41573,0.27779,-0.09755
};

const float TT[8][8] = { //T的转置
        0.35355, 0.49039, 0.46194, 0.41573, 0.35355, 0.27779, 0.19134, 0.09755,
        0.35355, 0.41573, 0.19134, -0.09755, -0.35355, -0.49039, -0.46194, -0.27779,
        0.35355, 0.27779, -0.19134, -0.49039, -0.35355, 0.09755, 0.46194, 0.41573,
        0.35355, 0.09755, -0.46194, -0.27779, 0.35355, 0.41573, -0.19134, -0.49039,
        0.35355, -0.09755, -0.46194, 0.27779, 0.35355, -0.41573, -0.19134, 0.49039,
        0.35355, -0.27779, -0.19134, 0.49039, -0.35355, -0.09755, 0.46194, -0.41573,
        0.35355, -0.41573, 0.19134, 0.09755, -0.35355, 0.49039, -0.46194, 0.27779,
        0.35355, -0.49039, 0.46194, -0.41573, 0.35355, -0.27779, 0.19134, -0.09755
};


float Y[MAX][MAX];
float U[MAX][MAX];
float V[MAX][MAX];

/********jpeg算法的有关定义*********/

#ifndef 图像压缩_JPEG_H
#define 图像压缩_JPEG_H

#endif //图像压缩_JPEG_H

