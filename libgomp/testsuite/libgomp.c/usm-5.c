/* { dg-do run } */
/* { dg-require-effective-target offload_device } */

#include <omp.h>
#include <stdint.h>

#pragma omp requires unified_shared_memory

int
main ()
{
  int *a = (int *) omp_alloc(sizeof(int), ompx_host_mem_alloc);
  if (!a)
    __builtin_abort ();

  a[0] = 42;

  uintptr_t a_p = (uintptr_t)a;

#pragma omp target map(a[0:1])
    {
      if (a[0] != 42 || a_p == (uintptr_t)a)
	__builtin_abort ();
    }

  omp_free(a, ompx_host_mem_alloc);
  return 0;
}
