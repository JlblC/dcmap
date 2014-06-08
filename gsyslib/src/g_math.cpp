#include "common.h"
#include "../g_math.h"

namespace gsys { namespace detail {

#ifdef GSYS_FASTMATH_ENABLE

#define  TABLE_SIZE ((360 << GSYS_FASTMATH_TRIG_TABLE_DENSITY))

float g_cos_table[TABLE_SIZE+1];
float g_sin_table[TABLE_SIZE+1];
float g_tan_table[TABLE_SIZE+1];


void InitFastMath()
{
	double dd = (_d_pi*2)/(TABLE_SIZE);

	double j=0;
	int ts = TABLE_SIZE;
	for(int i=0;i<=ts;i++,j+=dd)
	{
		g_cos_table[i] = (float)::cos(j);
		g_sin_table[i] = (float)::sin(j);
		g_tan_table[i] = (float)::tan(j);
	}
}

static struct _auto_init_tables
{
	_auto_init_tables(){InitFastMath();}
} st_auto_init_tables;

#endif // GSYS_FASTMATH_ENABLE

}}


