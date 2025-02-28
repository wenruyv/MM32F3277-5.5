#include "OTSU.h"
#include "headfile.h"
//extern uint8 Middle_Point[MT9V03X_W];

//int nThershold=85;
uint8 get_thr_ys(uint8 *image, uint16 col, uint16 row)
{
    #define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale];

    for (Y = 0; Y < GrayScale; Y++)
    {
        HistGram[Y] = 0; //初始化灰度直方图
    }
    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    int16 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = Y;
          }
    }
   return Threshold;
}


/*分段阈值处理
*nThershold_far--远处阈值,nThershold_neal--近处阈值，row_far--分段行数
*/
void gray_to_2bw_otsu_ys(unsigned char *image, unsigned char *bwImage, int width, int height,int nThershold_far,int nThershold_neal,int row_far)
{
    *bwImage=0;  //对下一幅图像先进行清零
    int length, i;
		int length_far,length_near;
	  length = width * height;
		/*前row_far行处理阈值*/
	  length_far = width * (height - (height - row_far)); // 120 * (80 - row_far)
	  for(i=0; i<length_far; i++)
    {
        if(*(image+i) < nThershold_far)
        {
            *(bwImage+i) = 0;
        }
        else
        {
            *(bwImage+i) = 255;
        }
    }
		/*后50行阈值处理*/
	  for(i= length_far; i< length; i++)
    {
        if(*(image+i) < nThershold_neal)
        {
            *(bwImage+i) = 0;
        }
        else
        {
            *(bwImage+i) = 255;
        }
    }
		
}

/*---------------------------------------------------------------
【函    数】Bin_Image_Filter
【功    能】过滤噪点
【参    数】无                     -------****-------
【返 回 值】无                      -----****-------
【注意事项】#define MT9V03X_W               120	// 图像宽度  范围1-752  188
            #define MT9V03X_H               80  // 图像高度	范围1-480    120
----------------------------------------------------------------*/
void Bin_Image_Filter (uint8 bin_img[MT9V03X_H][MT9V03X_W])
{
  int16_t nr; //行
  int16_t nc; //列
  
  for (nr = 1; nr < MT9V03X_H - 1; nr++)
  {
    for (nc = 1; nc < MT9V03X_W - 1; nc = nc + 1)
    {
      if ((bin_img[nr][nc] == 0)
          && (bin_img[nr - 1][nc] + bin_img[nr - 1][nc+1] + bin_img[nr + 1][nc] + bin_img[nr][nc + 1] + bin_img[nr][nc + 2] > 3))
      {
        bin_img[nr][nc] = 1;
      }
      else if ((bin_img[nr][nc] == 1)
               && (bin_img[nr - 1][nc] + bin_img[nr - 1][nc+1]+ bin_img[nr + 1][nc] + bin_img[nr][nc + 1] + bin_img[nr][nc + 2 ] < 2))
      {
        bin_img[nr][nc] = 0;
      }
    }
  }
}


void lq_sobel (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold)
{
  //* 卷积核大小 //
  short KERNEL_SIZE = 3;
  short xStart = KERNEL_SIZE / 2;
  short xEnd = LCDW - KERNEL_SIZE / 2;
  short yStart = KERNEL_SIZE / 2;
  short yEnd = LCDH - KERNEL_SIZE / 2;
  short i, j, k;
  short temp[4];
  for (i = yStart; i < yEnd; i++)
  {
    for (j = xStart; j < xEnd; j++)
    {
      // 计算不同方向梯度幅值  //
      temp[0] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j + 1]     //{{-1, 0, 1},
        - (short) imageIn[i][j - 1] + (short) imageIn[i][j + 1]        // {-1, 0, 1},
          - (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j + 1];    // {-1, 0, 1}};
      
      temp[1] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j - 1]     //{{-1, -1, -1},
        - (short) imageIn[i - 1][j] + (short) imageIn[i + 1][j]       // { 0,  0,  0},
          - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j + 1];    // { 1,  1,  1}};
      
      temp[2] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j - 1]       //  0, -1, -1
        - (short) imageIn[i][j + 1] + (short) imageIn[i + 1][j]       //  1,  0, -1
          - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j - 1];    //  1,  1,  0
      
      temp[3] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j + 1]       // -1, -1,  0
        - (short) imageIn[i][j - 1] + (short) imageIn[i + 1][j]       // -1,  0,  1
          - (short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j + 1];    //  0,  1,  1
      
      temp[0] = abs(temp[0]);
      temp[1] = abs(temp[1]);
      temp[2] = abs(temp[2]);
      temp[3] = abs(temp[3]);
      
      // 找出梯度幅值最大值  //
      for (k = 1; k < 4; k++)
      {
        if (temp[0] < temp[k])
        {
          temp[0] = temp[k];
        }
      }
      
      if (temp[0] > Threshold)
      {
        imageOut[i][j] = 255;
      }
      else
      {
        imageOut[i][j] = 0;
      }
    }
  }
}

