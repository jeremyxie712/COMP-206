/* FILE: A2_bmp_helpers.c is where you will code your answers for Assignment 2.
 *
 * Each of the functions below can be considered a start for you.
 *
 * You should leave all of the code as is, except for what's surrounded
 * in comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A2_bmp_headers.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

int bmp_open(char *bmp_filename, unsigned int *width,
             unsigned int *height, unsigned int *bits_per_pixel,
             unsigned int *padding, unsigned int *data_size,
             unsigned int *data_offset, unsigned char **img_data)
{

  // YOUR CODE FOR Q1 SHOULD REPLACE EVERYTHING FROM HERE
  typedef struct bit_header
  {
    int size : 32;
    int offset : 32;
    int width : 32;
    int height : 32;
    int bpp : 32;
    int pad : 32;
  } bit_header;

  FILE *ptr_file;
  bit_header *head = (bit_header *)malloc(sizeof(bit_header));
  void *temp_buf = malloc(10000);

  ptr_file = fopen(bmp_filename, "rb");
  fread(temp_buf, 32, 1, ptr_file);

  head->size = *((int *)(temp_buf + 2));
  head->offset = *((int *)(temp_buf + 10));
  head->width = *((int *)(temp_buf + 18));
  head->height = *((int *)(temp_buf + 22));
  head->bpp = *((int *)(temp_buf + 28));

  if (head->width * 3 % 4 != 0)
  {
    head->pad = 4 - (head->width * 3 % 4);
  }
  else
  {
    head->pad = 0;
  }

  *width = head->width;
  *height = head->height;
  *bits_per_pixel = head->bpp;
  *padding = head->pad;
  *data_size = head->size;
  *data_offset = head->offset;

  *img_data = (unsigned char *)malloc(head->size);
  memcpy(*img_data, temp_buf, 32);
  fread((*img_data + 32), head->size - 32, 1, ptr_file);
  fclose(ptr_file);

  free(temp_buf);
  free(head);

  // TO HERE
  return 0;
}

// We've implemented bmp_close for you. No need to modify this function
void bmp_close(unsigned char **img_data)
{

  if (*img_data != NULL)
  {
    free(*img_data);
    *img_data = NULL;
  }
}

int bmp_mask(char *input_bmp_filename, char *output_bmp_filename,
             unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max,
             unsigned char red, unsigned char green, unsigned char blue)
{
  unsigned int img_width;
  unsigned int img_height;
  unsigned int bits_per_pixel;
  unsigned int data_size;
  unsigned int padding;
  unsigned int data_offset;
  unsigned char *img_data = NULL;

  int open_return_code = bmp_open(input_bmp_filename, &img_width, &img_height, &bits_per_pixel, &padding, &data_size, &data_offset, &img_data);

  if (open_return_code)
  {
    printf("bmp_open failed. Returning from bmp_mask without attempting changes.\n");
    return -1;
  }

  // YOUR CODE FOR Q2 SHOULD REPLACE EVERYTHING FROM HERE
  unsigned int num_colors = bits_per_pixel / 8;
  unsigned char *pixel_data = img_data + data_offset;

  FILE *copy = fopen(output_bmp_filename, "wb");
  output_bmp_filename = malloc(data_size);

  memcpy(output_bmp_filename, img_data, data_size);
  for (int i = 0; i < img_width; i++)
  {
    for (int j = 0; j < img_height; j++)
    {
      if ((i >= x_min && i <= x_max) && (j >= y_min && j <= y_max))
      {
        pixel_data[j * (img_width * num_colors + padding) + i * num_colors + 0] = blue;
        pixel_data[j * (img_width * num_colors + padding) + i * num_colors + 1] = green;
        pixel_data[j * (img_width * num_colors + padding) + i * num_colors + 2] = red;
      }
      else
      {
        continue;
      }
    }
  }
  fwrite(img_data, data_size, 1, copy);
  fclose(copy);

  // TO HERE!

  bmp_close(&img_data);

  return 0;
}

int bmp_collage(char *bmp_input1, char *bmp_input2, char *bmp_result, int x_offset, int y_offset)
{

  unsigned int img_width1;
  unsigned int img_height1;
  unsigned int bits_per_pixel1;
  unsigned int data_size1;
  unsigned int padding1;
  unsigned int data_offset1;
  unsigned char *img_data1 = NULL;

  int open_return_code = bmp_open(bmp_input1, &img_width1, &img_height1, &bits_per_pixel1, &padding1, &data_size1, &data_offset1, &img_data1);

  if (open_return_code)
  {
    printf("bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input1);
    return -1;
  }

  unsigned int img_width2;
  unsigned int img_height2;
  unsigned int bits_per_pixel2;
  unsigned int data_size2;
  unsigned int padding2;
  unsigned int data_offset2;
  unsigned char *img_data2 = NULL;

  open_return_code = bmp_open(bmp_input2, &img_width2, &img_height2, &bits_per_pixel2, &padding2, &data_size2, &data_offset2, &img_data2);

  if (open_return_code)
  {
    printf("bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input2);
    return -1;
  }

  // YOUR CODE FOR Q3 SHOULD REPLACE EVERYTHING FROM HERE
  int num_colors_1 = bits_per_pixel1 / 8;
  int num_colors_2 = bits_per_pixel2 / 8;
  int new_data_size;
  int new_width;
  int new_height;
  int totalsize;
  int img1_start_col;
  int img2_start_col;
  int img1_end_col;
  int img2_end_col;
  int img1_start_row;
  int img2_start_row;
  int img1_end_row;
  int img2_end_row;
  int new_pad;
  unsigned char *pixel_data;
  unsigned char *pixel_data1 = img_data1 + data_offset1;
  unsigned char *pixel_data2 = img_data2 + data_offset2;

  FILE *ptr_file = fopen(bmp_result, "wb");

  if (num_colors_1 != num_colors_2)
  {
    printf("Bits per pixel of the two images do not match with each other. \n");
    return -1;
  }

  if (x_offset >= 0)
  {
    if (img_width2 + x_offset > img_width1)
    {
      new_width = img_width2 + x_offset;
    }
    else
    {
      new_width = img_width1;
    }
  }
  if (x_offset < 0)
  {
    if (img_width2 < img_width1 || (img_width2 - img_width1) < abs(x_offset))
    {
      new_width = img_width1 + abs(x_offset);
    }
    else
    {
      new_width = img_width2;
    }
  }

  if (y_offset >= 0)
  {
    if (img_height2 + y_offset > img_height1)
    {
      new_height = img_height2 + y_offset;
    }
    else
    {
      new_height = img_height1;
    }
  }
  if (y_offset < 0)
  {
    if (img_height2 < img_height1 || (img_height2 - img_height1) < abs(y_offset))
    {
      new_height = img_height1 + abs(y_offset);
    }
    else
    {
      new_height = img_height2;
    }
  }

  int tester = (new_width * 3) % 4;
  if (tester == 0)
  {
    new_pad = 0;
  }
  else
  {
    new_pad = 4 - (new_width * 3 % 4);
  }
  new_data_size = (new_width * num_colors_1 + new_pad) * new_height;

  int *p1 = (int *)(img_data1 + 10);
  int h1 = *p1;
  int *p2 = (int *)(img_data2 + 10);
  int h2 = *p2;

  if (h1 > h2)
  {
    totalsize = new_data_size + h1;
    bmp_result = malloc(totalsize);
    memcpy(bmp_result, img_data1, h1);
    memcpy((bmp_result + 2), &new_data_size, 4);
    memcpy((bmp_result + 18), &new_width, 4);
    memcpy((bmp_result + 22), &new_height, 4);
    //*((int *)(bmp_result + 2)) = new_data_size;
    // *(bmp_result + 18) = new_width;
    // *(bmp_result + 22) = new_height;
    pixel_data = (unsigned char *)(bmp_result + h1);
  }
  else
  {
    totalsize = new_data_size + h2;
    bmp_result = malloc(totalsize);
    memcpy(bmp_result, img_data2, h2);
    memcpy((bmp_result + 2), &new_data_size, 4);
    memcpy((bmp_result + 18), &new_width, 4);
    memcpy((bmp_result + 22), &new_height, 4);
    // *(bmp_result + 2) = new_data_size;
    // *(bmp_result + 18) = new_width;
    // *(bmp_result + 22) = new_height;
    pixel_data = (unsigned char *)(bmp_result + h2);
  }

  if (x_offset >= 0)
  {
    img1_start_col = 0;
    img1_end_col = img_width1;
    img2_start_col = x_offset;
    img2_end_col = x_offset + img_width2;
  }
  if (x_offset < 0)
  {
    img1_start_col = abs(x_offset);
    img1_end_col = abs(x_offset) + img_width1;
    img2_start_col = 0;
    img2_end_col = img_width2;
  }
  if (y_offset >= 0)
  {
    img1_start_row = 0;
    img1_end_row = img_height1;
    img2_start_row = y_offset;
    img2_end_row = y_offset + img_height2;
  }
  if (y_offset < 0)
  {
    img1_start_row = abs(y_offset);
    img1_end_row = abs(y_offset) + img_height1;
    img2_start_row = 0;
    img2_end_row = img_height2;
  }

  for (int i = 0; i < new_height; i++)
  {
    for (int j = 0; j < new_width; j++)
    {
      if (i >= img2_start_row && i < img2_end_row && j >= img2_start_col && j < img2_end_col && x_offset >= 0 && y_offset >= 0)
      {
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 0] = pixel_data2[(i - abs(y_offset)) * (img_width2 * num_colors_2 + padding2) + (j - abs(x_offset)) * num_colors_2 + 0];
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 1] = pixel_data2[(i - abs(y_offset)) * (img_width2 * num_colors_2 + padding2) + (j - abs(x_offset)) * num_colors_2 + 1];
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 2] = pixel_data2[(i - abs(y_offset)) * (img_width2 * num_colors_2 + padding2) + (j - abs(x_offset)) * num_colors_2 + 2];
      }
      else if (i >= img2_start_row && i < img2_end_row && j >= img2_start_col && j < img2_end_col && x_offset >= 0 && y_offset < 0)
      {
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 0] = pixel_data2[i * (img_width2 * num_colors_2 + padding2) + (j - abs(x_offset)) * num_colors_2 + 0];
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 1] = pixel_data2[i * (img_width2 * num_colors_2 + padding2) + (j - abs(x_offset)) * num_colors_2 + 1];
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 2] = pixel_data2[i * (img_width2 * num_colors_2 + padding2) + (j - abs(x_offset)) * num_colors_2 + 2];
      }
      else if (i >= img2_start_row && i < img2_end_row && j >= img2_start_col && j < img2_end_col && x_offset < 0 && y_offset >= 0)
      {
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 0] = pixel_data2[(i - abs(y_offset)) * (img_width2 * num_colors_2 + padding2) + j * num_colors_2 + 0];
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 1] = pixel_data2[(i - abs(y_offset)) * (img_width2 * num_colors_2 + padding2) + j * num_colors_2 + 1];
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 2] = pixel_data2[(i - abs(y_offset)) * (img_width2 * num_colors_2 + padding2) + j * num_colors_2 + 2];
      }
      else if (i >= img2_start_row && i < img2_end_row && j >= img2_start_col && j < img2_end_col && x_offset < 0 && y_offset < 0)
      {
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 0] = pixel_data2[i * (img_width2 * num_colors_2 + padding2) + j * num_colors_2 + 0];
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 1] = pixel_data2[i * (img_width2 * num_colors_2 + padding2) + j * num_colors_2 + 1];
        pixel_data[i * (new_width * num_colors_2 + new_pad) + j * num_colors_2 + 2] = pixel_data2[i * (img_width2 * num_colors_2 + padding2) + j * num_colors_2 + 2];
      }

      else if (i >= img1_start_row && i < img1_end_row && j >= img1_start_col && j < img1_end_col && x_offset >= 0 && y_offset >= 0)
      {
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 0] = pixel_data1[i * (img_width1 * num_colors_1 + padding1) + j * num_colors_1 + 0];
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 1] = pixel_data1[i * (img_width1 * num_colors_1 + padding1) + j * num_colors_1 + 1];
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 2] = pixel_data1[i * (img_width1 * num_colors_1 + padding1) + j * num_colors_1 + 2];
      }
      else if (i >= img1_start_row && i < img1_end_row && j >= img1_start_col && j < img1_end_col && x_offset >= 0 && y_offset < 0)
      {
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 0] = pixel_data1[(i - abs(y_offset)) * (img_width1 * num_colors_1 + padding1) + j * num_colors_1 + 0];
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 1] = pixel_data1[(i - abs(y_offset)) * (img_width1 * num_colors_1 + padding1) + j * num_colors_1 + 1];
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 2] = pixel_data1[(i - abs(y_offset)) * (img_width1 * num_colors_1 + padding1) + j * num_colors_1 + 2];
      }
      else if (i >= img1_start_row && i < img1_end_row && j >= img1_start_col && j < img1_end_col && x_offset < 0 && y_offset >= 0)
      {
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 0] = pixel_data1[i * (img_width1 * num_colors_1 + padding1) + (j - abs(x_offset)) * num_colors_1 + 0];
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 1] = pixel_data1[i * (img_width1 * num_colors_1 + padding1) + (j - abs(x_offset)) * num_colors_1 + 1];
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 2] = pixel_data1[i * (img_width1 * num_colors_1 + padding1) + (j - abs(x_offset)) * num_colors_1 + 2];
      }
      else if (i >= img1_start_row && i < img1_end_row && j >= img1_start_col && j < img1_end_col && x_offset < 0 && y_offset < 0)
      {
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 0] = pixel_data1[(i - abs(y_offset)) * (img_width1 * num_colors_1 + padding1) + (j - abs(x_offset)) * num_colors_1 + 0];
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 1] = pixel_data1[(i - abs(y_offset)) * (img_width1 * num_colors_1 + padding1) + (j - abs(x_offset)) * num_colors_1 + 1];
        pixel_data[i * (new_width * num_colors_1 + new_pad) + j * num_colors_1 + 2] = pixel_data1[(i - abs(y_offset)) * (img_width1 * num_colors_1 + padding1) + (j - abs(x_offset)) * num_colors_1 + 2];
      }
    }
  }
  fwrite(bmp_result, totalsize, 1, ptr_file);
  fclose(ptr_file);

  // TO HERE!

  bmp_close(&img_data1);
  bmp_close(&img_data2);

  return 0;
}
