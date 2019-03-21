/*
  a simple example of image processing: copy an initial image to an output image
  with the help of 2 functions read_image and write_image
  begin
    read the initial image from a file and copy the pixels in an array: image_in
    copy the elements of this array to a second array: image_out
    (this step will be replaced by a more elaborated processing)
    copy the elements of the second array in a file
  end

  the format of the files is the pgm format (alias P2, ASCII file that a
    standard text editor car process)
  the images are in black and white
  this format is understood by a lot of image readers (eog, display, gimp, ...)
  the (graphical) display of the images is done outside this program

  the pixel arrays are static arrays, defined in the header of the file (there
  is more place in the heap than in the stack (an obvious improvement is to use
  dynamical memory allocation (malloc) the functions are fast coded, the number
  of parameters is minimal but the program works

  constants are defined H (height), W (width), IMAGE_IN (name of the input
  file), IMAGE_OUT (output file)
  you have to change these values if you want to change the image to process

  to obtain the executable code:
  gcc -Wall -std=c99 image_processing_example.c -o image_processing
  to execute:
  ./image_processing
  to display:
  eog logo2.pgm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define CONV_MATRIX_SIZE 3

/*  global variables */

/*
static const int conv_matrix_lines[CONV_MATRIX_SIZE][CONV_MATRIX_SIZE] = {
    { -3, 0, 3 },
    { -1, 0, 1 },
    { -3, 0, 3 }
};

static const int conv_matrix_columns[CONV_MATRIX_SIZE][CONV_MATRIX_SIZE] = {
    { -3, -1, -3 },
    { 0, 0, 0 },
    { 3, 1, 3 }
};
*/

static const int conv_matrix_borders[CONV_MATRIX_SIZE][CONV_MATRIX_SIZE] = {
    { -1, -1, -1 },
    { -1, 8, -1 },
    { -1, -1, -1 }
};

void read_image (int **image, char file_name[], int *p_height, int *p_width,
  int *p_levels) {
  FILE *fin;
  int row, col;
  char buffer[80];

  /* open P2 image */
  fin = fopen (file_name, "r");
  if (fin == NULL){
    printf ("file open error\n");
    exit(-1);
  }
  fscanf (fin, "%s", buffer);
  if (strcmp (buffer, "P2")) {
    printf ("the image format must be P2\n");
    exit(-1);
  }
  fgets (buffer, 80, fin);
  fgets (buffer, 80, fin);
  fscanf (fin, "%d%d", p_width, p_height);
  fscanf (fin, "%d", p_levels);
  printf ("image reading ... h = %d w = %d\n", *p_height, *p_width);

  /* pixels reading */
  *image = calloc((*p_height) * (*p_width), sizeof(int));
  for (row = 0; row < *p_height; row++)
    for (col = 0; col < *p_width; col++)
       fscanf (fin, "%d", &((*image)[row * (*p_width) + col]));
  fclose (fin);

  return;
}

void write_image (int *image, char file_name[], int height, int width,
  int levels) {
  FILE *fout;
  int row, col;

  /* open the file */
  fout=fopen(file_name, "w");
  if (fout == NULL){
    printf ("file opening error\n");
    exit(-1);
  }

  /* header write */
  fprintf(fout,"P2\n# test \n%d %d\n%d\n", width, height, levels);
  /* format P2, commentary, w x h points, levels */

  /* pixels writing*/
  for (row = 0; row < height; row++)
    for (col = 0; col < width; col++)
       fprintf (fout, "%d\n", image[row * width + col]);

  fclose (fout);
  return;
}

void convolution(int *input, int *output, int height, int width,
  const int conv_matrix[CONV_MATRIX_SIZE][CONV_MATRIX_SIZE]) {
  // Recopy borders
  for (int idx=0; idx < width; idx++) {
    output[0 * width + idx] = 0;
    output[(height-1) * width + idx] = 0;
  }
  for (int idx=0; idx < height; idx++) {
    output[idx * width + 0] = 0;
    output[idx * width + width-1] = 0;
  }

  // Apply convolution elsewhere
  int row, col, r_conv, c_conv, sum;
  // Convolution offset
  int c_off = (int) CONV_MATRIX_SIZE / 2;
  for (row = 1; row < height-1; row ++) {
    for (col = 1; col < width-1; col ++) {
      sum = 0;
      for (r_conv = 0; r_conv < CONV_MATRIX_SIZE; r_conv ++) {
        for (c_conv = 0; c_conv < CONV_MATRIX_SIZE; c_conv ++) {
          sum += input[(row-c_off+r_conv) * width + (col-c_off+c_conv)] * \
            conv_matrix[r_conv][c_conv];
        }
      }
      output[row * width + col] = sum;
    }
  }
  return;
}

void saturate(int *image, int height, int width) {
  int row, col;
  for (row=0; row < height; row++) {
    for (col=0; col < width; col++) {
      if (image[row * width + col] < 0)
        image[row * width + col] = 0;
      else if (image[row * width + col] > 255)
        image[row * width + col] = 255;
    }
  }
  return;
}

int main(int argc, char const *argv[]) {

  if (argc != 2) {
    printf("Usage: %s <input_path>\n", argv[0]);
    return 1;
  }

  int height, width, levels;
  struct timeval tdeb, tfin;

  int *image_in;
  read_image (&image_in, (char *) argv[1], &height, &width, &levels);

  int *image_out = calloc(height * width, sizeof(int));

  /* image processing */
  gettimeofday(&tdeb, NULL);

  convolution(image_in, image_out, height, width, conv_matrix_borders);
  saturate(image_out, height, width);

  gettimeofday(&tfin, NULL);
  printf ("computation time (microseconds): %ld\n",
    (tfin.tv_sec - tdeb.tv_sec)*1000000 + (tfin.tv_usec - tdeb.tv_usec));

  write_image (image_out, "output.pgm", height, width, levels);

  free(image_in);
  free(image_out);

  return 0;
}
