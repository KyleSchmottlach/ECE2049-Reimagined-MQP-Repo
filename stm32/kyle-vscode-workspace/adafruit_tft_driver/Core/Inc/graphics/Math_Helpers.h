#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

inline int32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
inline uint32_t min(uint32_t a, uint32_t b) {
  return (a < b) ? a : b;
}

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */