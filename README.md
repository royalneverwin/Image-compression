# Image-compression

### 1. Project内容：

实现基于JPEG的图像压缩算法并对bmp图片进行压缩



### 2. BMP图片简介：

- BMP文件由文件头、位图信息头、颜色信息和图形数据四部分组成。文件头主要包含文件的大小、文件类型、图像数据偏离文件头的长度等信息；位图信息头包含图象的尺寸信息、图像用几个比特数值来表示一个像素、图像是否压缩、图像所用的颜色数等信息。颜色信息包含图像所用到的颜色表，显示图像时需用到这个颜色表来生成调色板，但如果图像为真彩色，既图像的每个像素用24个比特来表示，文件中就没有这一块信息，也就不需要操作调色板。文件中的数据块表示图像的相应的像素值。
- 文件存储图像的每一行像素值时，如果存储该行像素值所占的字节数为4的倍数，则正常存储，否则，需要在后端补0，凑足4的倍数。



### 3. JPEG压缩算法流程：

- 色彩空间转换：

  rgb空间转化成yuv空间

  ![image-20210618153226628](/Users/wangxinhao/Library/Application Support/typora-user-images/image-20210618153226628.png)

- 离散余弦变换（DCT）

- 量化（quantization）

- z字形编码（z encoding）

- RLE编码（RLE encoding）

- Huffman编码（Huffman encoding）

- 解码（上述过程的逆过程）



- encoding为压缩
- decoding为解压
- 效果预览： 
  - 743KB -> 51KB   24.bmp
  - 4.2MB -> 1.2MB   32.bmp
  - 2.5MB -> 86KB   1.bmp



新增**judgeResult.cpp**： 使用**PSNR**和**SSIM**图像质量评价方法对两个图像进行评价

修改**decoding**和**encoding**：程序现在可以输出运行时间，可以显示压缩和解压所花时间
