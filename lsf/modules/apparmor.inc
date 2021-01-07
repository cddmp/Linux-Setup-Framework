#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Debian_10,Debian_11"

apparmor_new()
{
	local config="$1"
	local dst="/etc/apparmor.d/"

	_print_heading "APPARMOR: NEW"
	_file_exists "$config" || return 1
	_file_apply "src=${config},dst=${dst},perm=600"
	aa-enforce "${dst}$(basename ${config})"
	_service_reload "apparmor"
}

apparmor_reload()
{
	_print_heading "APPARMOR: RELOAD"
	_service_reload "apparmor"
}

apparmor_stop()
{
	_print_heading "APPARMOR: STOP"
	_service_stop "apparmor"
}

apparmor_start()
{
	_print_heading "APPARMOR: START"
	_service_start "apparmor"
}

apparmor_enforce()
{
	local policy=""

	_print_heading "APPARMOR: ENFORCE"
	while [ $# -ne 0 ]; do
		policy="$1"
		_print_info "Enforcing $policy"
		aa-enforce "$policy"
		shift
	done
}
