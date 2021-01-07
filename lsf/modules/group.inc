#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

group_add()
{
	_print_heading "GROUP: ADD"
	_group_add $@
}

group_modify()
{
	_print_heading "GROUP: MODIFY"
	_group_modify $@
}
