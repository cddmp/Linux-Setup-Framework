#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

LSF_MODULE_SAMBA_SHARE_NAME=""
LSF_MODULE_SAMBA_SHARE_PATH=""
LSF_MODULE_SAMBA_SHARE_USER=""
LSF_MODULE_SAMBA_SHARE_GROUP=""
LSF_MODULE_SAMBA_SHARE_PERM=""
LSF_MODULE_SAMBA_SHARE_RO=""

samba_apply()
{
	local smbd="smbd"
	local nmbd="nmbd"
	local config="$1"

	_print_heading "SAMBA: SETUP"
	_file_exists "$config" || return 1
	_service_off $smbd
	_service_off $nmbd
	_file_apply "src=${config},dst=/etc/samba/smb.conf"
	_service_on $smbd
	_service_on $nmbd
}

samba_user_add()
{
	local user=""

	_print_heading "SAMBA: USER"

	while [ $# -ne 0 ]; do
		user_string="$1"
		_user_add "$user_string"
		user=$(_user_get_name "$user_string")
		_print_info "Adding $user to samba"
		if ! _samba_user_exists "$user"; then
			smbpasswd -a "$user"
			smbpasswd -e "$user"
		fi
		shift
	done
}

samba_group_add()
{
	_print_heading "SAMBA: GROUP"

	_group_add $@
}

samba_group_modify()
{
	_print_heading "SAMBA: GROUP MEMBERS"
	_group_modify $@
}

samba_usershare_add()
{
	_print_heading "SAMBA: USER SHARE"

	while [ $# -ne 0 ]; do
		_delimited_line_to_env "$1" "," "LSF_MODULE_SAMBA_SHARE"

		_print_info "Creating user share $LSF_MODULE_SAMBA_SHARE_NAME"
		_samba_share_exists "$LSF_MODULE_SAMBA_SHARE_NAME" && return

		# Make shares by default read-only
		[ -z "$LSF_MODULE_SAMBA_SHARE_RO" ] && LSF_MODULE_SAMBA_SHARE_RO="yes"

		mkdir -p "$LSF_MODULE_SAMBA_SHARE_PATH"
		chown -R "$LSF_MODULE_SAMBA_SHARE_USER" "$LSF_MODULE_SAMBA_SHARE_PATH"
		chmod "$LSF_MODULE_SAMBA_SHARE_PERM" "$LSF_MODULE_SAMBA_SHARE_PATH"

		local template=$(mktemp)
		_samba_share_template "user" >> $template
		_file_append_file "/etc/samba/smb.conf" "$template"
		rm -f $template

		_samba_unset
		shift
	done
}

samba_groupshare_add()
{
	_print_heading "SAMBA: GROUP SHARE"

	while [ $# -ne 0 ]; do
		_delimited_line_to_env "$1" "," "LSF_MODULE_SAMBA_SHARE"

		_print_info "Creating group share $LSF_MODULE_SAMBA_SHARE_NAME"
		_samba_share_exists "$LSF_MODULE_SAMBA_SHARE_NAME" && return

		mkdir -p "$LSF_MODULE_SAMBA_SHARE_PATH"
		chgrp -R "$LSF_MODULE_SAMBA_SHARE_GROUP" "$LSF_MODULE_SAMBA_SHARE_PATH"
		chmod "$LSF_MODULE_SAMBA_SHARE_PERM" "$LSF_MODULE_SAMBA_SHARE_PATH"

		local template=$(mktemp)
		_samba_share_template "group" >> $template
		_file_append_file "/etc/samba/smb.conf" "$template"
		rm -f $template

		_samba_unset
		shift
	done
}

_samba_share_template()
{
	local type="$1"

	local valid_users=""
	if [ "$type" = "group" ]; then
		valid_users="@${LSF_MODULE_SAMBA_SHARE_GROUP}"
	else
		valid_users="${LSF_MODULE_SAMBA_SHARE_USER}"
	fi

	# Make shares by default read-only
	[ -z "$LSF_MODULE_SAMBA_SHARE_RO" ] && LSF_MODULE_SAMBA_SHARE_RO="yes"

	printf -- "\n"
	printf -- "[$LSF_MODULE_SAMBA_SHARE_NAME]\n"
	printf -- "   path = $LSF_MODULE_SAMBA_SHARE_PATH\n"
	printf -- "   read only = $LSF_MODULE_SAMBA_SHARE_RO\n"
	printf -- "   guest ok = no\n"
	printf -- "   valid users = ${valid_users}\n"
	printf -- "   create mask = $LSF_MODULE_SAMBA_SHARE_PERM\n"
	printf -- "   directory mask = $LSF_MODULE_SAMBA_SHARE_PERM\n"
}

_samba_share_exists()
{
	local share_name="$1"

	_file_has_content "/etc/samba/smb.conf" "[${share_name}]" && _print_exists && return 0
	return 1
}

_samba_user_exists()
{
	local user="$1"

	pdbedit -L | cut -d ":" -f 1 | grep -q "$user" && _print_exists && return 0
	return 1
}

_samba_unset()
{
	_env_reset "LSF_MODULE_SAMBA"
}
