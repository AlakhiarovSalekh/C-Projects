#include "qr_code.h"

/* save the QR code as a proper PNG image file */
void create_png_from_qr(QRcode* qr_code, const char* filename) {
    int qr_size = qr_code->width;
    int final_image_size = (qr_size + 2 * BORDER_MARGIN) * PIXEL_SIZE_PER_MODULE;

    /* try to create the output file */
    FILE* output_file = fopen(filename, "wb");
    if (!output_file) {
        fprintf(stderr, "Oops! Couldn't create file %s\n", filename);
        return;
    }

    /* set up PNG writing structures */
    png_structp png_writer = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_writer) {
        fclose(output_file);
        fprintf(stderr, "Problem setting up PNG writer\n");
        return;
    }

    png_infop png_info = png_create_info_struct(png_writer);
    if (!png_info) {
        png_destroy_write_struct(&png_writer, NULL);
        fclose(output_file);
        fprintf(stderr, "Problem setting up PNG info\n");
        return;
    }

    /* error handling for PNG library */
    if (setjmp(png_jmpbuf(png_writer))) {
        png_destroy_write_struct(&png_writer, &png_info);
        fclose(output_file);
        fprintf(stderr, "Something went wrong while writing PNG\n");
        return;
    }

    png_init_io(png_writer, output_file);

    /* tell PNG about our image - RGB colors, no transparency */
    png_set_IHDR(png_writer, png_info, final_image_size, final_image_size, 8,
        PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_writer, png_info);

    /* make space for one row of pixels at a time */
    png_bytep pixel_row = (png_bytep)malloc(3 * final_image_size * sizeof(png_byte));

    /* draw each pixel in the final image */
    for (int y_coord = 0; y_coord < final_image_size; y_coord++) {
        for (int x_coord = 0; x_coord < final_image_size; x_coord++) {
            /* figure out which QR code module this pixel belongs to */
            int qr_x = x_coord / PIXEL_SIZE_PER_MODULE - BORDER_MARGIN;
            int qr_y = y_coord / PIXEL_SIZE_PER_MODULE - BORDER_MARGIN;

            unsigned char pixel_brightness = 255; /* start with white */

            /* if we're inside the actual QR code area, check if it should be black */
            if (qr_x >= 0 && qr_x < qr_size && qr_y >= 0 && qr_y < qr_size) {
                int module_index = qr_y * qr_size + qr_x;
                if (qr_code->data[module_index] & 1) {
                    pixel_brightness = 0; /* make it black for QR modules */
                }
            }

            /* set the same value for red, green, blue to make grayscale */
            pixel_row[x_coord * 3 + 0] = pixel_brightness;
            pixel_row[x_coord * 3 + 1] = pixel_brightness;
            pixel_row[x_coord * 3 + 2] = pixel_brightness;
        }
        png_write_row(png_writer, pixel_row);
    }

    /* finish up the PNG file */
    png_write_end(png_writer, NULL);

    free(pixel_row);
    png_destroy_write_struct(&png_writer, &png_info);
    fclose(output_file);
}