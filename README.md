# Image-compression

### 1. Project内容

实现基于JPEG的图像压缩算法并对bmp图片进行压缩





### 2. BMP图片简介

- BMP文件由文件头、位图信息头、颜色信息和图形数据四部分组成。文件头主要包含文件的大小、文件类型、图像数据偏离文件头的长度等信息；位图信息头包含图象的尺寸信息、图像用几个比特数值来表示一个像素、图像是否压缩、图像所用的颜色数等信息。颜色信息包含图像所用到的颜色表，显示图像时需用到这个颜色表来生成调色板，但如果图像为真彩色，既图像的每个像素用24个比特来表示，文件中就没有这一块信息，也就不需要操作调色板。文件中的数据块表示图像的相应的像素值。
- 文件存储图像的每一行像素值时，如果存储该行像素值所占的字节数为4的倍数，则正常存储，否则，需要在后端补0，凑足4的倍数。





### 3. JPEG压缩算法流程

#####(1) 色彩空间转换

- rgb空间转化成yuv空间

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%871.png?raw=true" alt="图片1.png" style="zoom:50%;" />



#####(2) 离散余弦变换（DCT）

- 分块：将缩减取样得到的Y,U,V三个分量分别建立三个区域，每个区域按照8×8分块

- 每一个8×8分块分别进行离散余弦变换

- DCT的原理：信号学中，如果一个信号长度为8字节，则可以用8个不同频率的余弦波去表示它，形成频域编码。在图像中也是如此。8×8分块可以被64个不同的余弦波加权表示

- DCT的矩阵表示：我们已经限制了DCT的作用对象是8×8块，那么可以直接算出8×8的DCT变换矩阵（矩阵存放在jpeg.h中）

  <img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%872.png?raw=true" alt="图片2.png" style="zoom:50%;" />

  

#####(3) 量化（quantization）

- 在DCT变换后，舍弃高频区数据的过程称为量化

- 人类视觉在一定频率内对亮度变化较为敏感，但是在高频区分辨能力较为迟钝，因此可以舍弃一些高频分量从而降低信息量

- 量化操作就是DCT后得到的矩阵与量化表同位置的项相除并取整

- 至少需要用到两张量化表，亮度量化表用于Y分量，色度量化表用于U,V分量（量化矩阵存放在jpeg.h中）

  ![图片3.png](https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%873.png?raw=true)

  

#####(4) z字形编码（Z encoding）

- JPEG使用z字形扫描矩阵，形成一维数组数据，类似频率的数据就会被放在一起

- （0, 0）位置的量被称为DC量，不参与z字形编码而是与前一个DC量进行差分编码，剩余63个量被称为AC量，进行z字形编码

  ![图片4.png](https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%874.png?raw=true)

  

#####(5) RLE编码（RLE encoding）

- 对z字形编码后的数据进行RLE编码

- （-1，-1）代表EOB（后面都是0），规定如果连续的0个数>=16个，那么这16个0组成一个RLE

  <img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%875.png?raw=true" alt="图片5.png" style="zoom:50%;" />

  

#####(6) Huffman编码（Huffman encoding）

- 对RLE编码后的RLE数组进行Huffman编码

- luminance (Y) 的DC、AC分量，chrominance (U, V) 的DC、AC分量对应的RLE数对的第一个数分别进行Huffman编码（使用不同的Huffman表），Huffman表已经附在Huffman table文件夹中。

- RLE数对的第二个数统一进行另一个Huffman编码，编码结果由getCode函数给出

  <img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%876.png?raw=true" alt="图片6.png" style="zoom:50%;" />

  

#####(7) 解码（上述过程的逆过程，具体过程不再赘述）





### 4. 我们的JPEG算法举例展示

##### (1) 初始 8x8 RGB矩阵

- rad矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%877.png?raw=true" alt="图片7.png" style="zoom:50%;" />

- green矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%878.png?raw=true" alt="图片8.png" style="zoom:50%;" />

- blue矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%879.png?raw=true" alt="图片9.png" style="zoom:50%;" />



##### (2) 色彩空间转换

- Y矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8710.png?raw=true" alt="图片10.png" style="zoom:50%;" />

- U矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8711.png?raw=true" alt="图片11.png" style="zoom:50%;" />

