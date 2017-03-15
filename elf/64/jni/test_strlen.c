#include <jni.h>
#include <string.h>
#include <stdio.h>

#include <inttypes.h>



#define RSANUMBYTES 256  // 2048 bit key length
#define RSANUMWORDS (RSANUMBYTES / sizeof(uint32_t))



typedef struct RSAPublicKeyInstance {
  int len;  // Length of n[] in number of uint32_t
  uint32_t n0inv;  // -1 / n[0] mod 2^32
  uint32_t n[RSANUMWORDS];  // modulus as little endian array
  uint32_t rr[RSANUMWORDS];  // R^2 as little endian array
} RSAPublicKeyInstance;

typedef const RSAPublicKeyInstance * const RSAPublicKey;


// a[] -= mod
void subM(RSAPublicKey key,
                 uint32_t* a) {
  int64_t A = 0;
  int i;
  for (i = 0; i < key->len; ++i) {
    A += (uint64_t)a[i] - key->n[i];
    a[i] = (uint32_t)A;
    A >>= 32;
  }
}

// return a[] >= mod
int geM(RSAPublicKey key,
               const uint32_t* a) {
  int i;
  for (i = key->len; i;) {
    --i;
    if (a[i] < key->n[i]) return 0;
    if (a[i] > key->n[i]) return 1;
  }
  return 1;  // equal
}

// montgomery c[] += a * b[] / R % mod
void montMulAdd(RSAPublicKey key,
                       uint32_t* c,
                       const uint32_t a,
                       const uint32_t* b) {
  uint64_t A = (uint64_t)a * b[0] + c[0];
  uint32_t d0 = (uint32_t)A * key->n0inv;
  uint64_t B = (uint64_t)d0 * key->n[0] + (uint32_t)A;
  int i;

  for (i = 1; i < key->len; ++i) {
    A = (A >> 32) + (uint64_t)a * b[i] + c[i];
    B = (B >> 32) + (uint64_t)d0 * key->n[i] + (uint32_t)A;
    c[i - 1] = (uint32_t)B;
  }

  A = (A >> 32) + (B >> 32);

  c[i - 1] = (uint32_t)A;

  if (A >> 32) {
    subM(key, c);
  }
}

// montgomery c[] = a[] * b[] / R % mod
 void montMul(RSAPublicKey key,
                    uint32_t* c,
                    const uint32_t* a,
                    const uint32_t* b) {
  int i;
  for (i = 0; i < key->len; ++i) {
    c[i] = 0;
  }
  for (i = 0; i < key->len; ++i) {
    montMulAdd(key, c, a[i], b);
  }
}