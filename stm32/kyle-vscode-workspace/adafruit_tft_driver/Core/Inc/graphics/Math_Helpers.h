#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

    uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
    uint32_t min(uint32_t a, uint32_t b);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */