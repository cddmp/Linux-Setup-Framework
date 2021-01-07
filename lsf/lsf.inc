#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

LSF_LIBS_DIR="$(pwd)/lsf/lib"
LSF_MODULES_DIR="$(pwd)/lsf/modules"
LSF_DIST=${1:-$LSF_DIST}
LSF_MODULES=${2:-$LSF_MODULES}
LSF_PATCH_DELIM=" ==> "

_check_arguments()
{
	local dist="$1"
	local modules="$2"

	[ -z "$dist" ] && _abort "No distribution given."
	[ -z "$modules" ] && _abort "No modules given."
}

_check_support()
{
	local supports="$1"
	local dist="$2"
	local retval=1

	for supported_dist in $(_cts $supports); do
		[ "$supported_dist" = "$dist" ] && retval=0 && break
	done
	return $retval
}

_load_modules()
{
	local dist="$1"
	local modules="$2"
	local module_path=""
	local retval=0

	[ -z "$dist" ] && _abort "Provide a linux distribution"
	[ -z "$modules" ] && _abort "Empty modules list given"

	for module in $(_cts $modules); do
		module_path="${LSF_MODULES_DIR}/${module}.inc"
		[ ! -e "$module_path" ] && _abort "Module '$module' does not exist"
		. "$module_path"
		_check_support "$MODULE_SUPPORTS" "$dist"
		retval=$?
		unset MODULE_SUPPORTS
		[ $retval -ne 0 ] && _abort "Module '$module' does not support '$dist'"
	done
}

_load_libs()
{
	local workdir=$(pwd)
	cd "$LSF_LIBS_DIR/"
	for lib in *; do
		. "${LSF_LIBS_DIR}/${lib}"
	done
	cd $workdir
}

_load_libs
_check_arguments "$LSF_DIST" "$LSF_MODULES"
_load_modules "$LSF_DIST" "$LSF_MODULES"
_print_banner
