#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

LSF_LIB_GROUP_NAME=""
LSF_LIB_GROUP_MEMBERS=""

_group_add()
{
	while [ $# -ne 0 ]; do
		_delimited_line_to_env "$1" "," "LSF_LIB_GROUP"
		_group_add_single
		_group_unset
		shift
	done
}

_group_add_single()
{
	_print_info "$LSF_LIB_GROUP_NAME"
	_group_exists "$LSF_LIB_GROUP_NAME" && _print_exists && return

	if _cmd_exists addgroup; then
		addgroup "$LSF_LIB_GROUP_NAME"
	elif _cmd_exists groupadd; then
		groupadd "$LSF_LIB_GROUP_NAME"
	fi

	if [ -n "$LSF_LIB_GROUP_MEMBERS" ]; then
		_group_modify_single
	fi
}

_group_modify()
{
	while [ $# -ne 0 ]; do
		_delimited_line_to_env "$1" "," "LSF_LIB_GROUP"
		_group_modify_single
		_group_unset
		shift
	done
}

_group_modify_single()
{
	for user in $(_dts "$LSF_LIB_GROUP_MEMBERS" ";"); do
		_print_info "Adding $user to $LSF_LIB_GROUP_NAME"
		
		if _cmd_exists usermod; then
			usermod -aG "$LSF_LIB_GROUP_NAME" "$user"
		elif _cmd_exists adduser; then
			adduser -D -H -G "$LSF_LIB_GROUP_NAME" "$user"
		elif _cmd_exists gpasswd; then
			gpasswd -a "$user" "$LSF_LIB_GROUP_NAME"
		fi
	done
}

_group_exists()
{
	getent group "$1" > /dev/null && return 0
	return 1
}

_group_unset()
{
	_env_reset "LSF_LIB_GROUP"
}
