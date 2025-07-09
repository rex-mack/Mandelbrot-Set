#ifndef CONFIG_H
#define CONFIG_H

// Image dimensions
constexpr int IMAGE_WIDTH  = 1920;
constexpr int IMAGE_HEIGHT = 1080;

// Max iterations for Mandelbrot calculation
constexpr int MAX_ITERATIONS = 1000;

// Color palette options
enum class ColorScheme {
    CLASSIC,
    FIRE,
    OCEAN,
    NEON,
};

#endif