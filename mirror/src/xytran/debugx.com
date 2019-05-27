# DEBUGX.COM - debug variables for xytran

bool	debug				# true if debugging

common /debugx/ debug
