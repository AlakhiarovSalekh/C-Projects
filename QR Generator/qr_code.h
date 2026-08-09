#ifndef QR_CODE_H
#define QR_CODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qrencode.h>
#include <png.h>

/* settings for the png image output */
#define PIXEL_SIZE_PER_MODULE 10  /* how big each little QR square should be */
#define BORDER_MARGIN 4           /* white space around the QR code */

/* function declarations */
void create_png_from_qr(QRcode* qr_code, const char* filename);
void display_qr_in_terminal(QRcode* qr_code);

#endif /* QR_CODE_H */