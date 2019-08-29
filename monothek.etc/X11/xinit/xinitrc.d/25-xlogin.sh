#!/bin/bash

# Don't use the systemd user bus for the X session
if [ "$DBUS_SESSION_BUS_ADDRESS" = "unix:path=$XDG_RUNTIME_DIR/bus" ]; then
    unset DBUS_SESSION_BUS_ADDRESS
fi
