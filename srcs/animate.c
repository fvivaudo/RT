//#include <rtv1.h>


//
// t_list *interpolation(double n1, double n2, int frames,char type)
// {
//       double dif;
//       dif = n1 - n2;
//       double step;
//       step = dif / frames
//       int i;
//       t_list *ret;
//
//       ret = ft_lstnew(n1)
//       i = -1;
//       while (++i < frames)
//           ft_lstadd(&ret ,t_listnew(n1 + step * i));
//     return ret;
// }

double  *tabinterpolation_linear(double n1, double n2, int frames)
{
  double *ret;
  double dif;
  double n;
  int i;

  ret = malloc(frames * sizeof(double));
  i = 0;
  dif = n1 + (n2 - n1);
  while (++i < frames)
  {
    n = i / frames;
    ret[i - 1] =  n1+ n * dif;
  }
  return ret;
}

// double  interpolation_linear(double n1, double n2, double n)
// {
//   double ret;
//   double dif;
//   int i;
//
//   i = 0;
//   dif = ;
//   return (n1 + n * (n2 - n1));
// }

void  interpolation_slow_to_fast(double n1, double n2, int frames)
{
  double ret;
  double step;
  double n;
  double i;

  i = 0;
  while (++i < frames)
  {
    n = i / frames;
    step = n1 - (n1 - n2) * n * n;
  }
}

void  interpolation_fast_to_slow(double n1, double n2, int frames)
{
  double ret;
  double step;
  double n;
  double i;

  i = 0;
  while (++i < frames)
  {
    n = i / frames;
    n = 1 - n;
    step = n2 - (n2 - n1) * n * n;
  }
}

int main()
{

  printf("TEST \n");
  interpolation_linear(-30, 40, 20);
  return 0;
}
