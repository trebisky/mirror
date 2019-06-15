# OGRAPH.COM - common area for ograph task

int	tc_len, he_len, gp_len	# actual lengths of data information arrays

int	tc_index[L_TTMP]	# for ttmp:  index = index in data array,
int	tc_radius[L_TTMP]	#            radius, theta, z = position of
int	tc_theta[L_TTMP]	#		                thermocouples
int	tc_z[L_TTMP]

int	he_index[L_HPWR]	# for hpwr:    index = index in data array,
int	he_radius[L_HPWR]	# (and htmp)   radius, theta, z = position of
int	he_theta[L_HPWR]	#		                heater powers
int	he_z[L_HPWR]

int	gp_index[L_GPAN]	# for gpan:  index = index in data array,
char	gp_name[LWORD, L_GPAN]	#            name = name of information

int	db_n[N_XTMP]		# number of items in time schedule
long	db_time[L_XTMP, N_XTMP]	# zone time in minutes
real	db_val[L_XTMP, N_XTMP]	# zone value

char	info_line[LWORD, N_LINES]	# names of information parameters
char	radius_line[LWORD, N_LINES]	# names of radius parameters
char	theta_line[LWORD, N_LINES]	# names of theta parameters
char	z_line[LWORD, N_LINES]		# names of z parameters
char	zone_line[LWORD, N_LINES]	# names of zone parameters
char	addr_line[LWORD, N_LINES]	# names of addr parameters
char	factor_line[LWORD, N_LINES]	# names of factor parameters

int	subsample			# subsample data by n values
real	factor[N_LINES]			# factor values for each line
char	device[SZ_LINE]			# output device
char	tmp_file[SZ_PATHNAME, N_LINES]	# temporary filenames for list output

common /ographcom/ tc_len, he_len, gp_len,
		   tc_index, tc_radius, tc_theta, tc_z,
		   he_index, he_radius, he_theta, he_z,
		   gp_index, gp_name,
		   db_n, db_time, db_val,
		   info_line, radius_line, theta_line, z_line, zone_line,
		   addr_line,
		   factor_line,
		   subsample, factor,
		   device, tmp_file
