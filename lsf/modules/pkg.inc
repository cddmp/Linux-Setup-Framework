#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

_pkg_add_debian()
{
	apt-get install -y "$1"
}

_pkg_add_alpine()
{
	apk add "$1"
}

_pkg_upgrade_debian()
{
	apt-get update && apt-get upgrade -y
}

_pkg_upgrade_alpine()
{
	apk -U upgrade
}

_is_installed_debian()
{
	dpkg-query -W -f="${Status}\n" | grep -q installed
	return $?
}

_is_installed_alpine()
{
	apk list "$1" | grep -q installed
	return $?
}

_is_available_debian()
{
	local result=""
	result=$(apt-cache search "^${1}$")
	[ -z "$result" ] && return 1
	return 0
}

_is_available_alpine()
{
	apk info "$1" > /dev/null 2>&1
	return $?
}

_is_installed()
{
	local pkg="$1"

	case "$LSF_DIST" in
		Alpine*)
			_is_installed_alpine "$pkg"
			retval=$?
			;;
		Debian*)
			_is_installed_debian "$pkg"
			retval=$?
			;;
	esac
	return $retval
}

_pkg_add()
{
	local pkg="$1"

	case "$LSF_DIST" in
		Alpine*)
			_pkg_add_alpine "$pkg"
			retval=$?
			;;
		Debian*)
			_pkg_add_debian "$pkg"
			retval=$?
			;;
	esac
	return $retval
}

_is_available()
{
	local pkg="$1"

	case "$LSF_DIST" in
		Alpine*)
			_is_available_alpine "$pkg"
			retval=$?
			;;
		Debian*)
			_is_available_debian "$pkg"
			retval=$?
			;;
	esac
	return $retval
}

pkg_add()
{
	_print_heading "PKGS: ADD"
	local pkg=""

	while [ $# -ne 0 ]; do
		pkg="$1"
		_print_info "$pkg"
		if ! _is_available "$pkg"; then
			_print_error "not available"
		elif _is_installed "$pkg"; then
			_print_success "already installed"
		elif ! _pkg_add "$pkg"; then
			_print_error "install failed"
		fi
		shift
	done

	return 0
}

pkg_upgrade()
{
	_print_heading "PKGS: UPGRADE"
	_print_info "Upgrading packages"
	case "$LSF_DIST" in
		Alpine*)
			_pkg_upgrade_alpine
			retval=$?
			;;
		Debian*)
			_pkg_upgrade_debian
			retval=$?
			;;
	esac
	
	return 0
}
