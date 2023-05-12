/* Check offloaded function's attributes and classification for OpenACC
   'kernels' (parloops version).  */

/* { dg-additional-options "--param openacc-kernels=decompose" } */

/* { dg-additional-options "-O2" }
   { dg-additional-options "-fopt-info-optimized-omp" }
   { dg-additional-options "-fopt-info-note-omp" }
   { dg-additional-options "-fdump-tree-ompexp" }
   { dg-additional-options "-fdump-tree-oaccloops1" } */

/* { dg-additional-options "--param=openacc-privatization=noisy" }
   Prune a few: uninteresting:
   { dg-prune-output {note: variable 'D\.[0-9]+' declared in block isn't candidate for adjusting OpenACC privatization level: not addressable} } */

/* { dg-additional-options "-Wopenacc-parallelism" } for testing/documenting
   aspects of that functionality.  */

#define N 1024

extern unsigned int *__restrict a;
extern unsigned int *__restrict b;
extern unsigned int *__restrict c;

void KERNELS ()
{
#pragma acc kernels copyin (a[0:N], b[0:N]) copyout (c[0:N]) /* { dg-line l_compute1 } */
  /* { dg-note {OpenACC 'kernels' decomposition: variable 'i' declared in block requested to be made addressable} {} { target *-*-* } l_compute1 } */
  /* { dg-note {variable 'i' made addressable} {} { target *-*-* } l_compute1 } */
  /* { dg-note {variable 'i' declared in block is candidate for adjusting OpenACC privatization level} {} { target *-*-* } l_compute1 } */
  /* { dg-note {variable '[abc]\.[0-9]+' declared in block isn't candidate for adjusting OpenACC privatization level: not addressable} {} { target *-*-* } l_compute1 } */
  for (unsigned int i = 0; i < N; i++)  /* { dg-message "note: forwarded loop nest in OpenACC .kernels. region to .Graphite. for analysis" } */
    /* { dg-optimized "assigned OpenACC gang vector loop parallelism" "" { target *-*-* } .-1 } */
    c[i] = a[i] + b[i];
}

/* Check the offloaded function's classification and compute dimensions (will
   always be 1 x 1 x 1 for non-offloading compilation).
   { dg-final { scan-tree-dump-times "(?n)Function is parallel_kernels_graphite OpenACC kernels offload" 1 "oaccloops1" } }
   { dg-final { scan-tree-dump-times "(?n)Compute dimensions \\\[1, 1, 1\\\]" 1 "oaccloops1" } }
   { dg-final { scan-tree-dump-times "(?n)__attribute__\\(\\(oacc function \\(1, 1, 1\\), oacc parallel_kernels_graphite, omp target entrypoint, noclone\\)\\)" 1 "oaccloops1" } } */
