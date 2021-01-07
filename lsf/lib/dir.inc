#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

LSF_LIB_DIR_OWNER=""
LSF_LIB_DIR_PATH=""
LSF_LIB_DIR_PERM=""

_dir_add()
{
	while [ $# -ne 0 ]; do
		_delimited_line_to_env "$1" "," "LSF_LIB_DIR"
		_dir_add_single
		_dir_unset
		shift
	done
}

_dir_add_single()
{
	_print_info "$LSF_LIB_DIR_PATH"
	
	if mkdir -p "$LSF_LIB_DIR_PATH"; then
		_print_success "created"
	else
       		_print_error "creating $LSF_LIB_DIR_PATH failed"
		return
	fi

	[ -z "$LSF_LIB_DIR_OWNER" ] && return
	
	_print_info "Changing ownership to $LSF_LIB_DIR_OWNER"
	if ! chown -v -R "$LSF_LIB_DIR_OWNER" "$LSF_LIB_DIR_PATH"; then
       		_print_error "changing ownership failed"
		return
	fi

	[ -z "$LSF_LIB_DIR_PERM" ] && return
	
	_print_info "Changing perms to $LSF_LIB_DIR_PERM"
	if ! chmod -v -R "$LSF_LIB_DIR_PERM" "$LSF_LIB_DIR_PATH"; then
       		_print_error "changing permissions failed"
		return
	fi
}

_dir_unset()
{
	_env_reset "LSF_LIB_DIR"
}