- V矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8712.png?raw=true" alt="图片12.png" style="zoom:50%;" />



##### (3) DCT变换

- 转换后的Y矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8713.png?raw=true" alt="图片13.png" style="zoom:50%;" />

- 转换后的U矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8714.png?raw=true" alt="图片14.png" style="zoom:50%;" />

- 转换后的V矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8715.png?raw=true" alt="图片15.png" style="zoom:50%;" />



##### (4) 量化

- 量化后的Y矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8716.png?raw=true" alt="图片16.png" style="zoom:50%;" />

- 量化后的U矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8717.png?raw=true" alt="图片17.png" style="zoom:50%;" />

- 量化后的V矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8718.png?raw=true" alt="图片18.png" style="zoom:50%;" />



##### (5) z字形编码

- z字形编码后的一维数组：（从上到下是Y、U、V）

![图片19.png](https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8719.png?raw=true)



##### (6) RLE编码

- RLE编码后的RLE数组：（从上到下是Y、U、V）

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8720.png?raw=true" alt="图片20.png" style="zoom:50%;" />



##### (7) Huffman编码

- Huffman编码后的二进制流：（从上到下是Y、U、V）

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8721.png?raw=true" alt="图片21.png" style="zoom:50%;" />



##### (8) 解码

- 编码的逆过程，不再给出中间步骤，直接给出最后解码出的RGB矩阵
- red矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8722.png?raw=true" alt="图片22.png" style="zoom:50%;" />

- green矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8723.png?raw=true" alt="图片23.png" style="zoom:50%;" />

- blue矩阵：

<img src="https://github.com/royalneverwin/Image-compression/blob/main/README%20images/%E5%9B%BE%E7%89%8724.png?raw=true" alt="图片24.png" style="zoom:50%;" />

- 可以看到，跟初始的RGB矩阵非常相似。以上就是我们的JPEG算法的实例展示





### 5. project各部分说明

##### (1) bmp.h

存放bmp图像结构信息，定义RGB矩阵和bmp24、bmp32矩阵

##### (2) jpeg.h

定义YUV矩阵，存放量化矩阵，存放DCT变换矩阵，存放Z encoding辅助矩阵，定义RLE数据结构，定义EOB

##### (3) compress_encoding.cpp

实现bmp图像数据读取，色彩空间压缩，压缩数据存放

##### (4) compress_decoding.cpp

实现压缩文件数据读取，压缩色彩空间解码，解码后数据存放

##### (5) judgeResult.cpp

实现两个bmp图片的PSNR和SSIM图像评价指标求解





### 6. BMP图片压缩效果展示：

##### (1) 评价原理

- 时间空间指标：压缩时间程序会通过clock() 函数给出，压缩比可以直接查看压缩前后文件大小来比较
- 主观评价：会在实例中把压缩前后的图片给大家观赏
- 客观评价：峰值信噪比和结构相似性
  - PSNR（峰值信噪比）：最普遍和使用最为广泛的图像客观评价指标，但未考虑到人眼但视觉特性，经常出现评价结果与人的主观感觉不一致的情况
  - SSIM（结构相似性）：一种全参考的图像质量评价指标，它分别从亮度、对比度、结构三方面度量图像相似性，SSIM范围[0, 1]，值越大表示图像失真越小，其在图像去噪、图像相似度评价上很多时候是优于PSNR的。
  - 实现了judgeResult.cpp，会给出两个bmp图像rbg的PSNR结果和SSIM结果。



##### (2) 具体BMP图片实例

- 一共4张bmp图片
- 



### 7. 不足和展望：





### 8. 成员分工：

- 王新昊：

  - 查阅部分资料

  - 编写bmp.h (68行)
  - 编写jpeg.h (85行)

  - 编写compress_encoding.cpp (525行)
  - 编写compress_decoding.cpp (719行)
  - 编写judgeResult.cpp (193行)
  - 使用bmp图片进行实验
  - 对算法进行优化
  - 制作ppt部分内容

- 丛浩原：

  - 查阅部分资料
  - 制作ppt部分内容









新增**judgeResult.cpp**： 使用**PSNR**和**SSIM**图像质量评价方法对两个图像进行评价

修改**decoding**和**encoding**：程序现在可以输出运行时间，可以显示压缩和解压所花时间
