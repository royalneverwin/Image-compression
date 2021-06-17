#include <iostream>
#include <fstream>
#include <cmath>
#include "bmp.h"
#include "jpeg.h"
#include <string>
#include <map>
#include <ctime>
using namespace std;


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


/*****convert from YUV to RGB*****/
BYTE yuv2r (float y, float u, float v){
    int ans = nearbyint(1 * y - 0.000007 * u + 1.402 * v);
    if(ans < 0)
        return 0;
    else if(ans > 255)
        return 255;
    else
        return BYTE(ans);
}
BYTE yuv2g(float y, float u, float v){
    int ans = nearbyint(1 * y - 0.344133 * u - 0.714138 * v);
    if(ans < 0)
        return 0;
    else if(ans > 255)
        return 255;
    else
        return BYTE(ans);
}
BYTE yuv2b(float y, float u, float v){
    int ans = nearbyint(1 * y + 1.772 * u -0.000015 * v);
    if(ans < 0)
        return 0;
    else if(ans > 255)
        return 255;
    else
        return BYTE(ans);
}
/*****convert from YUV to RGB*****/


/*****DCT*****/
void DCT(float matrix[8][8]){ //G = TAT'
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


/*****Huffman*****/
map<string, string> DCchro;
map<string, string> ACchro;
map<string, string> DClumi;
map<string, string> AClumi;
map<string, string> SecondWord;
int firstHuff = 0;

int getLength(int num) { //change function, return value == length of num's Binary code( which is equal to Huffman code's length)
    if(num == 0)
        return 0;
    if(num < 0)
        num *= -1;
    int cnt = 0;
    while (num != 0) {
        cnt++;
        num /= 2;
    }
    return cnt;
}

string Short2Binary(short num){ //string：left is max, num must be positive
    char ans[17];
    int index = 0;
    while(num != 0){
        ans[index] = (num % 2) + '0';
        index++;
        num /= 2;
    }
    char rotateAns[17];
    for(int i = 0; i < index; i++){
        rotateAns[i] = ans[index-1-i];
    }
    rotateAns[index] = '\0';
    string ret = rotateAns;
    return ret;
}

//short Binary2Short(string num){ //string：left is max
//    if(num[15] == 1){ //是负数，-1后按位取反
//        for(int i = 15; i >= 0; i--){
//            if(num[i] == '0')
//                num[i] = '1';
//            else{
//                num[i] = '0';
//                break;
//            }
//        }
//        for(int i = 0; i < 16; i++){
//            if(num[i] == '0')
//                num[i] = '1';
//            else
//                num[i] = '0';
//        }
//    }
//    short ans = 0;
//    for(int i = 0; i < 16; i++){
//        ans *= 2;
//        ans += num[i] - '0';
//    }
//    return ans;
//}

string getCode(short num) { //get second Huffman code
    if(num == 0)return "";
    int index = getLength(num);
    string ans;
    if(num < 0)
        ans = Short2Binary((short(-1) * num)^(short)(pow(2, index)-1));
    else
        ans = Short2Binary(num);
    int p = index - ans.length();
    for(int i=0; i<p; i++) {
        ans = "0" + ans;
    }
    return ans;
}

/***Initialze Huffman Format***/
void InitialHuffman() { //Decoding, a little different from encoding
    ifstream fDCLumi("./HuffmanTable/DC_luminance.txt");//DC_lumi
    ifstream fDCChro("./HuffmanTable/DC_chrominance.txt");//DC_chro
    ifstream fACLumi("./HuffmanTable/AC_luminance.txt");//AC_lumi
    ifstream fACChro("./HuffmanTable/AC_chrominance.txt");//AC_chro
    string first, second;
    while(fDCLumi >> first >> second){
        DClumi[second] = first;
    }
    while(fDCChro >> first >> second){
        DCchro[second] = first;
    }
    while(fACLumi >> first >> second){
        AClumi[second] = first;
    }
    while(fACChro >> first >> second){
        ACchro[second] = first;
    }
    for(int i = -32767; i <= 32767; i++){
        if(i != 0){
            string num2Huffman = getCode(i);
            char num2c[7];
            sprintf(num2c, "%d", i);
            string num2s = num2c;
            SecondWord[num2Huffman] = num2s;
        }
    }
    fDCLumi.close();
    fDCChro.close();
    fACLumi.close();
    fACChro.close();
}
/***Initialze Huffman Format***/

/***Luminance Huffman***/
void LumiHuffmanDecoding(int& rleCnt, RLECode rleCode[], ifstream& fin){
    char ans;
    int index = 0;
    string huffman_first = "";
    string huffman_second = "";
    int numIndex;
    int preZero;
    int num;
    bool handleFirst = true;
    bool handleSecond = false;
    int ifEnd = 0;
    while(!ifEnd) {
        fin.read((char *) &ans, sizeof(ans));
        int handleCharPos = 0;
        while(handleCharPos != 8) {
            if (index == 0) { //handle DC
                if(handleFirst) {
                    if (ans & (1 << handleCharPos))
                        huffman_first += "1";
                    else
                        huffman_first += "0";
                    if(DClumi.find(huffman_first) != DClumi.end()){ //if find first in Huffman
                        string numIndex2s = DClumi.find(huffman_first)->second;
                        numIndex = atoi(numIndex2s.c_str());
                        if(numIndex == 0){ //special case
                            rleCode[index].cur = 0;
                            rleCode[index].preZero = 0;
                            index++;
                            huffman_first = "";
                        }
                        else {
                            handleFirst = false;
                            handleSecond = true;
                            huffman_first = "";
                        }
                    }
                }
                else if(handleSecond){
                    if(numIndex != 0) {
                        if (ans & (1 << handleCharPos))
                            huffman_second += "1";
                        else
                            huffman_second += "0";
                        numIndex--;
                    }
                    if(numIndex == 0) { //numIndex = 0, huffman_second build success
                        string num2s = SecondWord.find(huffman_second)->second;
                        num = atoi(num2s.c_str());
                        handleFirst = true;
                        handleSecond = false;
                        huffman_second = "";
                        rleCode[index].cur = num;
                        rleCode[index].preZero = 0;
                        index++;
                    }
                }
            }
            else{ //handle AC
                if(handleFirst) {
                    if (ans & (1 << handleCharPos))
                        huffman_first += "1";
                    else
                        huffman_first += "0";
                    if(AClumi.find(huffman_first) != AClumi.end()){ //if find first in Huffman
                        string numIndex2s = AClumi.find(huffman_first)->second;
                        char ZeroCnt = numIndex2s[0];
                        char numIndex2c = numIndex2s[2];
                        if(ZeroCnt >= '0' && ZeroCnt <= '9')
                            preZero = ZeroCnt - '0';
                        else
                            preZero = ZeroCnt - 'A' + 10;
                        if(numIndex2c >= '0' && numIndex2c <= '9')
                            numIndex = numIndex2c - '0';
                        else
                            numIndex = numIndex2c - 'A' + 10;
                        if(numIndex == 0){ //special case
                            if(preZero == 0) { //EOB
                                rleCode[index].cur = -1;
                                rleCode[index].preZero = -1;
                                index++;
                                ifEnd = true;
                            }
                            else {
                                rleCode[index].cur = 0;
                                rleCode[index].preZero = preZero;
                                index++;
                                huffman_first = "";
                            }
                        }
                        else {
                            handleFirst = false;
                            handleSecond = true;
                            huffman_first = "";
                        }
                    }
                }
                else if(handleSecond){
                    if(numIndex != 0) {
                        if (ans & (1 << handleCharPos))
                            huffman_second += "1";
                        else
                            huffman_second += "0";
                        numIndex--;
                    }
                    if(numIndex == 0){ //numIndex = 0, huffman_second build success
                        string num2s = SecondWord.find(huffman_second)->second;
                        num = atoi(num2s.c_str());
                        handleFirst = true;
                        handleSecond = false;
                        huffman_second = "";
                        rleCode[index].cur = num;
                        rleCode[index].preZero = preZero;
                        index++;
                    }
                }
            }
            handleCharPos++;
        }
    }
    rleCnt = index;
}
/***Luminance Huffman***/

/***Chrominance Huffman***/
void ChroHuffmanDecoding(int& rleCnt, RLECode rleCode[], ifstream& fin){
    char ans;
    int index = 0;
    string huffman_first = "";
    string huffman_second = "";
    int numIndex;
    int preZero;
    int num;
    bool handleFirst = true;
    bool handleSecond = false;
    int ifEnd = 0;
    while(!ifEnd) {
        fin.read((char *) &ans, sizeof(ans));
        int handleCharPos = 0;
        while(handleCharPos != 8) {
            if (index == 0) { //handle DC
                if(handleFirst) {
                    if (ans & (1 << handleCharPos))
                        huffman_first += "1";
                    else
                        huffman_first += "0";
                    if(DCchro.find(huffman_first) != DCchro.end()){ //if find first in Huffman
                        string numIndex2s = DCchro.find(huffman_first)->second;
                        numIndex = atoi(numIndex2s.c_str());
                        if(numIndex == 0){ //special case
                            rleCode[index].cur = 0;
                            rleCode[index].preZero = 0;
                            index++;
                            huffman_first = "";
                        }
                        else {
                            handleFirst = false;
                            handleSecond = true;
                            huffman_first = "";
                        }
                    }
                }
                else if(handleSecond){
                    if(numIndex != 0) {
                        if (ans & (1 << handleCharPos))
                            huffman_second += "1";
                        else
                            huffman_second += "0";
                        numIndex--;
                    }
                    if(numIndex == 0) { //numIndex = 0, huffman_second build success
                        string num2s = SecondWord.find(huffman_second)->second;
                        num = atoi(num2s.c_str());
                        handleFirst = true;
                        handleSecond = false;
                        huffman_second = "";
                        rleCode[index].cur = num;
                        rleCode[index].preZero = 0;
                        index++;
                    }
                }
            }
            else{ //handle AC
                if(handleFirst) {
                    if (ans & (1 << handleCharPos))
                        huffman_first += "1";
                    else
                        huffman_first += "0";
                    if(ACchro.find(huffman_first) != ACchro.end()){ //if find first in Huffman
                        string numIndex2s = ACchro.find(huffman_first)->second;
                        char ZeroCnt = numIndex2s[0];
                        char numIndex2c = numIndex2s[2];
                        if(ZeroCnt >= '0' && ZeroCnt <= '9')
                            preZero = ZeroCnt - '0';
                        else
                            preZero = ZeroCnt - 'A' + 10;
                        if(numIndex2c >= '0' && numIndex2c <= '9')
                            numIndex = numIndex2c - '0';
                        else
                            numIndex = numIndex2c - 'A' + 10;
                        if(numIndex == 0){ //special case
                            if(preZero == 0) { //EOB
                                rleCode[index].cur = -1;
                                rleCode[index].preZero = -1;
                                index++;
                                ifEnd = true;
                            }
                            else {
                                rleCode[index].cur = 0;
                                rleCode[index].preZero = preZero;
                                index++;
                                huffman_first = "";
                            }
                        }
                        else {
                            handleFirst = false;
                            handleSecond = true;
                            huffman_first = "";
                        }
                    }
                }
                else if(handleSecond){
                    if(numIndex != 0) {
                        if (ans & (1 << handleCharPos))
                            huffman_second += "1";
                        else
                            huffman_second += "0";
                        numIndex--;
                    }
                    if(numIndex == 0){ //numIndex = 0, huffman_second build success
                        string num2s = SecondWord.find(huffman_second)->second;
                        num = atoi(num2s.c_str());
                        handleFirst = true;
                        handleSecond = false;
                        huffman_second = "";
                        rleCode[index].cur = num;
                        rleCode[index].preZero = preZero;
                        index++;
                    }
                }
            }
            handleCharPos++;
        }
    }
    rleCnt = index;
}
/***Chrominance Huffman***/

/***Huffman decoding***/
void HuffmanDecoding(int& rleCnt, RLECode rleCode[], int yuv, ifstream& fin){//yuv=0->lumi,yuv=1->chro
    if(firstHuff == 0){ //if performing Huffman encoding for the first time, read 4 txt files to initialize the Huffman table
        InitialHuffman();
        firstHuff = 1;
    }
    if(yuv){ // chro
        ChroHuffmanDecoding(rleCnt, rleCode, fin);
    }
    else{ // lumi
        LumiHuffmanDecoding(rleCnt, rleCode, fin);
    }
}
/***Huffman decoding***/

/*****Huffman*****/


/*****RLE decoding******/
void RLEDecoding(int zCode[], RLECode rleCode[], int rleCnt, int& lastBeforeZero){
    int index = 0;
    for(int i = 0; i < rleCnt; i++){
        if(rleCode[i].preZero != -1 || rleCode[i].cur != -1) {
            for (int j = 0; j < rleCode[i].preZero; j++) {
                zCode[index] = 0;
                index++;
            }
            zCode[index] = rleCode[i].cur;
            if(rleCode[i].cur != 0)
                lastBeforeZero = index;
            index++;
        }
        else{
            break;
        }
    }
    for(; index < 64; index++)
        zCode[index] = 0;
}
/*****RLE decoding******/


/*****z-decoding*****/
int prevDC = 0;
void ZCODEDecoding(int matrix[8][8], int ans[64], int lastPosBeforeZero){ //matrix[0][0] is DC, is special, using sub-encoding, else are called AC, using z-encoding
    matrix[0][0] = ans[0] + prevDC;
    prevDC = matrix[0][0];
    for(int k = 1; k <= lastPosBeforeZero; k++){
        int index = ZCodeMatrix[k];
        matrix[index/8][index%8] = ans[k];
    }
}
/*****z-decoding*****/


/*****quantization decoding*****/
void QUANTIZDecoding(float matrix[8][8], int yuv, int ans[8][8]){//yuv=0->lumi,yuv=1->chro
    for(int i = 0;i < 8;i++) {
        for (int j = 0; j < 8; j++) {
            if (yuv == 0)
                matrix[i][j] = ans[i][j] * DefaultQuantLuminance[i][j];
            else
                matrix[i][j] = ans[i][j] * DefaultQuantChrominance[i][j];
        }
    }
}

/*****quantization decoding*****/


/*****DCT decoding*****/
void DCTDecoding(float matrix[8][8]){ //G = TAT' => A = T'GT
    float tmp[8][8];
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            float sum = 0;
            for(int k = 0; k < 8; k++){
                sum += matrix[i][k] * T[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            float sum = 0;
            for(int k = 0; k < 8; k++){
                sum += TT[i][k] * tmp[k][j];
            }
            matrix[i][j] = sum;
        }
    }
}
/*****DCT decoding*****/


/*****jpeg compression in chunks 8x8*****/
void UnpackDivide(int r, int c, int width, int height, ifstream& fin){
    //Huffman decoding
    RLECode rleCodeY[64];
    RLECode rleCodeU[64];
    RLECode rleCodeV[64];
    int RLECntY = 0;
    int RLECntU = 0;
    int RLECntV = 0;
    HuffmanDecoding(RLECntY, rleCodeY, 0, fin);
    HuffmanDecoding(RLECntU, rleCodeU, 1, fin);
    HuffmanDecoding(RLECntV, rleCodeV, 1, fin);
    //RLE decoding
    int lastPosBeforeZeroY = 0;
    int lastPosBeforeZeroU = 0;
    int lastPosBeforeZeroV = 0;
    int z_encodeY[64];
    int z_encodeU[64];
    int z_encodeV[64];
    RLEDecoding(z_encodeY, rleCodeY, RLECntY, lastPosBeforeZeroY);
    RLEDecoding(z_encodeU, rleCodeU, RLECntU, lastPosBeforeZeroU);
    RLEDecoding(z_encodeV, rleCodeV, RLECntV, lastPosBeforeZeroV);
    //z-decoding
    int processedY[8][8];
    int processedU[8][8];
    int processedV[8][8];
    memset(processedY, 0, sizeof(processedY));
    memset(processedU, 0, sizeof(processedU));
    memset(processedV, 0, sizeof(processedV));
    ZCODEDecoding(processedY, z_encodeY, lastPosBeforeZeroY);
    ZCODEDecoding(processedU, z_encodeU, lastPosBeforeZeroU);
    ZCODEDecoding(processedV, z_encodeV, lastPosBeforeZeroV);
    //inverse quantification
    float divideY[8][8];
    float divideU[8][8];
    float divideV[8][8];
    QUANTIZDecoding(divideY,0, processedY);
    QUANTIZDecoding(divideU,1, processedU);
    QUANTIZDecoding(divideV,1, processedV);
    //inverse DCT
    DCTDecoding(divideY);
    DCTDecoding(divideU);
    DCTDecoding(divideV);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(r + i < height && c + j < width) {
                Y[r + i][c + j] = divideY[i][j];
                U[r + i][c + j] = divideU[i][j];
                V[r + i][c + j] = divideV[i][j];
            }
        }
    }
}
/*****jpeg compression in chunks 8x8*****/


