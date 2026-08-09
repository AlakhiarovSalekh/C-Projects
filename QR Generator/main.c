#include "qr_code.h"

int main(int argument_count, char* argument_values[]) {
    /* check if user provided the right arguments */
    if (argument_count < 2 || argument_count > 3) {
        printf("How to use this program:\n");
        printf("  %s <your-link> [output-filename.png]\n", argument_values[0]);
        printf("  <your-link>        - The website address you want in the QR code\n");
        printf("  [output-filename.png] - Optional: Where to save the QR code image\n");
        printf("\n");
        printf("Examples:\n");
        printf("  %s \"https://devlogs.xyz\"\n", argument_values[0]);
        printf("  %s \"https://devlogs.xyz\" my_qr_code.png\n", argument_values[0]);
        printf("\nIf you don't specify an output file, it just shows the QR code in terminal.\n");
        return 1;
    }

    const char* website_url = argument_values[1];
    const char* output_filename = (argument_count == 3) ? argument_values[2] : NULL;

    /* actually generate the QR code */
    QRcode* generated_qr = QRcode_encodeString(website_url, 0, QR_ECLEVEL_M, QR_MODE_8, 1);

    if (!generated_qr) {
        fprintf(stderr, "Sorry! Couldn't create a QR code for that link.\n");
        return 1;
    }

    printf("Great! QR code created successfully!\n");
    printf("Link encoded: %s\n", website_url);
    printf("QR code size: %dx%d modules\n", generated_qr->width, generated_qr->width);

    if (output_filename) {
        /* save as PNG file */
        create_png_from_qr(generated_qr, output_filename);
        printf("QR code saved as: %s\n", output_filename);
    }
    else {
        /* just show it in the terminal */
        printf("Here's your QR code:\n");
        display_qr_in_terminal(generated_qr);
    }

    /* clean up the QR code memory */
    QRcode_free(generated_qr);

    return 0;
}