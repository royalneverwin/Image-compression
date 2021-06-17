/*****基于峰值信噪比比较图片压缩效果*****/

#include <iostream>
#include <fstream>
#include <cmath>
#include "bmp.h"
#include <string>
#include <map>
#include <ctime>
using namespace std;

BYTE ResultRed[MAX][MAX];
BYTE ResultGreen[MAX][MAX];
BYTE ResultBlue[MAX][MAX];
BYTE ResultAlpha[MAX][MAX];


/*****judge result with PSNR*****/
void JudgePSNR(int height, int width){
    double MSERed = 0;
    double MSEGreen = 0;
    double MSEBlue = 0;
    double PSNRRed;
    double PSNRGreen;
    double PSNRBlue;
    //求MSE
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            MSERed += pow(int(red[i][j]) - int(ResultRed[i][j]), 2);
            MSEGreen += pow(int(green[i][j]) - int(ResultGreen[i][j]), 2);
            MSEBlue += pow(int(blue[i][j]) - int(ResultBlue[i][j]), 2);
        }
    }
    MSERed /= float(height * width);
    MSEGreen /= float(height * width);
    MSEBlue /= float(height * width);
    //求PSNR
    PSNRRed = 10 * log10(255 * 255 / MSERed);
    PSNRGreen = 10 * log10(255 * 255 / MSEGreen);
    PSNRBlue = 10 * log10(255 * 255 / MSEBlue);
    cout << "PSNR of red is " << PSNRRed << endl;
    cout << "PSNR of green is " << PSNRGreen << endl;
    cout << "PSNR of blue is " << PSNRBlue << endl;
}
/*****judge result with PSNR*****/


/*****judge result with SSIM*****/
void JudgeSSIM(int height, int width){
    double C1 = pow(0.01 * 255, 2), C2 = pow(0.03 * 255, 2);
    //求average
    double averageRed1 = 0;
    double averageRed2 = 0;
    double averageGreen1 = 0;
    double averageGreen2 = 0;
    double averageBlue1 = 0;
    double averageBlue2 = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++) {
            averageRed1 += red[i][j];
            averageRed2 += ResultRed[i][j];
            averageGreen1 += green[i][j];
            averageGreen2 += ResultGreen[i][j];
            averageBlue1 += blue[i][j];
            averageBlue2 += ResultBlue[i][j];
        }
    }
    averageRed1 /= float(height * width);
    averageRed2 /= float(height * width);
    averageGreen1 /= float(height * width);
    averageGreen2 /= float(height * width);
    averageBlue1 /= float(height * width);
    averageBlue2 /= float(height * width);
    //求标准差s, 协方差ss
    double sRed1 = 0;
    double sRed2 = 0;
    double sGreen1 = 0;
    double sGreen2 = 0;
    double sBlue1 = 0;
    double sBlue2 = 0;
    double ssRed = 0;
    double ssGreen = 0;
    double ssBlue = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            sRed1 += pow(float(red[i][j]) - averageRed1, 2);
            sRed2 += pow(float(ResultRed[i][j]) - averageRed2, 2);
            sGreen1 += pow(float(green[i][j]) - averageGreen1, 2);
            sGreen2 += pow(float(ResultGreen[i][j]) - averageGreen2, 2);
            sBlue1 += pow(float(blue[i][j]) - averageBlue1, 2);
            sBlue2 += pow(float(ResultBlue[i][j]) - averageBlue2, 2);
            ssRed += abs((float(red[i][j]) - averageRed1) * (float(ResultRed[i][j]) - averageRed2));
            ssGreen += abs((float(green[i][j]) - averageGreen1) * (float(ResultGreen[i][j]) - averageGreen2));
            ssBlue += abs((float(blue[i][j]) - averageBlue1) * (float(ResultBlue[i][j]) - averageBlue2));
        }
    }
    sRed1 = sqrt(sRed1 / float(height * width - 1));
    sRed2 = sqrt(sRed2 / float(height * width - 1));
    sGreen1 = sqrt(sGreen1 / float(height * width - 1));
    sGreen2 = sqrt(sGreen2 / float(height * width - 1));
    sBlue1 = sqrt(sBlue1 / float(height * width - 1));
    sBlue2 = sqrt(sBlue2 / float(height * width - 1));
    ssRed /= float(height * width - 1);
    ssGreen /= float(height * width - 1);
    ssBlue /= float(height * width - 1);
    //计算ssim
    double SSIMRed = (2 * averageRed1 * averageRed2 + C1) * (2 * ssRed + C2) / ((averageRed1 * averageRed1 + averageRed2 * averageRed2 + C1) * (sRed1 * sRed1 + sRed2 * sRed2 + C2));
    double SSIMGreen = (2 * averageGreen1 * averageGreen2 + C1) * (2 * ssGreen + C2) / ((averageGreen1 * averageGreen1 + averageGreen2 * averageGreen2 + C1) * (sGreen1 * sGreen1 + sGreen2 * sGreen2 + C2));
    double SSIMBlue = (2 * averageBlue1 * averageBlue2 + C1) * (2 * ssBlue + C2) / ((averageBlue1 * averageBlue1 + averageBlue2 * averageBlue2 + C1) * (sBlue1 * sBlue1 + sBlue2 * sBlue2 + C2));
    cout << "SSIM of red is " << SSIMRed << endl;
    cout << "SSIM of green is " << SSIMGreen << endl;
    cout << "SSIM of blue is " << SSIMBlue << endl;
}
/*****judge result with SSIM*****/


