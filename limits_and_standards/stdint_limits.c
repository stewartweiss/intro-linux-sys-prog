#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main ()
{
    printf("Minimum values of exact-width signed integer types = %20d\n",
    INT32_MIN );

    printf("Maximum values of exact-width signed integer types = %20d\n",
    INT32_MAX );

    printf("Maximum values of exact-width unsigned integer types = %20ud\n",
    UINT32_MAX );

#if defined INT_LEAST32_MIN
    printf("Minimum values of minimum-width signed integer types = %20d\n",
    INT_LEAST32_MIN );
#endif

#if defined INT_LEAST32_MAX
    printf("Maximum values of minimum-width signed integer types = %20d\n",
    INT_LEAST32_MAX );
#endif

#if defined UINT_LEAST32_MAX
    printf("Maximum values of minimum-width unsigned integer types = %20ud\n",
    UINT_LEAST32_MAX );
#endif

#if defined  INT_FAST32_MIN
    printf("Minimum values of fastest minimum-width signed integer types = %20ld\n",
    INT_FAST32_MIN );
#endif

#if defined  UINT_FAST32_MAX
    printf("Maximum values of fastest minimum-width signed integer types = %20lu\n",
    UINT_FAST32_MAX );
#endif

    printf("Minimum value of pointer-holding signed integer type = %20ld\n",
    INTPTR_MIN );

    printf("Maximum value of pointer-holding signed integer type = %20ld\n",
    INTPTR_MAX );

    printf("Maximum value of pointer-holding unsigned integer type = %20lu\n",
    UINTPTR_MAX );

    printf("Minimum value of greatest-width signed integer type = %20jd\n",
    INTMAX_MIN );

    printf("Maximum value of greatest-width signed integer type = %20jd\n",
    INTMAX_MAX );

    printf("Maximum value of greatest-width unsigned integer type = %20lu\n",
    UINTMAX_MAX );
    return 0;
}
