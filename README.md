# Linux Setup Framework (LSF)

Warning, this code is still in an experimental state.

## Motivation
Many people use configuration management tools like Puppet, Chef or Ansible. For private purpose these are often overkill and most of them require to install an agent on the target system. In addition, they bring complex configuration files, which takes some time to get into. This framework is meant as a lightweight alternative.

The idea is to write a shell script and use LSF as a framework. LSF encapsulates tasks which need to be often performed in simple function calls. Such calls allow e.g. to patch the SSH configuration, restart services, add users or upgrade the system. The various functions are implemented in modules (see modules directory).

All module functions can be called directly from a shell script (see example below).

## Features
* Color support (an NO_COLOR support)
* BusyBox compatible
* Tested in sh mode of BusyBox ash, bash and dash

## Usage
Take a look at the example shell script below. The ```source``` line loads the framework and tells the framework which Linux Distribution to expect (currently Alpine Linux and Debian Buster, Bullseye are (more or less) supported) and what modules to use. The supported modules are found in the modules directory.

```shell
#!/bin/bash

source ./lsf/lsf.inc "Debian_10" "user,ssh,pkg"

# Step 1: Install Debian Buster without any services
# Step 2: Run this script
pkg_upgrade
pkg_add vim tmux zsh openssh-server
user_add "name=alice" "name=someapp,type=system,shell=/bin/nologin,home=/srv/someapp"
ssh_patch_apply "#Port 22 ==> Port 7777" "#AddressFamily any ==> AddressFamily inet"
```

The use case goes like that: The user has installed Debian Buster on a server and now wants to change the default port of the SSH daemon. In addition, the user wants the SSH daemon to only listen on IPv4. For this, the user just copies over the repo (USB drive or direct git clone) and runs the script. The example script above will perform the following tasks:

1. Upgrade all packages
2. Install vim, tmux, zsh and openssh-server
3. Add the normal user alice and the system user bob (will automatically prompt for a password)
4. Patches Debian Buster's default SSH configuration to listen on 7777/tcp IPV4 only - the service will automatically restart

New modules can be easily added by just copying an existing module and modifying it or writing it from scratch. The basic idea is, that the name of the functions follows a certain naming convention, e.g. service_function (like ssh_restart). That way the various commands can be easily remembered.

## Design & Contribution
* Code which is meant to be used by multiple modules must go into lib
* Always use local variables to not clutter env
* For compatibility with POSIX do not use arrays
* Don't declare and initalize a variable on the same line
