#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

COLOR_RESET="\e[0m"
COLOR_RED="\e[31m"
COLOR_GREEN="\e[32m"
COLOR_YELLOW="\e[33m"
COLOR_MAGENTA="\e[35m"
COLOR_CYAN="\e[36m"

_green()
{
	_color "green" "$1"
}

_magenta()
{
	_color "magenta" "$1"
}

_yellow()
{
	_color "yellow" "$1"
}

_cyan()
{
	_color "cyan" "$1"
}

_red()
{
	_color "red" "$1"
}

_color()
{
	local color="$1"
	local msg="$2"

	if [ -n "$NO_COLOR" ]; then
		printf -- "$msg"
		return
	fi

	case "$color" in
		"red")
		printf -- "${COLOR_RED}$msg${COLOR_RESET}"

		;;
		"green")
		printf -- "${COLOR_GREEN}$msg${COLOR_RESET}"
		;;
		"yellow")
		printf -- "${COLOR_YELLOW}$msg${COLOR_RESET}"
		;;
		"magenta")
		printf -- "${COLOR_MAGENTA}$msg${COLOR_RESET}"
		;;
		"cyan")
		printf -- "${COLOR_CYAN}$msg${COLOR_RESET}"
		;;
		*)
		printf -- "$msg"
		;;
	esac

}

_print_banner()
{
	printf -- "$(_green "---< LINUX SETUP FRAMEWORK (LSF) >---")\n"
	printf -- "Author: mw\n"
	printf -- "Source: https://github.com/cddmp/lsf\n"
	printf -- "Distribution: $(_magenta $LSF_DIST)\n"
}

 _print_success()
{
	local msg="$1"
	if [ -n "$msg" ]; then
		printf -- "$(_green "OK: $msg")\n"
		return
	fi
	printf -- "$(_green "OK")\n"
}

_print_exists()
{
	_print_success "exists, skipping"
}

 _print_error()
{
	local msg="$1"
	if [ -n "$msg" ]; then
		printf -- "$(_red "Fail: $msg")\n"
	        return
	fi
	printf -- "$(_red "Fail")\n"
}

 _print_warn()
{
	local msg="$1"
	if [ -n "$msg" ]; then
		printf -- "$(_yellow "Warn: $msg")\n"
		return
	fi
	printf -- "$(_yellow "Warn")\n"
}

 _print_prompt()
{
	local msg="$1"
	printf -- "\n$(_magenta "Input required: $msg")\n"
}

 _print_heading()
{
	local msg="$1"
	printf -- "\n$(_yellow "---< $msg >---")\n"
}

 _print_info()
{
	local msg="$1"
	printf -- "$(_cyan "> ${msg}\n")"
}
