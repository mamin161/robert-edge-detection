#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static float kernel_x[] = {
	-1, 0,
	 0, 1
};

static float kernel_y[] = {
	 0, 1,
	-1, 0
};

int main(void)
{
	const int REQ_COMP = 3;
	const float threshold = 50;

	const char* input_path = "input.jpg";
	int width, height, comp;
	uint8_t* input_img = stbi_load(input_path, &width, &height, &comp, REQ_COMP);

	uint8_t *output_img = (uint8_t*)malloc(width * height * REQ_COMP);

	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			float rx = 0.0f, gx = 0.0f, bx = 0.0f;
			float ry = 0.0f, gy = 0.0f, by = 0.0f;
			// DO SOMETHING HERE TO MODIFY R, G, B
			for (int i = 0; i < 2; ++i) {
				for (int j = 0; j < 2; ++j) {
					int ni = row + i;
					int nj = col + j;
					float kx = kernel_x[i * 2 + j];
					float ky = kernel_y[i * 2 + j];

					if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
						int n_pixel = (ni * width + nj) * REQ_COMP;
						rx += input_img[n_pixel] * kx;
						gx += input_img[n_pixel + 1] * kx;
						bx += input_img[n_pixel + 2] * kx;

						ry += input_img[n_pixel] * ky;
						gy += input_img[n_pixel + 1] * ky;
						by += input_img[n_pixel + 2] * ky;
					}
				}
			}

			float r, g, b;
			float gradient_x = (rx + gx + bx);
			float gradient_y = (ry + gy + by);
			float mag_gradient = (gradient_x * gradient_x) + (gradient_y * gradient_y);

			if (mag_gradient >= (threshold * threshold)) {
				r = g = b = 255;
			} else {
				r = g = b = 0;
			}

			int pixel_index = (row * width + col) * REQ_COMP;
			output_img[pixel_index]     = (uint8_t)r;
			output_img[pixel_index + 1] = (uint8_t)g;
			output_img[pixel_index + 2] = (uint8_t)b;
		}
	}

	const char* output_path = "output.jpg";
	stbi_write_jpg(output_path, width, height, REQ_COMP, output_img, 90);

	stbi_image_free(input_img);
	free(output_img);
	return 0;
}
