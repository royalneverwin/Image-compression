#include <iostream>
#include <fstream>
#include <cmath>
#include "bmp.h"
#include "jpeg.h"
#include <string>
#include <map>
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


/*****quantization*****/
void QUANTIZ(float matrix[8][8], int yuv, int ans[8][8]){//yuv=0->lumi,yuv=1->chro
    for(int i = 0;i < 8;i++) {
        for (int j = 0; j < 8; j++) {
//            cout << matrix[i][j] << ' ';
            if (yuv == 0)
                matrix[i][j] = matrix[i][j] / DefaultQuantLuminance[i][j];
            else
                matrix[i][j] = matrix[i][j] / DefaultQuantChrominance[i][j];
            ans[i][j] = nearbyint(matrix[i][j]); //ans[i][j] = int(matrix[i][j] + matrix[i][j] >= 0? + 0.5f: -0.5f);
//            cout << matrix[i][j] << ' ' << ans[i][j] << endl;
        }
    }
}

/*****quantization*****/


/*****z-encoding*****/
int prevDC = 0;
void ZCODE(int matrix[8][8], int ans[64], int &lastZero){ //matrix[0][0] is DC, is special, using sub-encoding, else are called AC, using z-encoding
    ans[0] = matrix[0][0] - prevDC;
    prevDC = matrix[0][0];
    for(int k = 1; k < 64; k++){
        int index = ZCodeMatrix[k];
        ans[k] = matrix[index/8][index%8];
        if(ans[k] != 0)
            lastZero = k;
    }
}
/*****z-encoding*****/


/*****RLE encoding******/
void RLE(int zCode[], RLECode rleCode[], int lastZero, int& rleCnt){
    int index = 0;
    int zeroCnt = 0;
//    if(lastZero == 0 && zCode[0] == 0){ //特殊情况
//        rleCode[index].cur = rleCode[index].preZero = 0;
//        rleCode[index+1] = EOB;
//        rleCnt = index + 2;
//        return;
//    }
    rleCode[index].preZero = 0;
    rleCode[index].cur = zCode[0];
    index++;
    for(int i = 1; i <= lastZero; i++){
        if(zCode[i] == 0) {
            zeroCnt++;
            if(zeroCnt == 16){  //>=16个0，则16个0一组
                rleCode[index].preZero = 15;
                rleCode[index].cur = 0;
                zeroCnt = 0;
                index++;
            }
        }
        else{
            rleCode[index].preZero = zeroCnt;
            rleCode[index].cur = zCode[i];
            zeroCnt = 0;
            index++;
        }
    }
    rleCode[index] = EOB;
    rleCnt = index + 1;
}
/*****RLE encoding******/


/*****Huffman*****/
map<string, string> DCchro;
map<string, string> ACchro;
map<string, string> DClumi;
map<string, string> AClumi;
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

string Short2Binary(short num){ //string：left is max
    char ans[17];
    int index = 0;
    bool ifminus = false;
    if(num < 0){
        ifminus = true;
        num = short(-1) * num;
    }
    while(num != 0){
        ans[index] = (num % 2) + '0';
        index++;
        num /= 2;
    }
    while(index != 17){
        ans[index] = '0';
        index++;
    }
    if(ifminus){ //按位取反，末位+1
        for(int i = 0; i < 16; i++){
            if(ans[i] == '1')
                ans[i] = '0';
            else
                ans[i] = '1';
        }
        for(int i = 0; i < 16; i++){
            if(ans[i] == '1')
                ans[i] = '0';
            else {
                ans[i] = '1';
                break;
            }

        }
    }
    char rotateAns[17];
    for(int i = 0; i < 16; i++){
        rotateAns[i] = ans[15-i];
    }
    rotateAns[index] = '\0';
    string ret = rotateAns;
    return ret;
}

