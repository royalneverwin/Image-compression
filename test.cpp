#include <iostream>
#include <fstream>
#include <cmath>
#include "bmp.h"
#include "jpeg.h"
#include <string>
#include <map>
#include <iomanip>
#include <ctime>
using namespace std;
#pragma pack(1)


map<string, string> DCchro;
map<string, string> ACchro;
map<string, string> DClumi;
map<string, string> AClumi;
map<string, string> SecondWord;



BYTE tmpRed[8][8] = {
        138,141,142,141,136,132,127,124,
        137,143,144,143,137,133,128,125,
        136,143,145,144,138,134,129,126,
        132,138,141,141,136,134,132,130,
        129,135,139,140,137,135,134,132,
        125,130,135,138,136,134,132,131,
        122,126,132,136,134,131,128,126,
        120,123,129,134,134,132,127,123
};

BYTE tmpGreen[8][8] = {
        108,111,112,111,107,103,98,95,
        106,112,114,112,108,104,99,96,
        104,111,113,112,109,105,101,98,
        100,106,109,109,107,105,103,101,
        97,103,107,108,107,106,105,104,
        93,98,103,106,107,106,104,103,
        90,94,100,104,105,103,100,98,
        88,91,98,102,106,104,99,95

};

BYTE tmpBlue[8][8] = {
        58,61,62,61,61,59,54,51,
        56,62,64,62,63,59,54,51,
        53,61,63,62,62,59,54,51,
        53,59,62,62,64,63,60,56,
        50,56,60,61,63,62,61,58,
        46,51,56,59,60,59,57,56,
        43,47,53,57,58,55,52,50,
        41,44,51,55,57,54,50,47
};

//BYTE tmpRed[8][8] = {0};
//
//BYTE tmpGreen[8][8] = {0};
//
//BYTE tmpBlue[8][8] = {0};

float tmpY[8][8];
float tmpU[8][8];
float tmpV[8][8];

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
    float ans = 1 * y - 0.000007 * u + 1.402 * v;
    if(ans < 0)
        return 0;
    else
        return BYTE(1 * y - 0.000007 * u + 1.402 * v);
}
BYTE yuv2g(float y, float u, float v){
    float ans = 1 * y - 0.344133 * u - 0.714138 * v;
    if(ans < 0)
        return 0;
    else
        return BYTE(1 * y - 0.344133 * u - 0.714138 * v);
}
BYTE yuv2b(float y, float u, float v){
    float ans = 1 * y + 1.772 * u -0.000015 * v;
    if(ans < 0)
        return 0;
    else
        return BYTE(1 * y + 1.772 * u -0.000015 * v);
}
/*****convert from YUV to RGB*****/

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

