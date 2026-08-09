#include "qr_code.h"

/* show the QR code right in the terminal using block characters */
void display_qr_in_terminal(QRcode* qr_code) {
    int qr_size = qr_code->width;

    printf("\n");

    /* we print two rows of the QR code at once using special characters */
    for (int y_coord = 0; y_coord < qr_size; y_coord += 2) {
        for (int x_coord = 0; x_coord < qr_size; x_coord++) {
            int top_module_index = y_coord * qr_size + x_coord;
            int bottom_module_index = (y_coord + 1) * qr_size + x_coord;

            int top_is_black = (qr_code->data[top_module_index] & 1);
            int bottom_is_black = (y_coord + 1 < qr_size) ? (qr_code->data[bottom_module_index] & 1) : 0;

            /* choose the right character based on which modules are black */
            if (top_is_black && bottom_is_black) {
                printf("█"); /* full block when both are black */
            }
            else if (top_is_black && !bottom_is_black) {
                printf("▀"); /* top half block */
            }
            else if (!top_is_black && bottom_is_black) {
                printf("▄"); /* bottom half block */
            }
            else {
                printf(" "); /* space when both are white */
            }
        }
        printf("\n");
    }
    printf("\n");
}