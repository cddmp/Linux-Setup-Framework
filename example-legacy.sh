#!/bin/sh

LSF_DIST="Debian_10"
LSF_MODULES="user,ssh,pkg"
. lsf/lsf.inc

# Step 1: Install Debian Buster without any services
# Step 2: Run this script
pkg_upgrade
pkg_add "vim" "tmux" "zsh" "openssh-server"
user_add "user=alice" "user=someapp,type=system,shell=/bin/nologin,home=/srv/someapp"
ssh_patch_apply "#Port 22 ==> Port 7777" "#AddressFamily any ==> AddressFamily inet"
