#include <iostream>
#include <fstream>
#include "bmp.h"
#include "jpeg.h"
using namespace std;
#pragma pack(1)


/*****convert from RGB to YUV*****/
float rgb2y (float r, float g, float b){
    return 0.299f * r + 0.587f * g + 0.114f * b;
}
float rgb2u(float r, float g, float b){
    return -0.16874f * r - 0.33126f * g + 0.5f * b;
}
float rgb2v(float r, float g, float b){
    return 0.5f * r -0.41869f * g - 0.08131f * b;
}
/*****convert from RGB to YUV*****/




/*****DCT*****/
void DCT(float matrix[][8]){ //G = TAT'
    float tmp[8][8];
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            float sum = 0;
            for(int k = 0; k < 8; k++){
                sum += matrix[i][k] * TT[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            float sum = 0;
            for(int k = 0; k < 8; k++){
                sum += T[i][k] * tmp[k][j];
            }
            matrix[i][j] = sum;
        }
    }
}
/*****DCT*****/

/*****jpeg compression in chunks 8x8*****/
void JPEG_DIVIDE(int r, int c, int width, int height){
    float divideY[8][8];
    float divideU[8][8];
    float divideV[8][8];
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(r + i < width && c + j < height) {
                divideY[i][j] = Y[r + i][c + j];
                divideU[i][j] = U[r + i][c + j];
                divideV[i][j] = V[r + i][c + j];
            }
            else{
                divideY[i][j] = 0;
                divideU[i][j] = 0;
                divideV[i][j] = 0;
            }
        }
    }
    //DCT
    DCT(divideY);
    DCT(divideU);
    DCT(divideV);
    //quantization

}
/*****jpeg compression in chunks 8x8*****/



/*****jpeg*****/
void MyJPEG(int width, int height){
    int r = 0;
    int c = 0;
    //Color space conversion
    /***ps: I escaped Decline Sampling**/
    for(r = 0; r < width; r++){
        for(c = 0; c < height; c++){
            Y[r][c] = rgb2y(float(red[r][c]), float(green[r][c]), float(blue[r][c])) - 128; //make its value range fall in [-128,127]
            U[r][c] = rgb2u(float(red[r][c]), float(green[r][c]), float(blue[r][c]));
            V[r][c] = rgb2v(float(red[r][c]), float(green[r][c]), float(blue[r][c]));
        }
    }
    //Block processing
    for(r = 0; r < width; r += 8){
        for(c = 0; c < height; c += 8){
            JPEG_DIVIDE(r, c, width, height);
        }
    }
}
/*****jpeg*****/




int main(int argc, char *argv[]){//2 file names in the command line input, compress the photos of the first file into the second file
    ifstream fin(argv[1],ios::in|ios::binary);
    ofstream fout(argv[2], ios::out | ios::binary);
    int count = 0;
    int ifInterval = 0;//there may be some irrelevant data between the 2 Headers and the data, which must be recorded and translated
    char *interval;
    int intervalCnt;
    int makeUpCnt;
    int width;
    int height;
    if(!fin.is_open()){//if there is an error in opening the file, an error will be output
        cerr << "Error in opening files" << endl;
        exit(1);
    }
    if(width > MAX || height > MAX){ //set MAX = 3000 first, it can be changed, I don’t know how big the specific picture can be
        cerr << "too big" << endl;
        exit(2);
    }
    fin.read((char *)&bmpFileHeader, sizeof(bmpFileHeader));//the first item of read must be char *
    fin.read((char *)&bmpInfoHeader, sizeof(bmpInfoHeader));
    count = count + sizeof(bmpFileHeader) + sizeof(bmpInfoHeader);
    width = bmpInfoHeader.biWidth;
    height = bmpInfoHeader.biHeight;
    intervalCnt = bmpFileHeader.bfOffBits - count; //normally there is nothing in the middle, but just in case check it
    if(intervalCnt != 0)
        ifInterval = 1;
    if(ifInterval){
        interval = new char[intervalCnt];
        fin.read(interval, intervalCnt);
    }
    //we only deal with bitmap data, so the previous stuff is left intact
    fout.write((char *)&bmpFileHeader, sizeof(bmpFileHeader));
    fout.write((char *)&bmpInfoHeader, sizeof(bmpInfoHeader));
    if(ifInterval){ //if there is still information between the bitmap data and the header
        fout.write(interval, intervalCnt);
        delete []interval;
    }
    //read bitmap data
    if(bmpInfoHeader.biBitCount == 24){//if it is 24 bits, the color order is BGR
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                fin.read((char *)&blue[i][j], sizeof(blue[i][j]));
                fin.read((char *)&green[i][j], sizeof(green[i][j]));
                fin.read((char *)&red[i][j], sizeof(red[i][j]));
            }
            //each line of bmp has an alignment principle, the number of bytes in each line must be a multiple of 4, the rest is replaced by 0.
            //makeup is the number of bytes
            makeUpCnt = (width * 3 + 3) / 4 * 4 - width * 3;
            BYTE tmp;
            for(int j = 0; j < makeUpCnt; j++) //just throw it away after reading it
                fin.read((char *)&tmp, sizeof(tmp));
        }
    }
    else if(bmpInfoHeader.biBitCount == 32){//if it is 32 bits, the color order is BGRA (Alpha)
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                fin.read((char *)&blue[i][j], sizeof(blue[i][j]));
                fin.read((char *)&green[i][j], sizeof(green[i][j]));
                fin.read((char *)&red[i][j], sizeof(red[i][j]));
            }
            //no need for makeUpCnt, 4 bytes of 1 pixel must be a multiple of 4, and 0 will not be added
        }
    }
    else{ //24 and 32 bits are true colors, just read the color directly, the other formats will have a palette that will index the corresponding colors, which will be more complicated

    }
    //jpeg compressor
    MyJPEG(width, height);

    fin.close();
    fout.close();
    return 0;
}