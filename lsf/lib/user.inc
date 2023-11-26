#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

LSF_LIB_USER_TYPE=""
LSF_LIB_USER_NAME=""
LSF_LIB_USER_DESC=""
LSF_LIB_USER_HOME=""
LSF_LIB_USER_SHELL=""

_user_add()
{
	while [ $# -ne 0 ]; do
		_delimited_line_to_env "$1" "," "LSF_LIB_USER"
		_user_add_single
		_user_unset
		shift
	done
}

_user_add_single()
{
	local opts=""
	local command=""

	command=$(_user_get_command)

	_print_info "Adding $LSF_LIB_USER_NAME"
	_user_exists "$LSF_LIB_USER_NAME" && _print_exists && return

	case "$LSF_LIB_USER_TYPE" in
		"user")
			opts=""
			;;
		"system")
			opts="${opts} --system"
			;;
		"")
			opts=""
			;;
		*)
			_print_error "Invalid type \'$LSF_LIB_USER_TYPE\', skipping $LSF_LIB_USER_NAME"
			continue
			;;
	esac

	case "$LSF_LIB_USER_HOME" in
		"-")
			opts="${opts} --no-create-home"
			;;
		"")
			opts="${opts}"
			;;
		*)
			if [ "$command" = "adduser" ]; then
				opts="${opts} --home $LSF_LIB_USER_HOME"
			elif [ "$command" = "useradd" ]; then
				opts="${opts} --home-dir $LSF_LIB_USER_HOME"
			fi
			;;
	esac

	case "$LSF_LIB_USER_SHELL" in
		"-")
			opts="${opts}"
			;;
		"")
			opts="${opts}"
			;;
		*)
			opts="${opts} --shell $LSF_LIB_USER_SHELL"
			;;
	esac

	$command  "$LSF_LIB_USER_NAME" $opts
	[ "$command" = "useradd" -a "$LSF_LIB_USER_TYPE" != "system" ] && passwd "$LSF_LIB_USER_NAME"
}

_user_get_command()
{
	if _cmd_exists useradd; then
		printf -- "useradd"
	elif _cmd_exists adduser; then
		printf -- "adduser"
	fi
}

_user_exists()
{
	getent passwd "$1" > /dev/null && return 0
	return 1
}

 _user_passwd()
{
	while [ $# -ne 0 ]; do
		_print_info "$1"
		passwd "$1"
		shift
	done
}

# Currently needed by the Samba module - do not delete
_user_get_name()
{
	local input_str="$1"
	local name=""

	_delimited_line_to_env "$1" "," "LSF_LIB_USER"
	printf -- "$LSF_LIB_USER_NAME"
	_user_unset
}

_user_unset()
{
	_env_reset "LSF_LIB_USER"
}
