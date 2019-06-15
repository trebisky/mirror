# DEBUGD.COM - debug variables for odisp.

bool	debug			# true if debugging
int	ndebug			# counter for debugging

common /debugd/ debug, ndebug
