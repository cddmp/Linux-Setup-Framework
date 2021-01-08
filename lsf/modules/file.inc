#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

MODULE_SUPPORTS="Alpine,Debian_10,Debian_11"

# @description
# Allows to patch multiple lines in a file. Default delim is "  ==> ".
#
# @example
# file_patch "my.config" "#someline ==> somenewline"
#
# @arg $1 string The file name of the file which should be patched.
# @arg $2-* string The various patch lines. Delimiter is " ==>".
file_patch()
{
	local file="$1"
	shift
	local patches="$@"

	_print_heading "FILE: PATCH"
	_file_patch "$file" "$patches"
}

# @description
# Allows to uncomment multiple lines in a file.
#
# @example
# file_uncomment "my.config" "#" "#someline" "#anotherline" "#one more line"
#
# @arg $1 string The file name of the file which should be patched.
# @arg $2 string comment character
# @arg $3-* The various comment lines.
file_uncomment()
{
	local file="$1"
	shift
	local comment_char="$1"
	shift

	_print_heading "FILE: UNCOMMENT"
	_file_uncomment "$file" "$comment_char" "$@"
}

# @description
# Copies a configuration from source to destination and allows to
# set ownership and permissions. Multiple files can be processed with
# one function call. The dst parameter can be a file or directory.
# If a directory is given, the file being created in that directory will
# have the same name as src. In any case, if the dst file exists already,
# a backup will be created in the current working directory. The file extension
# will be ".save".
#
# @example
# file_apply "src=my.config,dst=/etc/myservice/my.config,owner=myservice,perm=600"
#
# @arg $1-* Colon separated key-value string. Supported keys: src, dst, owner, perm
file_apply()
{
	_print_heading "FILE: APPLY"
	_file_apply $@
}

# @description
# Appends lines to a given file. A backup of the file will be automatically created
# in the currend working directory. The file extension will be ".save".
#
# @example
# file_append_lines "my.config" "a line" "yet another one" "one more line" ...
#
# @arg $1 filename
# @arg $2-* The various lines to be appended.
file_append_lines()
{
	local filename="$1"; shift

	_print_heading "FILE: APPEND LINES"
	_file_append_lines "$filename" "$@"
}

# @description
# Insert lines to a given file at a given pattern. A backup of the file will be a
# utomatically created in the currend working directory. The file extension will be
# ".save".
#
# @example
# file_insert_lines "my.config" "some entry in my.config" "new line" "another one" ...
#
# @arg $1 filename
# @arg $2 pattern
# @arg $3-* The various lines to be appended after the pattern.
file_insert_lines()
{
	local filename="$1"; shift
	local pattern="$1"; shift

	_print_heading "FILE: INSERT LINES"
	_file_insert_lines "$filename" "$pattern" "$@"
}

file_append_file()
{
	local filename="$1"
	local append="$2"

	_print_heading "FILE: APPEND LINES"
	_file_append_file "$filename" "$append"
}

file_insert_file()
{
	local filename="$1"
	local pattern="$2"
	local insert="$3"

	_print_heading "FILE: INSERT FILE"
	_file_insert_file "$filename" "$pattern" "$insert"
}
