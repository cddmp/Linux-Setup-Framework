#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

LSF_LIB_CRON_CMD=""
LSF_LIB_CRON_TIME=""
LSF_LIB_CRON_USER=""
LSF_LIB_CRON_COMMENT=""

_cron_add()
{
	while [ $# -ne 0 ]; do
		_delimited_line_to_env "$1" "," "LSF_LIB_CRON"
		_cron_add_single
		_cron_unset
		shift
	done
}

 _cron_add_single()
{
	_print_info "Setting up cron job '$LSF_LIB_CRON_CMD' for user $LSF_LIB_CRON_USER"

	if ! _user_exists "$LSF_LIB_CRON_USER"; then
		_print_error "user $LSF_LIB_CRON_USER does not exist"
		return 1
	fi

	local tmp="$(mktemp)"
	if _cron_crontab_exists; then
		crontab -u "$LSF_LIB_CRON_USER" -l > "$tmp"
	fi

	local cronjob="${LSF_LIB_CRON_TIME} ${LSF_LIB_CRON_CMD}"
	if ! _cron_cronjob_exists "${cronjob}"; then
		[ -n "$LSF_LIB_CRON_COMMENT" ] && printf -- "# ${LSF_LIB_CRON_COMMENT}\n" >> "$tmp"
		printf -- "${cronjob}\n" >> "$tmp"
		if crontab -u "$LSF_LIB_CRON_USER" "$tmp"; then
			_print_success "installed crontab"
		else
			_print_error "could not install crontab for user $LSF_LIB_CRON_USER"
		fi
	fi
	rm -f "$tmp"
}

_cron_crontab_exists()
{
	crontab -l -u "$LSF_LIB_CRON_USER" 2>&1 | grep -q "no crontab"
	[ $? -eq 1 ] && return 0
	return 1
}

_cron_cronjob_exists()
{
	local cronjob="$1"

	_cron_crontab_exists || return 1

	local crontab
	crontab=$(crontab -l -u "$LSF_LIB_CRON_USER" 2>/dev/null)

	_has_line "${crontab}\n" "$cronjob" && _print_exists && return 0
	return 1
}

_cron_unset()
{
	_env_reset "LSF_LIB_CRON"
}
