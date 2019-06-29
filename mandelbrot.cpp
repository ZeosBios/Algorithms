#define _CRT_SECURE_NO_WARNINGS

#include <complex>
#include <cstdio>
#include <ctime>

typedef std::complex<double> complex;

int mandelbrot_calculate(complex c, int maxiter)
{
	complex z = c;
	int n = 0;
	for (; n<maxiter; ++n)
	{
		if (std::abs(z) >= 2.0) break;
		z = z*z + c;
	}
	return n;
}
int main(int argc, char *argv[])
{
	const int width = 78, height = 44, num_pixels = width*height;

	const complex center(-.7, 0), span(2.7, -(4 / 3.) * 2.7 * height / width);
	const complex begin = center - span / 2.;
	const int maxiter = 100000;

	unsigned int start = clock();

#pragma omp parallel for ordered schedule(dynamic)
	for (int pix = 0; pix<num_pixels; ++pix)
	{
		const int x = pix%width, y = pix / width;

		complex c = begin + complex(x * span.real() / (width + 1.0),
			y * span.imag() / (height + 1.0));

		int n = mandelbrot_calculate(c, maxiter);
		if (n == maxiter) n = 0;

#pragma omp ordered
		{
			char c = ' ';
			if (n > 0)
			{
				static const char charset[] = ".,c8M@jawrpogOQEPGJ";
				c = charset[n % (sizeof(charset) - 1)];
			}
			std::putchar(c);
			if (x + 1 == width) std::puts("|");
		}
	}
	printf("\n");
	unsigned int end = clock();
	printf("Time = %d\n", end - start);
	system("pause");
	return EXIT_SUCCESS;
}