short Binary2Short(string num){ //string：left is max
    if(num[15] == 1){ //是负数，-1后按位取反
        for(int i = 15; i >= 0; i--){
            if(num[i] == '0')
                num[i] = '1';
            else{
                num[i] = '0';
                break;
            }
        }
        for(int i = 0; i < 16; i++){
            if(num[i] == '0')
                num[i] = '1';
            else
                num[i] = '0';
        }
    }
    short ans = 0;
    for(int i = 0; i < 16; i++){
        ans *= 2;
        ans += num[i] - '0';
    }
    return ans;
}

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
void InitialHuffman() {
    ifstream fDCLumi("./HuffmanTable/DC_luminance.txt");//DC_lumi
    ifstream fDCChro("./HuffmanTable/DC_chrominance.txt");//DC_chro
    ifstream fACLumi("./HuffmanTable/AC_luminance.txt");//AC_lumi
    ifstream fACChro("./HuffmanTable/AC_chrominance.txt");//AC_chro
    string first, second;
    while(fDCLumi >> first >> second){
        DClumi[first] = second;
    }
    while(fDCChro >> first >> second){
        DCchro[first] = second;
    }
    while(fACLumi >> first >> second){
        AClumi[first] = second;
    }
    while(fACChro >> first >> second){
        ACchro[first] = second;
    }
    fDCLumi.close();
    fDCChro.close();
    fACLumi.close();
    fACChro.close();
}
/***Initialze Huffman Format***/

/***Luminance Huffman***/
void LumiHuffman(int rleCnt, RLECode rleCode[], ofstream& fout){
    string ans = "";
    //处理DC
    int num = rleCode[0].cur;
    int index = getLength(num);
    char index2c[3];
    sprintf(index2c, "%d", index);
    string index2s = index2c;
    string huffman_first = DClumi[index2s];
    string huffman_second = getCode(num);
    ans += huffman_first;
    ans += huffman_second;
    //处理AC
    int pZero;
    char indexHex;
    char pZeroHex;
    for(int i = 1; i < rleCnt; i++){
        num = rleCode[i].cur;
        pZero = rleCode[i].preZero;
        if(num == -1 && pZero == -1) {//i == rleCnt - 1
            huffman_first = AClumi["0/0"];
            huffman_second = "";
        }
        else {
            index = getLength(num);
            if (index >= 0 && index <= 9)
                indexHex = index + '0';
            else
                indexHex = index - 10 + 'A';
            if (pZero >= 0 && pZero <= 9)
                pZeroHex = pZero + '0';
            else
                pZeroHex = pZero - 10 + 'A';
            char ZeroIndex2c[4];
            sprintf(ZeroIndex2c, "%c/%c", pZeroHex, indexHex);
            string ZeroIndex2s = ZeroIndex2c;
            huffman_first = AClumi[ZeroIndex2s];
            huffman_second = getCode(num);
        }
        ans += huffman_first;
        ans += huffman_second;
    }
    //将ans表示的二进制输入进文件中，注意要补0对齐（一次最少输入1字节）
    int ansLength = ans.length();
    int changePos;
    char *ans2c = new char[(ansLength + 7) / 8];
    memset(ans2c, 0, (ansLength + 7) / 8);
    for(int i = 0; i < ansLength; i++){
        index = i / 8;
        changePos = i - index * 8;
        if(ans[i] == '1')
            ans2c[index] |= (1 << changePos);
    }
    cout << ansLength << endl;
    fout.write(ans2c, (ansLength + 7) / 8);
}
/***Luminance Huffman***/

