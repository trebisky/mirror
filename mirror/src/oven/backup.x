# BACKUP -- Copy the current oven database to the backup directory if defined.
# Example: set backup = /u1d/pilot/

procedure backup

char	dummy
int	envfind()

begin
	if (envfind ("backup", dummy, 1) < 0)
	    return;

	iferr {
	    call delete ("backup$database")
	} then {
	    ;
	}

	iferr {
	    call fcopy ("database", "backup$database")
	} then {
	    ;
	}
	return
end
