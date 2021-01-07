#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

prompt_user()
{
	local msg="$1"
	
	_print_prompt "$msg" && read
}