/***Chrominance Huffman***/
void ChroHuffman(int rleCnt, RLECode rleCode[], ofstream& fout){
    string ans = "";
    //处理DC
    int num = rleCode[0].cur;
    int index = getLength(num);
    char index2c[3];
    sprintf(index2c, "%d", index);
    string index2s = index2c;
    string huffman_first = DCchro[index2s];
    string huffman_second = getCode(num);
    ans += huffman_first;
    ans += huffman_second;
    //处理AC
    int pZero;
    char indexHex;
    char pZeroHex;
    for(int i = 1; i < rleCnt; i++){
        num = rleCode[i].cur;
        pZero = rleCode[i].preZero;
        if(num == -1 && pZero == -1) {//i == rleCnt - 1
            huffman_first = ACchro["0/0"];
            huffman_second = "";
        }
        else {
            index = getLength(num);
            if (index >= 0 && index <= 9)
                indexHex = index + '0';
            else
                indexHex = index - 10 + 'A';
            if (pZero >= 0 && pZero <= 9)
                pZeroHex = pZero + '0';
            else
                pZeroHex = pZero - 10 + 'A';
            char ZeroIndex2c[4];
            sprintf(ZeroIndex2c, "%c/%c", pZeroHex, indexHex);
            string ZeroIndex2s = ZeroIndex2c;
            huffman_first = ACchro[ZeroIndex2s];
            huffman_second = getCode(num);
        }
        ans += huffman_first;
        ans += huffman_second;
    }
    //将ans表示的二进制输入进文件中，注意要补0对齐（一次最少输入1字节）
    int ansLength = ans.length();
    int changePos;
    char *ans2c = new char[(ansLength + 7) / 8];
    memset(ans2c, 0, (ansLength + 7) / 8);
    for(int i = 0; i < ansLength; i++){
        index = i / 8;
        changePos = i - index * 8;
        if(ans[i] == '1')
            ans2c[index] |= (1 << changePos);
    }
    cout << ansLength << endl;
    fout.write(ans2c, (ansLength + 7) / 8);
}
/***Chrominance Huffman***/

/***Huffman encoding***/
void Huffman(int rleCnt, RLECode rleCode[], int yuv, ofstream& fout){//yuv=0->lumi,yuv=1->chro
    if(firstHuff == 0){ //if performing Huffman encoding for the first time, read 4 txt files to initialize the Huffman table
        InitialHuffman();
        firstHuff = 1;
    }
    if(yuv){ // chro
        ChroHuffman(rleCnt, rleCode, fout);
    }
    else{ // lumi
        LumiHuffman(rleCnt, rleCode, fout);
    }
}
/***Huffman encoding***/

/*****Huffman*****/


/*****jpeg compression in chunks 8x8*****/
void JPEG_DIVIDE(int r, int c, int width, int height, ofstream& fout){
    float divideY[8][8];
    float divideU[8][8];
    float divideV[8][8];
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(r + i < height && c + j < width) {
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
#ifdef DEBUG
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << divideY[i][j] << ' ' << divideU[i][j] << ' ' << divideV[i][j] << endl;
        }
    }
#endif
    // DCT
    DCT(divideY);
    DCT(divideU);
    DCT(divideV);
#ifdef DEBUG
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << divideY[i][j]<< ' ';
        }
        cout << endl;
    }
    cout <<divideY[0][0] << endl;
    cout <<divideU[0][0] << endl;
    cout <<divideV[0][0] << endl;
#endif
    //quantization
    int processedY[8][8];
    int processedU[8][8];
    int processedV[8][8];
    QUANTIZ(divideY,0, processedY);
    QUANTIZ(divideU,1, processedU);
    QUANTIZ(divideV,1, processedV);
    //z-encoding
    int lastPosBeforeZeroY = 0; // find last coefficient which is not zero (because trailing zeros are encoded differently)
    int z_encodeY[64];
    int lastPosBeforeZeroU = 0; // find last coefficient which is not zero (because trailing zeros are encoded differently)
    int z_encodeU[64];
    int lastPosBeforeZeroV = 0; // find last coefficient which is not zero (because trailing zeros are encoded differently)
    int z_encodeV[64];
    ZCODE(processedY, z_encodeY, lastPosBeforeZeroY);
    ZCODE(processedU, z_encodeU, lastPosBeforeZeroU);
    ZCODE(processedV, z_encodeV, lastPosBeforeZeroV);
    //RLE encoding
    RLECode rleCodeY[64];
    RLECode rleCodeU[64];
    RLECode rleCodeV[64];
    int RLECntY = 0;
    int RLECntU = 0;
    int RLECntV = 0;
    RLE(z_encodeY, rleCodeY, lastPosBeforeZeroY, RLECntY);
    RLE(z_encodeU, rleCodeU, lastPosBeforeZeroU, RLECntU);
    RLE(z_encodeV, rleCodeV, lastPosBeforeZeroV, RLECntV);
