#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

nftables_apply()
{
	local config="$1"

	_print_heading "FIREWALL: SETUP"
	_file_exists "$config" || return 1
	_service_off "nftables"
	case "$LSF_DIST" in
		Alpine*)
			_file_apply "src=${config},dst=/etc/nftables.d/nftables.local"
			;;
	
		Debian*)
			_file_apply "src=${config},dst=/etc/nftables.conf"
			;;
	esac
	_service_on "nftables"
}
