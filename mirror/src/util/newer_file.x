include <error.h>
include <finfo.h>

# NEWER_FILE.X - test if a file has been modified since a given time.

bool procedure newer_file (file, time)

char	file[ARB]			# file name to check
long	time				# the given time

int	stat
long	ostruct[LEN_FINFO]

int	finfo()

begin
	stat = finfo (file, ostruct)

	if (stat == ERR) {
	    call eprintf ("cannot access file '%s'\n")
		call pargstr (file)
	    call erract (EA_FATAL)
	}

	return (FI_MTIME (ostruct) > time)
end
