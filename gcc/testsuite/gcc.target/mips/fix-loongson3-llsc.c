/* { dg-do compile } */
/* { dg-options "-mfix-loongson3-llsc" } */

NOMIPS16 int foo (int *v)
{
  return  __sync_val_compare_and_swap (v, 0, 1);
}

/* { dg-final { scan-assembler "1:\n\tsync\n\tll" } } */
/* { dg-final { scan-assembler "2:\n\tsync\n" } } */
