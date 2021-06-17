# Image-compression
以jpeg压缩图片方法为模版实现压缩bmp图片
- encoding为压缩
- decoding为解压
- 效果预览： 
  - 743KB -> 51KB   24.bmp
  - 4.2MB -> 1.2MB   32.bmp
  - 2.5MB -> 86KB   1.bmp



新增**judgeResult.cpp**： 使用**PSNR**和**SSIM**图像质量评价方法对两个图像进行评价

修改**decoding**和**encoding**：程序现在可以输出运行时间，可以显示压缩和解压所花时间
