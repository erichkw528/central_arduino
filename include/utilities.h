#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
template <typename T>
T CLIP(T value, T min, T max) {
    return MAX(min, MIN(value, max));
}