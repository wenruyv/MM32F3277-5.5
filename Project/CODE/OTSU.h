#ifndef __OTSU_H_
#define __OTSU_H_
#include "common.h"
#include "SEEKFREE_MT9V03X.h"
//void gray_to_2bw_otsu_ys(unsigned char *image, unsigned char *bwImage, int width, int height,int nThershold);
void gray_to_2bw_otsu_ys(unsigned char *image, unsigned char *bwImage, int width, int height,int nThershold_far,int nThershold_neal,int row_far);
void actual_moddle(unsigned char *image);
uint8 get_thr_ys(uint8 *image, uint16 col, uint16 row);


extern int nThershold;
#define  LCDW    MT9V03X_W
#define  LCDH     MT9V03X_H
void Bin_Image_Filter (uint8 bin_img[MT9V03X_H][MT9V03X_W]);
void lq_sobel (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold);

#endif /*MYOTSU_H_*/
