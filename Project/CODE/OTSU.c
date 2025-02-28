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
        HistGram[Y] = 0; //��ʼ���Ҷ�ֱ��ͼ
    }
    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height��ΪY =Image_Height���Ա���� �ж�ֵ��
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //ͳ��ÿ���Ҷ�ֵ�ĸ�����Ϣ
        }
    }

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // ��䷽��;
    int16 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
    {
        return MaxValue;          // ͼ����ֻ��һ����ɫ
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // ͼ����ֻ�ж�����ɫ
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  ��������
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //ǰ�����ص���
          PixelFore = Amount - PixelBack;         //�������ص���
          OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
          OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
          PixelIntegralBack += HistGram[Y] * Y;  //ǰ���Ҷ�ֵ
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
          MicroBack = (double)PixelIntegralBack / PixelBack;//ǰ���ҶȰٷֱ�
          MicroFore = (double)PixelIntegralFore / PixelFore;//�����ҶȰٷֱ�
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//����������䷽��g
          {
              SigmaB = Sigma;
              Threshold = Y;
          }
    }
   return Threshold;
}


/*�ֶ���ֵ����
*nThershold_far--Զ����ֵ,nThershold_neal--������ֵ��row_far--�ֶ�����
*/
void gray_to_2bw_otsu_ys(unsigned char *image, unsigned char *bwImage, int width, int height,int nThershold_far,int nThershold_neal,int row_far)
{
    *bwImage=0;  //����һ��ͼ���Ƚ�������
    int length, i;
		int length_far,length_near;
	  length = width * height;
		/*ǰrow_far�д�����ֵ*/
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
		/*��50����ֵ����*/
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
����    ����Bin_Image_Filter
����    �ܡ��������
����    ������                     -------****-------
���� �� ֵ����                      -----****-------
��ע�����#define MT9V03X_W               120	// ͼ����  ��Χ1-752  188
            #define MT9V03X_H               80  // ͼ��߶�	��Χ1-480    120
----------------------------------------------------------------*/
void Bin_Image_Filter (uint8 bin_img[MT9V03X_H][MT9V03X_W])
{
  int16_t nr; //��
  int16_t nc; //��
  
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
  //* ����˴�С //
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
      // ���㲻ͬ�����ݶȷ�ֵ  //
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
      
      // �ҳ��ݶȷ�ֵ���ֵ  //
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

