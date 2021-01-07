#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

_abort()
{
	printf "${COLOR_RED}Error:  $1${COLOR_RESET}\n"
	exit 1
}
