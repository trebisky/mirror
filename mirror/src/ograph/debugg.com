# DEBUGG.COM - debug variables for ograph task.

bool	debug		# true if debugging
int	ndebug		# counter for debugging

common /debugg/ debug, ndebug
