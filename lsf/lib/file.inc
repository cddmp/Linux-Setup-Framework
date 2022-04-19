#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

LSF_LIB_FILE_SRC=""
LSF_LIB_FILE_DST=""
LSF_LIB_FILE_OWNER=""
LSF_LIB_FILE_PERM=""

# @description
# Allows to patch multiple lines in a file. Default delim is "  ==> ".
# This function should only be called from a module, not from an LSF
# shell script.
#
# @example
# _file_patch "my.config" "#someline ==> somenewline"
#
# @arg $1 string The file name of the file which should be patched.
# @arg $2-* string The various patch lines. Delimiter is " ==>".
_file_patch()
{
	local filename="$1"
	local backup="$(basename $filename).save"

	_file_backup "$filename" "$backup"

	local from=""
	local to=""
	local patch_line=""

	_print_info "Patching $filename"
	while [ $# -ne 1 ]; do
		shift
		patch_line="$1"
		from=$(_split_head "$patch_line" "$LSF_PATCH_DELIM")
		to=$(_split_tail "$patch_line" "$LSF_PATCH_DELIM")
		_patch_line "$filename" "$from" "$to"
		[ $? -ne 0 ] && _print_error && return
	done

	_file_diff "$backup" "$filename"
}

_patch_line()
{
	local filename="$1"
	local from="$2"
	local to="$3"

	local escaped_from=""
	local escaped_to=""

	escaped_from=$(_string_escape "$from")
	escaped_to=$(_string_escape "$to")

	sed -i "s/${escaped_from}/${escaped_to}/g" $filename
	return $?
}

_file_uncomment()
{
	local filename="$1"
	shift
	local comment_char="$1"
	shift

	local backup="$(basename $filename).save"
	_file_backup "$filename" "$backup"

	local line=""
	_print_info "Patching $filename"
	while [ $# -ne 0 ]; do
		line="$1"
		_uncomment_line "$filename" "$comment_char" "$line"
		[ $? -ne 0 ] && _print_error && return
		shift
	done

	_file_diff "$backup" "$filename"
}

_uncomment_line()
{
	local filename="$1"
	local comment_char="$2"
	local line="$3"

	local to=""
	to=$(_split_tail "$line" "$comment_char")

	local escaped_from=""
	local escaped_to=""
	escaped_from=$(_string_escape "$line")
	escaped_to=$(_string_escape "$to")

	sed -i "s/${escaped_from}/${escaped_to}/g" $filename
	return $?
}

_file_backup()
{
	local src="$1"
	local dst="$2"

	if ! _file_exists "$src"; then
		_abort "$src does not exist" 
	fi

	_print_info "Backing up $src to $dst"
	cp -v "$src" "$dst"
}

_file_diff()
{
	local file1="$1"
	local file2="$2"

	tmp_diff=$(mktemp)

	_print_info "Comparing $file1 $file2"
	if [ -n $NO_COLOR ]; then
		diff --color=always "$file1" "$file2" > $tmp_diff
	else
		diff "$file1" "$file2" > $tmp_diff
	fi

	if [ ! -s $tmp_diff ]; then
		_print_warn "Nothing changed - already patched?"
	else
		cat $tmp_diff
	fi
	rm -f $tmp_diff
}

_file_cp()
{
	local src="$1"
	local dst="$2"

	_print_info "Copy file '$src'"
	cp -v "$src" "$dst"
}

_file_chmod()
{
	local perm="$1"
	local filename="$2"

	# Permissions are optional, return early if not passed in
	[ -z "$perm" ] && return
	_print_info "Applying permissions"
	chmod -v "$perm" "$dst"
}

_file_chown()
{
	local owner="$1"
	local filename="$2"

	# Owner is optional, return early if not passed in
	[ -z "$owner" ] && return
	_print_info "Changing ownership"
	chown -v "$owner" "$dst"
}

_file_append_lines()
{
	local filename="$1"; shift

	local backup="$(basename $filename).save"
	_file_backup "$filename" "$backup"

	local line=""
	_print_info "Appending lines into $filename"
	while [ $# -ne 0 ]; do
		line="$1"
		if _file_has_line "$filename" "$line"; then
			shift
			continue
		fi
		printf "${line}" >> "$filename"
		[ $# -gt 0 ] && printf "\n" >> "$filename"
		shift
	done

	_file_diff "$backup" "$filename"
}

_file_insert_lines()
{
	local filename="$1"; shift
	local pattern="$1"; shift

	local backup="$(basename $filename).save"
	_file_backup "$filename" "$backup"

	local tmp="$(mktemp)"
	_print_info "Inserting lines into $filename"
	while [ $# -ne 0 ]; do
		line="$1"
		if _file_has_line "$filename" "$line"; then
			shift
			continue
		fi
		printf "${line}" >> "$tmp"
		[ $# -gt 0 ] && printf "\n" >> "$tmp"
		shift
	done
	sed -i "/$pattern/r $tmp" "$filename"
	rm -f $tmp

	_file_diff "$backup" "$filename"
}

_file_append_file()
{
	local filename="$1"
	local append="$2"
	local backup="$(basename $filename).save"

	_file_backup "$filename" "$backup"

	_print_info "Appending into $filename"
	cat "$append" >> "$filename"
	_file_diff "$backup" "$filename"
}

_file_insert_file()
{
	local filename="$1"
	local pattern="$2"
	local insert="$3"

	local backup="$(basename $filename).save"
	_file_backup "$filename" "$backup"

	_print_info "Inserting into $filename"
	sed -i "/$pattern/r $insert" "$filename"
	_file_diff "$backup" "$filename"
}

_file_has_content()
{
	local filename="$1"
	local content="$2"
	content="$(_string_escape "$content")"

	grep -q "$content" "$filename" && return 0
	return 1
}

_file_has_line()
{
	local filename="$1"
	local line="$2"
	line="$(_string_escape "$line")"

	grep -q "^${line}$" "$filename" && return 0
	return 1
}

_file_exists()
{
	local filename="$1"

	[ ! -f "$filename" ] && _print_error "$filename does not exist" && return 1
	return 0
}

_file_apply()
{
	while [ $# -ne 0 ]; do
		_delimited_line_to_env "$1" "," "LSF_LIB_FILE"
		_file_apply_single
		_file_unset
		shift
	done
}

_file_apply_single()
{
	local src="$LSF_LIB_FILE_SRC"
	local dst="$LSF_LIB_FILE_DST"
	local perm="$LSF_LIB_FILE_PERM"
	local owner="$LSF_LIB_FILE_OWNER"
	local backup="$(basename $dst).save"

	_file_exists "$src" || return 1

	_print_info "Applying $LSF_LIB_FILE_SRC"

	# Expand file name if destination is a directory
	if [ -d "$dst" ]; then
		dst="${dst}/$(basename $src)"
	elif [ -f "$dst" ]; then
		_file_backup "$dst" "$backup"
	fi

	_file_cp "$src" "$dst"
	_file_chmod "$perm" "$dst"
	_file_chown "$owner" "$dst"
}

# Currently not used
_file_is_applied()
{
	local file="$1"
	local owner="$2"
	local perm="$3"

	local perm_match=0
	local owner_match=0

	_file_exists "$file" || return 1

	if [ -n "$owner" ]; then
		local current_owner=""
		if _has_substr "$owner" ":"; then
			current_owner=$(stat -c "%U:%G" "$file")
		else
			current_owner=$(stat -c "%U" "$file")
		fi

		if [ "$current_owner" = "$owner" ]; then
			owner_match=0
		else
			owner_match=1
		fi
	fi

	if [ -n "$perm" ]; then
		local current_perm=""
		current_perm=$(stat -c "%a" "$dst")

		local num_digits=""
		num_digits=$(printf -- "$current_perm" | wc -c)
		if [ $num_digits -eq 3 ]; then
			current_perm="0${current_perm}"
		fi

		num_digits=$(printf -- "$perm" | wc -c)
		if [ $num_digits -eq 3 ]; then
			perm="0${perm}"
		fi

		if [ "$current_perm" = "$perm" ]; then
			perm_match=0
		else
			perm_match=1
		fi
	fi

	if [ $owner_match -eq 0 -a $perm_match -eq 0 ]; then
		_print_success "$dst is already applied, skipping"
		return 0
	fi
	return 1
}

_file_unset()
{
	_env_reset "LSF_LIB_FILE"
}
