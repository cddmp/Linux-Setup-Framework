#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

LSF_NETWORK_PING_HOST="8.8.8.8"
LSF_NETWORK_PING_ATTEMPTS=3

network_check()
{
	_print_heading "NETWORK: CHECK"
	counter=1
	while [ $counter -lt $LSF_NETWORK_PING_ATTEMPTS ]; do
		_print_info "${counter}: ping $LSF_NETWORK_PING_HOST"
		ping -c1 "$LSF_NETWORK_PING_HOST" > /dev/null
		retval=$?
		counter=$((counter+1))
		[ $retval -eq 0 ] && _print_success && break
		_print_error

	done
	[ $retval -eq 0 ] || echo "No internet connection"
	return $?
}


network_apply()
{
	local config="$1"

	_print_heading "NETWORK: SETUP"

	_file_exists "$config" || return 1

	_service_off "networking"
	case "$LSF_DIST" in
		Alpine*)
			_file_apply "src=${config},dst=/etc/network/interfaces"
			;;
		Debian*)
			_file_apply "src=${config},dst=/etc/network/interfaces"
	esac
	_service_on "networking"
}
