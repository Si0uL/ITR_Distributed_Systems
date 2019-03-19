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

#define W 943
#define H 827
#define IMAGE_IN "logo.pgm"
#define IMAGE_OUT "logo2.pgm"
#define CONV_MATRIX_SIZE 3

/*  global variables */
int image_in[H][W];
int image_out[H][W];

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

void read_image (int image[H][W], char file_name[], int *p_h, int *p_w,
  int *p_levels) {
  FILE *fin;
  int i, j, h, w, levels ;
  char buffer[80];

  /* open P2 image */
  fin = fopen (IMAGE_IN, "r");
  if (fin == NULL){
    printf ("file open error\n");
    exit(-1);
  }
  fscanf (fin, "%s", buffer);
  if (strcmp (buffer, "P2")){
    printf ("the image format must be P2\n");
    exit(-1);
  }
  fgets (buffer, 80, fin);
  fgets (buffer, 80, fin);
  fscanf (fin, "%d%d", &w, &h);
  fscanf (fin, "%d", &levels);
  printf ("image reading ... h = %d w = %d\n", h, w);

  /* pixels reading */
  for (i = 0; i < h ; i++)
    for (j = 0; j < w; j++)
       fscanf (fin, "%d", &image[i][j]);
  fclose (fin);

  *p_h = h;
  *p_w = w;
  *p_levels = levels;
  return;
}

void write_image (int image[H][W], char file_name[], int h, int w, int levels) {
  FILE *fout;
  int i, j;

  /* open the file */
  fout=fopen(IMAGE_OUT,"w");
  if (fout == NULL){
    printf ("file opening error\n");
    exit(-1);
  }

  /* header write */
  fprintf(fout,"P2\n# test \n%d %d\n%d\n", w, h, levels);
  /* format P2, commentary, w x h points, levels */

  /* pixels writing*/
  for (i = 0; i < h ; i++)
    for (j = 0; j < w; j++)
       fprintf (fout, "%d\n", image[i][j]);

  fclose (fout);
  return;
}

int main(void) {
  int i, j, h, w, levels ;
  struct timeval tdeb, tfin;

  read_image (image_in, IMAGE_IN, &h, &w, &levels);

  /* image processing */
  gettimeofday(&tdeb, NULL);

  for (i = 0; i < h ; i++)
    for (j = 0; j < w; j++)
      image_out[i][j] = image_in[i][j];

  gettimeofday(&tfin, NULL);
  printf ("computation time (microseconds): %ld\n",
  (tfin.tv_sec - tdeb.tv_sec)*1000000 + (tfin.tv_usec - tdeb.tv_usec));

  write_image (image_out, IMAGE_OUT, h, w, levels);

  return 1;
}
