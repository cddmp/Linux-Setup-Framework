#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

cron_add()
{
	_print_heading "CRON: ADD"
	while [ $# -ne 0 ]; do
		_cron_add "$1"
		shift
	done
}
