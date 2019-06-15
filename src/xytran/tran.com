# TRAN.COM - Common block for task xytran

int	last_therm, last_heater
char	name[LWORD, L_ARRAY]
int	lr[L_ARRAY], lt[L_ARRAY], lz[L_ARRAY]
real	lx[L_ARRAY], ly[L_ARRAY]

common /xytran/ last_therm, last_heater, lr, lt, lz, lx, ly, name
