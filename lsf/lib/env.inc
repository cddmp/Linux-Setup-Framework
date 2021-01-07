#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

# @description
# Takes a variable name pattern an resets all environment variables which match
# the pattern.
#
# @example
# _env_reset "LSF_MODULE_FILE"
# This will reset all variables containing "LSF_MODULE_FILE" in the variable name, e.g.:
#    LSF_MODULE_FILE_NAME=""
#    LSF_MODULE_FILE_OWNER=""
# @arg $1 string pattern
_env_reset()
{
	local pattern="$1"
	pattern="$(_string_escape "$pattern")"

	for var in $(env | grep "^${pattern}[A-Z_]"'*'"=" | cut -d '=' -f1); do
		export "$var"=""
	done
}
