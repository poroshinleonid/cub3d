#include "config.h"
#include "funcs.h"
#include <math.h>

float	distance(t_point a, t_point b)
{
	return (sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

float	ceil_map(float x)
{
	return (((int)x / MAP_S + 1) * MAP_S);
}

float	floor_map(float x)
{
	return (((int)x / MAP_S) * MAP_S);
}
