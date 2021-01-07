#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

_cmd_exists()
{
	local cmd="$1"

	which $cmd > /dev/null
	return $?
}
