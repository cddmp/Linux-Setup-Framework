#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

user_add()
{
	_print_heading "USER: ADD"
	_user_add $@
}

user_passwd()
{
	_print_heading "USER: PASSWD"
	_user_passwd $@
}
