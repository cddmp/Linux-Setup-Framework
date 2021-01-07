#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

ssh_apply()
{
	local config="$1"

	_print_heading "SSH: SETUP"
	_ssh_get_service_name
	_file_exists "$config" || return 1
	_service_off "$LSF_MODULE_SSH_SERVICE_NAME"
	_file_replace "$config" "/etc/sshd/sshd_config"
	_service_on "$LSF_MODULE_SSH_SERVICE_NAME"
}

ssh_patch()
{
	_print_heading "SSH: PATCH"
	_file_patch "/etc/ssh/sshd_config" "$@"
}

ssh_append_lines()
{
	_print_heading "SSH: APPEND LINES"
	_file_append_lines "/etc/ssh/sshd_config" "$@"
}

ssh_insert_lines()
{
	local pattern="$1"
	shift

	_print_heading "SSH: INSERT LINES"
	_file_insert_lines "/etc/ssh/sshd_config" "$pattern" "$@"
}

ssh_patch_apply()
{
	_print_heading "SSH: SETUP"
	_ssh_get_service_name
	_service_off "$LSF_MODULE_SSH_SERVICE_NAME"
	_file_patch "/etc/ssh/sshd_config" "$@"
	_service_on "$LSF_MODULE_SSH_SERVICE_NAME"
}

ssh_append_apply()
{
	local sshd="sshd"

	_print_heading "SSH: SETUP"
	_ssh_get_service_name
	_service_off "$LSF_MODULE_SSH_SERVICE_NAME"
	_file_append_lines "/etc/ssh/sshd_config" "$@"
	_service_on "$LSF_MODULE_SSH_SERVICE_NAME"
}

ssh_restart()
{
	_print_heading "SSH: RESTART"
	_ssh_get_service_name
	_service_restart "$LSF_MODULE_SSH_SERVICE_NAME"
}

ssh_stop()
{
	_print_heading "SSH: RESTART"
	_ssh_get_service_name
	_service_stop "$LSF_MODULE_SSH_SERVICE_NAME"
}

ssh_start()
{
	local sshd="sshd"

	_print_heading "SSH: RESTART"
	_ssh_get_service_name
	_service_start "$LSF_MODULE_SSH_SERVICE_NAME"
}

# This function is a fix for a weird systemd issue in Debian.
_ssh_get_service_name()
{
	case "$LSF_DIST" in
		Debian*)
			LSF_MODULE_SSH_SERVICE_NAME="ssh"
			;;
		*)
			LSF_MODULE_SSH_SERVICE_NAME="sshd"
			;;
	esac
}
