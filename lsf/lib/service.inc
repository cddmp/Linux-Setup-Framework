#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

_has_systemd()
{
	pidof "systemd" > /dev/null && return 0
	return 1
}

_is_running()
{
	local service="$1"
	local retval=""

	if _has_systemd; then
		systemctl is-active --quiet "$service"
		retval="$?"
	else
		#FIXME: Untested
		rc-service "$service" status
		retval="$?"
	fi
	return $retval
}

_is_enabled()
{
	local service="$1"
	local retval=""

	if _has_systemd; then
		systemctl is-enabled --quiet "$service"
		retval="$?"
	else
		#FIXME: Untested
		rc-update show | grep -q "$service"
		retval="$?"
	fi
	return $retval
}

_service_enable()
{
	local service="$1"
	
	_print_info "Enabling $service"
	if _has_systemd; then
		systemctl enable "$service"
	else
		rc-update add "$service" 
	fi
}

_service_disable()
{
	local service="$1"
	
	_print_info "Disabling $service"
	if _has_systemd; then
		systemctl disable "$service"
	else
		rc-update del "$service" 
	fi
}

_service_start()
{
	local service="$1"
	
	_print_info "Starting $service"
	if _has_systemd; then
		systemctl start "$service"
	else
		rc-service "$service" start
	fi
}

_service_stop()
{
	local service="$1"
	
	_print_info "Stopping $service"
	if _has_systemd; then
		systemctl stop "$service"
	else
		rc-service "$service" stop 
	fi
}

_service_restart()
{
	local service="$1"
	
	_print_info "Restarting $service"
	if _has_systemd; then
		systemctl restart "$service"
	else
		rc-service "$service" restart
	fi
}

_service_reload()
{
	local service="$1"

	_print_info "Reloading $service"
	if _has_systemd; then
		systemctl reload "$service"
	else
		rc-service "$service" restart
	fi
}

_service_on()
{
	local service="$1"

	_service_enable "$service"
	_service_start "$service"
}

_service_off()
{
	local service="$1"

	_service_stop "$service"
	_service_disable "$service"
}

_service_new()
{
	local service_file="$1"

	_file_exists "$service_file" || return 1
	if _has_systemd; then
		_file_apply "src=${service_file},dst=/etc/systemd/system/,perm=0644"
	else
		_file_apply "src=${service_file},dst=/etc/init.d/,perm=755"
	fi
}

# Currently not used
_service_exists()
{
	local service_file="$1"

	if _has_systemd; then
		_file_exists "/etc/systemd/system/${service_file}" && return 0
	else
		_file_exists "/etc/init.d/${service}" && return 0
	fi
	return 1
}
