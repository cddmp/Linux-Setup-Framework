#
# LSF - Linux Setup Framework 
# https://github.com/cddmp/lsf
#

_string_escape()
{
	local input="$1"
	printf -- "$input" | sed -e 's/[]\/$*.^[]/\\&/g'
}

# Comma to space conversion
_cts()
{
	printf -- "$1" | sed "s/,/ /g"
}

# delim to space conversion
_dts()
{
	local input_str="$1"
	local delim="$2"

	printf -- "$input_str" | sed "s/${delim}/ /g"
}

_split_head()
{	
	local input_str="$1"
	local delim="$2"

	echo -n "${input_str%%${delim}*}"
}

_split_tail()
{
	local input_str="$1"
	local delim="$2"

	echo -n "${input_str##*${delim}}"
}

_trim_head()
{
	local input_str="$1"
	local head="$2"

	printf -- "${input_str#"$head"}"
}

_has_substr()
{
	local input_str="$1"
	local sub_str="$2"
	sub_str="$(_string_escape "$sub_str")"

	printf -- "${input_str}" | grep -q "$sub_str"
	return $?
}

_has_line()
{
	local input_str="$1"
	local line="$2"
	line="$(_string_escape "$line")"

	printf -- "${input_str}" | grep -q "^${line}$"
	return $?
}

# @description
# Splits up a delimited line of key-value pairs and creates environment variables.
#
# @example
# _delimited_line_to_env "key1=value1,key2=value2" "," "SOME_VAR_PREFIX"
# This will create the following environment variables:
#    SOME_VAR_PREFIX_KEY1=value1
#    SOME_VAR_PREFIX_KEY2=value2
# @arg $1 string A delimited line of key-value pairs.
# @arg $2 string Delimiter, needs to be a single character like ','.
# @arg $3 string A variable name prefix.
_delimited_line_to_env()
{
	local delimited_line="$1"
	local delim="$2"
	local var_prefix="$3"

	local key=""
	local value=""
	local remainder=""

	remainder="${delimited_line},"

	while _has_substr "$remainder" "$delim"; do
		kv=$(_split_head "$remainder" ",")
		remainder=$(_trim_head "$remainder" "${kv},")
		key=$(_split_head "$kv" "=")
		value=$(_split_tail "$kv" "=")
		"_key_value_to_var" "$var_prefix" "$key" "$value"
	done
}

# @description
# Creates an environment variable from a key-value pair.
#
# @example
# _key_value_to_var "SOME_VAR_PREFIX" "key" "value"
# This will create the following environment variables:
#    SOME_VAR_PREFIX_KEY=value
# @arg $1 string A variable name prefix.
# @arg $2 string key
# @arg $2 string value
_key_value_to_var()
{
	local var_prefix="$1"
	local key="$2"
	local value="$3"

	local var=""

	key=$(printf -- "$key" | tr '[:lower:]' '[:upper:]')
	var="${var_prefix}_${key}"
	export "$var"="$value"
}