/*****jpeg*****/
void MyUnpack(int width, int height, ifstream& fin){ //firstly read Y, next read U, last read V
    int r = 0;
    int c = 0;
    //Block processing
    for(r = 0; r < height; r += 8){
        for(c = 0; c < width; c += 8){
            UnpackDivide(r, c, width, height, fin);
        }
    }
    //Color space conversion
    /***ps: I escaped Decline Sampling**/
    for(r = 0; r < height; r++){
        for(c = 0; c < width; c++){
            Y[r][c] += 128;
            red[r][c] = yuv2r(Y[r][c], U[r][c], V[r][c]);
            green[r][c] = yuv2g(Y[r][c], U[r][c], V[r][c]);
            blue[r][c] = yuv2b(Y[r][c], U[r][c], V[r][c]);
        }
    }
}
/*****jpeg*****/




int main(int argc, char *argv[]){//2 file names in the command line input, unpack the package of the first file into the second file
    ifstream fin(argv[1],ios::in|ios::binary);
    ofstream fout(argv[2], ios::out | ios::binary);
    int count = 0;
    int ifInterval = 0;//there may be some irrelevant data between the two Headers and the data, which must be recorded and translated
    char *interval;
    int intervalCnt;
    int width;
    int height;
    if(!fin.is_open()){//if there is an error in opening the file, an error will be output
        cerr << "Error in opening files" << endl;
        exit(1);
    }
    fin.read((char *)&bmpFileHeader, sizeof(bmpFileHeader));//must give read "char *"
    fin.read((char *)&bmpInfoHeader, sizeof(bmpInfoHeader));
    count = count + sizeof(bmpFileHeader) + sizeof(bmpInfoHeader);
    width = bmpInfoHeader.biWidth;
    height = bmpInfoHeader.biHeight;
    if(width >= MAX || height >= MAX){ //set MAX = 3000 first, it can be changed, I don’t know how big the specific picture can be
        cerr << "too big" << endl;
        exit(2);
    }
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
    if(bmpInfoHeader.biBitCount == 24){//if it is 24 bits, the color order is BGR
        //jpeg unpack
        MyUnpack(width, height, fin);
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                bmp24[i][j].rgbRed = red[i][j];
                bmp24[i][j].rgbGreen = green[i][j];
                bmp24[i][j].rgbBlue = blue[i][j];
            }
        }
        //write back
        for(int i = 0 ; i < height; i++){
            for(int j = 0; j < width; j++){
                fout.write((char *)&bmp24[i][j], sizeof(bmp24[i][j]));
            }
            //note that the bytes should also be filled here to ensure that the bytes in each row are multiples of 4.
            int makeUpNew = (width * 3 + 3) / 4 * 4 - width * 3;
            BYTE tmp = 0;
            for(int j = 0; j < makeUpNew; j++){
                fout.write((char *)&tmp, sizeof(tmp));
            }
        }
    }
    else if(bmpInfoHeader.biBitCount == 32) {//if it is 32 bits, the color order is BGRA (Alpha)
        //first read alpha
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                fin.read((char *) &alpha[i][j], sizeof(alpha[i][j]));
                bmp32[i][j].rgbReserved = alpha[i][j];
            }
        }
        //jpeg unpack
        MyUnpack(width, height, fin);
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                bmp32[i][j].rgbRed = red[i][j];
                bmp32[i][j].rgbGreen = green[i][j];
                bmp32[i][j].rgbBlue = blue[i][j];
            }
        }
        //write back
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                fout.write((char *)&bmp32[i][j], sizeof(bmp32[i][j]));
            }
            //32 is a multiple of 4, so there is no need to add 0 to the backend
        }
    }
    else{ //24 and 32 bits are true colors, just read the color directly, the other formats will have a palette that will index the corresponding colors, which will be more complicated
        cerr << "not 24/32 " << endl;
        exit(3);
    }
#ifdef DEBUG
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            cout << blue[i][j] << ' ' << green[i][j] << ' ' << red[i][j] << endl;
        }
    }
#endif
    fin.close();
    fout.close();
    cout << "The run time is " << clock() / 1000 << "ms" << endl;
    return 0;
}
