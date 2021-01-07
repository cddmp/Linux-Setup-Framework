#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

service_start()
{
	local service="$1"

	_print_heading "SERVICE: START"
	_service_start "$service"
}

service_stop()
{
	local service="$1"

	_print_heading "SERVICE: STOP"
	_service_stop "$service"
}

service_enable()
{
	local service="$1"

	_print_heading "SERVICE: ENABLE"
	_service_enable "$service"
}

service_disable()
{
	local service="$1"

	_print_heading "SERVICE: DISABLE"
	_service_disable "$service"
}

service_restart()
{
	local service="$1"

	_print_heading "SERVICE: RESTART"
	_service_restart "$service"
}

service_apply()
{
	local service="$1"

	_print_heading "SERVICE: APPLY"
	_service_off "$service"
	_file_apply $@
	_service_on "$service"
}

service_patch_apply()
{
	local service="$1"
	local config="$2"
	local patches="$3"

	_print_heading "SERVICE: PATCH"
	_service_off "$service"
	_file_patch "$config" "$patches"
	_service_on "$service"
}

service_patch()
{
	local service="$1"
	local config="$2"
	local patches="$3"

	_print_heading "SERVICE: PATCH"
	_file_patch "$config" "$patches"
}

service_new()
{
	local service_file="$1"

	_print_heading "SERVICE: NEW"
	_file_exists "$service_file" || return 1
	_service_new "$service_file"
	_service_on "$service_file"
}

service_on()
{
	local service="$1"

	_print_heading "SERVICE: START AND ENABLE"
	_service_on "$service"
}

service_off()
{
	local service="$1"

	_print_heading "SERVICE: STOP AND DISABLE"
	_service_off "$service"
}