#ifdef DEBUG
    for(int i = 0; i < RLECntY; i++){
        cout << '(' << rleCodeY[i].preZero << ',' << rleCodeY[i].cur << ')' << ' ';
    }
    cout << endl;
#endif
    //Huffman encoding
    Huffman(RLECntY, rleCodeY, 0, fout);
    Huffman(RLECntU, rleCodeU, 1, fout);
    Huffman(RLECntV, rleCodeV, 1, fout);
}
/*****jpeg compression in chunks 8x8*****/


/*****jpeg*****/
void MyJPEG(int width, int height, ofstream& fout){
    int r = 0;
    int c = 0;
    //Color space conversion
    /***ps: I escaped Decline Sampling**/
    for(r = 0; r < height; r++){
        for(c = 0; c < width; c++){
            Y[r][c] = rgb2y(float(red[r][c]), float(green[r][c]), float(blue[r][c])) - 128; //make its value range fall in [-128,127]
            U[r][c] = rgb2u(float(red[r][c]), float(green[r][c]), float(blue[r][c]));
            V[r][c] = rgb2v(float(red[r][c]), float(green[r][c]), float(blue[r][c]));
        }
    }
    //Block processing
    for(r = 0; r < height; r += 8){
        for(c = 0; c < width; c += 8){
            JPEG_DIVIDE(r, c, width, height, fout);
        }
    }
}
/*****jpeg*****/




int main(int argc, char *argv[]){//2 file names in the command line input, compress the photos of the first file into the second file
    ifstream fin(argv[1],ios::in|ios::binary);
    ofstream fout(argv[2], ios::out | ios::binary);
    int count = 0;
    int ifInterval = 0;//在两个Header和数据之间可能还有一些无关数据, 要都记录并且平移
    char *interval;
    int intervalCnt;
    int width;
    int height;
    if(!fin.is_open()){//若打开文件错误, 则输出错误
        cerr << "Error in opening files" << endl;
        exit(1);
    }
    fin.read((char *)&bmpFileHeader, sizeof(bmpFileHeader));//read的首项必须是char *
    fin.read((char *)&bmpInfoHeader, sizeof(bmpInfoHeader));
    count = count + sizeof(bmpFileHeader) + sizeof(bmpInfoHeader);
    width = bmpInfoHeader.biWidth;
    height = bmpInfoHeader.biHeight;
    if(width >= MAX || height >= MAX){ //set MAX = 3000 first, it can be changed, I don’t know how big the specific picture can be
        cout << width << ' ' << height << endl;
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
            int makeUpCnt = (width * 3 + 3) / 4 * 4 - width * 3;
            BYTE tmp;
            for(int j = 0; j < makeUpCnt; j++) //just throw it away after reading it
                fin.read((char *)&tmp, sizeof(tmp));
        }
    }
    else if(bmpInfoHeader.biBitCount == 32){//if it is 32 bits, the color order is BGRA (Alpha)
        char drop;
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                fin.read((char *)&blue[i][j], sizeof(blue[i][j]));
                fin.read((char *)&green[i][j], sizeof(green[i][j]));
                fin.read((char *)&red[i][j], sizeof(red[i][j]));
                fin.read((char *)&alpha[i][j], sizeof(alpha[i][j]));
                //we don't compress alpha, so we just put it back untouched
                fout.write((char *)&alpha[i][j], sizeof(alpha[i][j]));
            }
            //no need for makeUpCnt, 4 bytes of 1 pixel must be a multiple of 4, and 0 will not be added
        }
    }
    else{ //24 and 32 bits are true colors, just read the color directly, the other formats will have a palette that will index the corresponding colors, which will be more complicated

    }
#ifdef DEBUG
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            cout << blue[i][j] << ' ' << green[i][j] << ' ' << red[i][j] << endl;
        }
    }
#endif
    //jpeg compressor
    MyJPEG(width, height, fout);

    fin.close();
    fout.close();
    return 0;
}