void InitialHuffman1() {
    ifstream fDCLumi("/Users/wangxinhao/CLionProjects/图像压缩/HuffmanTable/DC_luminance.txt");//DC_lumi
    ifstream fDCChro("/Users/wangxinhao/CLionProjects/图像压缩/HuffmanTable/DC_chrominance.txt");//DC_chro
    ifstream fACLumi("/Users/wangxinhao/CLionProjects/图像压缩/HuffmanTable/AC_luminance.txt");//AC_lumi
    ifstream fACChro("/Users/wangxinhao/CLionProjects/图像压缩/HuffmanTable/AC_chrominance.txt");//AC_chro
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

void InitialHuffman2() { //Decoding, a little different from encoding
    ifstream fDCLumi("/Users/wangxinhao/CLionProjects/图像压缩/HuffmanTable/DC_luminance.txt");//DC_lumi
    ifstream fDCChro("/Users/wangxinhao/CLionProjects/图像压缩/HuffmanTable/DC_chrominance.txt");//DC_chro
    ifstream fACLumi("/Users/wangxinhao/CLionProjects/图像压缩/HuffmanTable/AC_luminance.txt");//AC_lumi
    ifstream fACChro("/Users/wangxinhao/CLionProjects/图像压缩/HuffmanTable/AC_chrominance.txt");//AC_chro
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



/*****quantization*****/
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

/*****quantization*****/


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
/*****DCT*****/
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
int prevDCC = 0;
void ZCODE(int matrix[8][8], int ans[64], int &lastZero){ //matrix[0][0] is DC, is special, using sub-encoding, else are called AC, using z-encoding
    ans[0] = matrix[0][0] - prevDCC;
    prevDCC = matrix[0][0];
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
    cout << ans << endl;
    cout << endl;
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
    cout << ans << endl;
    cout << endl;
    fout.write(ans2c, (ansLength + 7) / 8);
}
/***Chrominance Huffman***/

/***Huffman encoding***/
void Huffman(int rleCnt, RLECode rleCode[], int yuv, ofstream& fout){//yuv=0->lumi,yuv=1->chro
    if(yuv){ // chro
        ChroHuffman(rleCnt, rleCode, fout);
    }
    else{ // lumi
        LumiHuffman(rleCnt, rleCode, fout);
    }
}
/***Huffman encoding***/


/***Luminance Huffman***/
void Lumi(int rleCnt, RLECode rleCode[]){
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
    cout << ans.length() << endl;
    cout << ans << endl;
}
/***Luminance Huffman***/

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
    if(yuv){ // chro
        ChroHuffmanDecoding(rleCnt, rleCode, fin);
    }
    else{ // lumi
        LumiHuffmanDecoding(rleCnt, rleCode, fin);
    }
}
/***Huffman decoding***/


//float divideY[8][8] = {159.83,103.317,22.5995,-39.8787,-58.2235,-35.2432,4.8937,33.8395,
//246.098,196.819, 125.858, 69.5835, 50.4506, 66.6239, 98.1183, 121.287,
//256.489, 220.574, 167.64, 122.828, 102.239, 105.836, 121.362, 133.857,
//126.276, 107.823, 78.4448, 48.6092, 26.1176,13.2816, 7.94379, 6.49125,
//-24.032, -23.5819, -27.4635, -41.0875, -65.6386, -96.2623, -124.185, -140.734,
//-82.2462, -64.3334, -44.66, -43.3072, -69.761, -116.818, -165.604, -196.101,
//-71.8555, -40.5783, -2.87797, 9.93678, -17.973, -77.6061, -142.361, -183.531,
//-57.5862, -19.0756, 28.3819, 47.4004, 18.7025, -47.7375, -121.135, -168.083};


int main() {//命令行输入有两个文件名, 将第一个文件的照片顺时针旋转90度放到第二个文件中
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << short(tmpRed[r][c]) << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << short(tmpGreen[r][c]) << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << short(tmpBlue[r][c]) << ' ';
        }
        cout << endl;
    }
    cout << endl;
    ofstream fout("/Users/wangxinhao/CLionProjects/图像压缩/tmp", ios::out | ios::binary);
    InitialHuffman1();
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            tmpY[r][c] = rgb2y(float(tmpRed[r][c]), float(tmpGreen[r][c]), float(tmpBlue[r][c])) - 128; //make its value range fall in [-128,127]
            tmpU[r][c] = rgb2u(float(tmpRed[r][c]), float(tmpGreen[r][c]), float(tmpBlue[r][c]));
            tmpV[r][c] = rgb2v(float(tmpRed[r][c]), float(tmpGreen[r][c]), float(tmpBlue[r][c]));
        }
    }
    cout << "**********" << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << tmpY[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << tmpU[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << tmpV[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    // DCT
    DCT(tmpY);
    DCT(tmpU);
    DCT(tmpV);
    cout << "**********" << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << tmpY[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << tmpU[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << tmpV[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    //quantization
    int processedY[8][8];
    int processedU[8][8];
    int processedV[8][8];
    QUANTIZ(tmpY,0, processedY);
    QUANTIZ(tmpU,1, processedU);
    QUANTIZ(tmpV,1, processedV);
    cout << "**********" << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << processedY[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << processedU[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << processedV[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
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
    cout << "**********" << endl;
    for(int r = 0; r < 64; r++)
            cout << z_encodeY[r] << ' ';
    cout << endl;
    cout << endl;
    for(int r = 0; r < 64; r++)
        cout << z_encodeU[r] << ' ';
    cout << endl;
    cout << endl;
    for(int r = 0; r < 64; r++)
        cout << z_encodeV[r] << ' ';
    cout << endl;
    cout << endl;
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
    cout << "***********" << endl;
    for(int i = 0; i < RLECntY; i++){
        cout << rleCodeY[i].preZero << ',' << rleCodeY[i].cur << ' ';
    }
    cout << endl;
    cout << endl;
    for(int i = 0; i < RLECntU; i++){
        cout << rleCodeU[i].preZero << ',' << rleCodeU[i].cur << ' ';
    }
    cout << endl;
    cout << endl;
    for(int i = 0; i < RLECntV; i++){
        cout << rleCodeV[i].preZero << ',' << rleCodeV[i].cur << ' ';
    }
    cout << endl;
    cout << endl;
    //Huffman encoding
    Huffman(RLECntY, rleCodeY, 0, fout);
    Huffman(RLECntU, rleCodeU, 1, fout);
    Huffman(RLECntV, rleCodeV, 1, fout);
    fout.close();

    DClumi.clear();
    AClumi.clear();
    DCchro.clear();
    ACchro.clear();
    InitialHuffman2();
    ifstream fin("/Users/wangxinhao/CLionProjects/图像压缩/tmp", ios::in | ios::binary);
    //Huffman decoding
    RLECode rleCodeYY[64];
    RLECode rleCodeUU[64];
    RLECode rleCodeVV[64];
    int RLECntYY = 0;
    int RLECntUU = 0;
    int RLECntVV = 0;
    HuffmanDecoding(RLECntYY, rleCodeYY, 0, fin);
    HuffmanDecoding(RLECntUU, rleCodeUU, 1, fin);
    HuffmanDecoding(RLECntVV, rleCodeVV, 1, fin);
    cout << "***********" << endl;
    for(int i = 0; i < RLECntYY; i++){
        cout << rleCodeYY[i].preZero << ',' << rleCodeYY[i].cur << ' ';
    }
    cout << endl;
    cout << endl;
    for(int i = 0; i < RLECntUU; i++){
        cout << rleCodeUU[i].preZero << ',' << rleCodeUU[i].cur << ' ';
    }
    cout << endl;
    cout << endl;
    for(int i = 0; i < RLECntVV; i++){
        cout << rleCodeVV[i].preZero << ',' << rleCodeVV[i].cur << ' ';
    }
    cout << endl;
    cout << endl;
    //RLE decoding
    int lastPosBeforeZeroYY = 0;
    int lastPosBeforeZeroUU = 0;
    int lastPosBeforeZeroVV = 0;
    int z_encodeYY[64];
    int z_encodeUU[64];
    int z_encodeVV[64];
    RLEDecoding(z_encodeYY, rleCodeYY, RLECntYY, lastPosBeforeZeroYY);
    RLEDecoding(z_encodeUU, rleCodeUU, RLECntUU, lastPosBeforeZeroUU);
    RLEDecoding(z_encodeVV, rleCodeVV, RLECntVV, lastPosBeforeZeroVV);
    cout << "**********" << endl;
    for(int r = 0; r < 64; r++)
        cout << z_encodeYY[r] << ' ';
    cout << endl;
    cout << endl;
    for(int r = 0; r < 64; r++)
        cout << z_encodeUU[r] << ' ';
    cout << endl;
    cout << endl;
    for(int r = 0; r < 64; r++)
        cout << z_encodeVV[r] << ' ';
    cout << endl;
    cout << endl;
    //z-decoding
    int processedYY[8][8];
    int processedUU[8][8];
    int processedVV[8][8];
    memset(processedYY, 0, sizeof(processedYY));
    memset(processedUU, 0, sizeof(processedUU));
    memset(processedVV, 0, sizeof(processedVV));
    ZCODEDecoding(processedYY, z_encodeYY, lastPosBeforeZeroYY);
    ZCODEDecoding(processedUU, z_encodeUU, lastPosBeforeZeroUU);
    ZCODEDecoding(processedVV, z_encodeVV, lastPosBeforeZeroVV);
    cout << "**********" << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << processedYY[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << processedUU[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << processedVV[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    //inverse quantification
    float divideY[8][8];
    float divideU[8][8];
    float divideV[8][8];

    QUANTIZDecoding(divideY,0, processedYY);
    QUANTIZDecoding(divideU,1, processedUU);
    QUANTIZDecoding(divideV,1, processedVV);
    cout << "**********" << endl;
    cout << "**********" << endl;
    cout << "**********" << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << divideY[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << divideU[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << divideV[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    //inverse DCT
    DCTDecoding(divideY);
    DCTDecoding(divideU);
    DCTDecoding(divideV);
    cout << "**********" << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << divideY[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << divideU[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << divideV[r][c] << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            divideY[r][c] += 128;
            red[r][c] = yuv2r(divideY[r][c], divideU[r][c], divideV[r][c]);
//            cout << short(yuv2r(Y[r][c], U[r][c], V[r][c])) << endl;
//            cout << short(yuv2g(Y[r][c], U[r][c], V[r][c])) << endl;
//            cout << short(yuv2b(Y[r][c], U[r][c], V[r][c])) << endl;
//            cout << "red: " << short(red[r][c]) << ',';
            green[r][c] = yuv2g(divideY[r][c], divideU[r][c], divideV[r][c]);
//            cout << "green: " << short(green[r][c]) << ',';
            blue[r][c] = yuv2b(divideY[r][c], divideU[r][c], divideV[r][c]);
//            cout << "blue: " << short(blue[r][c]) << ',';
        }
    }
    cout << "**********" << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << short(red[r][c]) << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << short(green[r][c]) << ' ';
        }
        cout << endl;
    }
    cout << endl;
    for(int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            cout << short(blue[r][c]) << ' ';
        }
        cout << endl;
    }
    cout << endl;
    cout << CLOCKS_PER_SEC << endl;
    fin.close();
    return 0;
}
//    // DCT
//    DCT(divideY);
//    for(int i = 0; i < 8; i++){
//        for(int j = 0; j < 8; j++){
//            cout << divideY[i][j] << ' ';
//        }
//        cout << endl;
//    }
//    cout << "***********" << endl;
//    //quantization
//    int processedY[8][8];
//    QUANTIZ(divideY,0, processedY);
//    for(int i = 0; i < 8; i++){
//        for(int j = 0; j < 8; j++){
//            cout << processedY[i][j] << ' ';
//        }
//        cout << endl;
//    }
//    cout << "***********" << endl;
//    //z-encoding
//    int lastPosBeforeZeroY = 0; // find last coefficient which is not zero (because trailing zeros are encoded differently)
//    int z_encodeY[64];
//    ZCODE(processedY, z_encodeY, lastPosBeforeZeroY);
//    for(int i = 0; i < 64; i++){
//        cout << z_encodeY[i] << ' ';
//    }
//    cout << endl;
//    cout << "***********" << endl;
//    //RLE encoding
//    RLECode rleCodeY[64];
//    int RLECntY = 0;
//    RLE(z_encodeY, rleCodeY, lastPosBeforeZeroY, RLECntY);
//    cout << RLECntY << endl;
//    for(int i = 0; i < RLECntY; i++)
//        cout << rleCodeY[i].preZero << ' ' << rleCodeY[i].cur << endl;
//    cout << "***********" << endl;
//    //Huffman encoding
//    InitialHuffman1();
//    Lumi(RLECntY, rleCodeY);
//    int rleCnt = 8;
//    RLECode rleCode[10];
//    rleCode[0].cur = 5;
//    rleCode[0].preZero = 0;
//    rleCode[1].cur = 31;
//    rleCode[1].preZero = 0;
//    rleCode[2].cur = 45;
//    rleCode[2].preZero = 0;
//    rleCode[3].cur = 23;
//    rleCode[3].preZero = 4;
//    rleCode[4].cur = -30;
//    rleCode[4].preZero = 1;
//    rleCode[5].cur = -8;
//    rleCode[5].preZero = 0;
//    rleCode[6].cur = 1;
//    rleCode[6].preZero = 2;
//    rleCode[7].cur = -1;
//    rleCode[7].preZero = -1;
//    ofstream fout("/Users/wangxinhao/CLionProjects/图像压缩/tmp", ios::out | ios::binary);
//    LumiHuffman(rleCnt, rleCode, fout);
//    fout.close();
//    DClumi.clear();
//    DCchro.clear();
//    AClumi.clear();
//    ACchro.clear();
//    InitialHuffman2();
//    ifstream fin("/Users/wangxinhao/CLionProjects/图像压缩/tmp", ios::in | ios::binary);
//    int rleCnt1 = 0;
//    RLECode rleCode1[10];
//    LumiHuffmanDecoding(rleCnt1, rleCode1, fin);
//    cout << "***********" << endl;
//    cout << rleCnt1 << endl;
//    for(int i = 0; i < rleCnt1; i++)
//        cout << rleCode1[i].preZero << ' ' << rleCode1[i].cur << endl;
//    cout << "***********" << endl;
//    fin.close();
//    //RLE decoding
//    int lastPosBeforeZero = 0;
//    int z_encode[64];
//    RLEDecoding(z_encode, rleCode, rleCnt, lastPosBeforeZero);
//    cout << lastPosBeforeZero << endl;
//    for(int i = 0; i < 64; i++){
//        cout << z_encode[i] << ' ';
//    }
//    cout << endl;
//    cout << "***********" << endl;
//    //z-decoding
//    int processed[8][8];
//    memset(processed, 0, sizeof(processed));
//    ZCODE(processed, z_encode, lastPosBeforeZero);
//    for(int i = 0; i < 8; i++){
//        for(int j = 0; j < 8; j++){
//            cout << processed[i][j] << ' ';
//        }
//        cout << endl;
//    }
//    cout << "***********" << endl;
//    //inverse quantification
//    float divide[8][8];
//    QUANTIZDecoding(divide,0, processed);
//    for(int i = 0; i < 8; i++){
//        for(int j = 0; j < 8; j++){
//            cout << divide[i][j] << ' ';
//        }
//        cout << endl;
//    }
//    cout << "***********" << endl;
//    //inverse DCT
//    DCTDecoding(divide);
//    for(int i = 0; i < 8; i++){
//        for(int j = 0; j < 8; j++){
//            cout << divide[i][j] << ' ';
//        }
//        cout << endl;
//    }
//    cout << "***********" << endl;