int main(int argc, char *argv[]){//2 file names in the command line input, compare the result of compressing
    ifstream fin1(argv[1],ios::in|ios::binary);
    ifstream fin2(argv[2], ios::in | ios::binary);
    int count = 0;
    int ifInterval = 0;//there may be some irrelevant data between the two Headers and the data, which must be recorded and translated
    char *interval;
    int intervalCnt;
    int width;
    int height;
    if(!fin1.is_open() || !fin2.is_open()){//if there is an error in opening the file, an error will be output
        cerr << "Error in opening files" << endl;
        exit(1);
    }
    fin1.read((char *)&bmpFileHeader, sizeof(bmpFileHeader));//must give read "char *"
    fin1.read((char *)&bmpInfoHeader, sizeof(bmpInfoHeader));
    fin2.read((char *)&bmpFileHeader, sizeof(bmpFileHeader));//must give read "char *"
    fin2.read((char *)&bmpInfoHeader, sizeof(bmpInfoHeader));
    count = count + sizeof(bmpFileHeader) + sizeof(bmpInfoHeader);
    width = bmpInfoHeader.biWidth;
    height = bmpInfoHeader.biHeight;
    if(width > MAX || height > MAX){
        cerr << "too big" << endl;
        exit(2);
    }
    intervalCnt = bmpFileHeader.bfOffBits - count;
    if(intervalCnt != 0)
        ifInterval = 1;
    if(ifInterval){
        interval = new char[intervalCnt];
        fin1.read(interval, intervalCnt);
        fin2.read(interval, intervalCnt);
        delete []interval;
    }
    if(bmpInfoHeader.biBitCount == 24){//if it is 24 bits, the color order is BGR
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                fin1.read((char *)&blue[i][j], sizeof(blue[i][j]));
                fin1.read((char *)&green[i][j], sizeof(green[i][j]));
                fin1.read((char *)&red[i][j], sizeof(red[i][j]));
                fin2.read((char *)&ResultBlue[i][j], sizeof(ResultBlue[i][j]));
                fin2.read((char *)&ResultGreen[i][j], sizeof(ResultGreen[i][j]));
                fin2.read((char *)&ResultRed[i][j], sizeof(ResultRed[i][j]));
            }
            //each line of bmp has an alignment principle, the number of bytes in each line must be a multiple of 4, the rest is replaced by 0.
            //makeup is the number of bytes
            int makeUp = (width * 3 + 3) / 4 * 4 - width * 3;
            BYTE tmp;
            for(int j = 0; j < makeUp; j++){
                fin1.read((char *)&tmp, sizeof(tmp));
                fin2.read((char *)&tmp, sizeof(tmp));
            }
        }
    } else if(bmpInfoHeader.biBitCount == 32){//if it is 32 bits, the color order is BGRA (Alpha)
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                fin1.read((char *)&blue[i][j], sizeof(blue[i][j]));
                fin1.read((char *)&green[i][j], sizeof(green[i][j]));
                fin1.read((char *)&red[i][j], sizeof(red[i][j]));
                fin1.read((char *)&alpha[i][j], sizeof(alpha[i][j]));
                fin2.read((char *)&ResultBlue[i][j], sizeof(ResultBlue[i][j]));
                fin2.read((char *)&ResultGreen[i][j], sizeof(ResultGreen[i][j]));
                fin2.read((char *)&ResultRed[i][j], sizeof(ResultRed[i][j]));
                fin2.read((char *)&ResultAlpha[i][j], sizeof(ResultAlpha[i][j]));
            }
            //32 is a multiple of 4, so there is no need to add 0 to the backend
        }
    } else{
        cerr << "not 24/32 " << endl;
        exit(3);
    }
    JudgePSNR(height, width);
    cout << endl;
    JudgeSSIM(height, width);
    fin1.close();
    fin2.close();
    return 0;
}