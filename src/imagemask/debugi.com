# DEBUGI.COM - debugging variable

bool	debug		# true for debugging

common /debugi/ debug
