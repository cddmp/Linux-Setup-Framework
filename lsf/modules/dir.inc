#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

dir_add()
{
	_print_heading "DIR: ADD"
	while [ $# -ne 0 ]; do
		_dir_add "$1"
		shift
	done
}
