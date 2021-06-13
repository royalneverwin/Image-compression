#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#pragma pack(1)

typedef unsigned char BYTE; //8位 - 1Byte
typedef unsigned short WORD; //16位 -2Byte
typedef unsigned int DWORD; //32位 -4Byte
typedef unsigned int LONG; //32位 -4Byte


const int MAX = 3000;


typedef struct tagBITMAPFILEHEADER//BMP文件头
{
    WORD bfType; // 位图文件的类型，必须为“BM”
    DWORD bfSize; // 位图文件的大小，以字节为单位
    WORD bfReserved1; // 位图文件保留字，必须为0
    WORD bfReserved2; // 位图文件保留字，必须为0
    DWORD bfOffBits; // 位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位
} BITMAPFILEHEADER;//该结构占据14个字节


typedef struct tagBITMAPINFOHEADER{//BMP位图信息头
    DWORD biSize; // 本结构所占用字节数
    LONG biWidth; // 位图的宽度，以像素为单位
    LONG biHeight; // 位图的高度，以像素为单位
    WORD biPlanes; // 目标设备的平面数不清，必须为1
    WORD biBitCount;// 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一
    DWORD biCompression; // 位图压缩类型，必须是 0(不压缩),1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
    DWORD biSizeImage; // 位图的大小，以字节为单位
    LONG biXPelsPerMeter; // 位图水平分辨率，每米像素数
    LONG biYPelsPerMeter; // 位图垂直分辨率，每米像素数
    DWORD biClrUsed;// 位图实际使用的颜色表中的颜色数
    DWORD biClrImportant;// 位图显示过程中重要的颜色数
} BITMAPINFOHEADER; //该结构占据40个字节


//typedef struct tagRGB24 {//32字节
//    BYTE rgbBlue;// 蓝色的亮度(值范围为0-255)
//    BYTE rgbGreen; // 绿色的亮度(值范围为0-255)
//    BYTE rgbRed; // 红色的亮度(值范围为0-255)
//} RGB24;
//
//
//typedef struct tagRGB32 {//32字节
//    BYTE rgbBlue;// 蓝色的亮度(值范围为0-255)
//    BYTE rgbGreen; // 绿色的亮度(值范围为0-255)
//    BYTE rgbRed; // 红色的亮度(值范围为0-255)
//    BYTE rgbReserved;// 保留，必须为0，即为Alpha
//} RGB32;



BITMAPFILEHEADER bmpFileHeader;
BITMAPINFOHEADER bmpInfoHeader;
BYTE blue[MAX][MAX];
BYTE red[MAX][MAX];
BYTE green[MAX][MAX];


int main(int argc, char *argv[]){//命令行输入有两个文件名, 将第一个文件的照片压缩放到第二个文件中
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
    if(width > MAX || height > MAX){ //这里先设MAX = 3000，可以变，我主要不知道具体图片可以有多大
        cerr << "too big" << endl;
        exit(2);
    }
    fin.read((char *)&bmpFileHeader, sizeof(bmpFileHeader));//read的首项必须是char *
    fin.read((char *)&bmpInfoHeader, sizeof(bmpInfoHeader));
    count = count + sizeof(bmpFileHeader) + sizeof(bmpInfoHeader);
    width = bmpInfoHeader.biWidth;
    height = bmpInfoHeader.biHeight;
    intervalCnt = bmpFileHeader.bfOffBits - count; //按理说中间应该没东西了，但是以防万一检查下
    if(intervalCnt != 0)
        ifInterval = 1;
    if(ifInterval){
        interval = new char[intervalCnt];
        fin.read(interval, intervalCnt);
    }
    /*我们只处理位图数据，因此前面的东西原封不动放进去*/
    fout.write((char *)&bmpFileHeader, sizeof(bmpFileHeader));
    fout.write((char *)&bmpInfoHeader, sizeof(bmpInfoHeader));
    if(ifInterval){ //位图数据和头之间还有信息
        fout.write(interval, intervalCnt);
        delete []interval;
    }
    /*处理位图数据*/
    if(bmpInfoHeader.biBitCount == 24){//如果是24位，颜色顺序是BGR

    }
    else if(bmpInfoHeader.biBitCount == 32){//如果是32位，颜色顺序是BGRA(Alpha)

    }
    else{ //24和32位是真彩色，直接读颜色就行，其余格式会有调色板将索引对应颜色，会比较复杂

    }
    fin.close();
    fout.close();
    return 0;
}