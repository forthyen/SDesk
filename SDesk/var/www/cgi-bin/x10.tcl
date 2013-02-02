# This is my X10 setup

# The overall X10 device path
set devicePath "/dev/x10/"

# All my devices.
set devices(LivingRoomLight1) e4
set devices(LivingRoomLight2) e5
set devices(LivingRoomLight3) e6
set devices(LivingRoomTV) e7

proc x10On {name} {
    if {[catch {writeToX10Device $name 1} res]} {
        puts "ERROR: $res"
    }
}

proc x10Off {name} {
    if {[catch {writeToX10Device $name 0} res]} {
        puts "ERROR: $res"
    }
}

proc writeToX10Device {name value} {
    set fd [open $::devicePath$::devices($name) w]
    puts $fd $value
    close $fd
}

# Custom commands
proc livingRoomOff {} {
    x10Off LivingRoomLight1
    x10Off LivingRoomLight2
    x10Off LivingRoomLight3
#    x10Off LivingRoomTV
}

proc livingRoomOn {} {
    x10On LivingRoomLight1
    x10On LivingRoomLight2
    x10On LivingRoomLight3
#    x10On LivingRoomTV
}

