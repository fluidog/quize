#!/bin/bash

# match-spec ::= 'func' string |
#                'file' string |
#                'module' string |
#                'format' string |
#                'class' string |
#                'line' line-range
# line-range ::= lineno |
#                '-'lineno |
#                lineno'-' |
#                lineno'-'lineno
# lineno ::= unsigned-int


# -    remove the given flags
# +    add the given flags
# =    set the flags to the given flags

# p    enables the pr_debug() callsite.
# _    enables no flags.
# Decorator flags add to the message-prefix, in order:
# t    Include thread ID, or <intr>
# m    Include module name
# f    Include the function name
# l    Include line number


# example
echo "module <module> +p" > /proc/dynamic_debug/control
echo "func <func> +pm" > /proc/dynamic_debug/control
echo "file <file> +pfl" > /proc/dynamic_debug/control

