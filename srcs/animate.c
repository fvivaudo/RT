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

void  interpolation_linear(double n1, double n2, int frames)
{
  double ret;
  double step;
  double n;
  double i;

  i = 0;
  while (++i < frames)
  {
    n = i / frames;
    printf("n = %g ", n);
  }
}

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

float smootherstep(float edge0, float edge1, float x)
{
    // Scale, and clamp x to 0..1 range
    x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*x*(x*(x*6 - 15) + 10);
}

float clamp(float x, float lowerlimit, float upperlimit)
{
    if (x < lowerlimit) x = lowerlimit;
    if (x > upperlimit) x = upperlimit;
    return x;
}

void  interpolation_smooth(double n1, double n2, int frames)
{
  double ret;
  double step;
  double n;
  double i;

  i = 0;
  while (++i < frames)
  {
    n = i / (frames / 2);
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

// int main()
// {
//
//   printf("TEST \n");
//   interp_()
//   return 0;
// }
