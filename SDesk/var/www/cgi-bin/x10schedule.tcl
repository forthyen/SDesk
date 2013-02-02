# This script will run X10 commands based upon 

# setup the last day of the month array. Februarys are just treated as 28th.
set ::gLastDayArray(1) 31
set ::gLastDayArray(2) 28
set ::gLastDayArray(3) 31
set ::gLastDayArray(4) 30
set ::gLastDayArray(5) 31
set ::gLastDayArray(6) 30
set ::gLastDayArray(7) 31
set ::gLastDayArray(8) 31
set ::gLastDayArray(9) 30
set ::gLastDayArray(10) 31
set ::gLastDayArray(11) 30
set ::gLastDayArray(12) 31

# Set to 1 for testing
set ::gTest 0
set ::gTestPrint 0

# Load the X10 stuff
catch {source /var/www/cgi-bin/x10.tcl}

proc log {msg} {
    puts "[clock format [clock seconds]]: $msg"
}

proc getClockSeconds {} {
    if {[info exists ::gClockSeconds]} {
        return $::gClockSeconds
    }
    return [clock seconds]
}

proc doRun {task} {
    global tasks

    log "Running task: $task"
    set definition [set tasks($task)]
    set command [lindex $definition 0]

    if {[catch $command res]} {
        log "ERROR: $res"
    }

    set reschedule [lindex $definition 1]
    if {$reschedule} {
        set day [lindex $definition 2]
        set hour [lindex $definition 3]
        set minute [lindex $definition 4]
        set variation [lindex $definition 5]
        set every [lindex $definition 6]
        if {$every > 0} {
            # The task is to be run every x seconds
            set runTime [expr $every * 1000]
            log "Scheduling $task to run in $every secs"
            after $runTime [list doRun $task]
        } else {
            # Run the time at a specified time
            set runTime [computeRunTime $task $day $hour $minute $variation]
            if {[lindex $runTime 1] < 10000} {
                set runTime [computeRunTime $task $day $hour \
                                 [expr $minute + ($variation / 60) + 1] \
                                 $variation]
            }
            log "Scheduling $task to run at [clock format [lindex $runTime 0]]"
            after [lindex $runTime 1] [list doRun $task]
        }
    }
}

proc computeRunTime {task day hour minute variation} {
    set runTime 0

    # Get the current time in seconds
    set now [getClockSeconds]
    set nowWeekday [string trim [clock format $now -format "%w"]]
    set nowYear [string trim [clock format $now -format "%Y"]]
    set nowMonth [string trim [clock format $now -format "%m"]]
    set nowDay [string trim [clock format $now -format "%e"]]
    set nowHour [string trim [clock format $now -format "%k"]]
    set nowMinute [string trim [clock format $now -format "%M"]]

    if {$::gTestPrint} {
        puts ""
        puts "NOWTOME: [clock format $::gClockSeconds]"
        puts "NOW WEEKDAY: $nowWeekday"
        puts "RUNAT: $day $hour $minute $variation"
    }

    # Compute the next month in case we spill over
    set nextMonth [expr $nowMonth + 1]
    if {$nextMonth == 13} {
        set nextMonth 1
    }

    # Compute the next year in case we spill over
    set nextYear [expr $nowYear + 1]

    # Compute the last day of the month
    set lastDay $::gLastDayArray($nowMonth)
    set nextLastDay $::gLastDayArray($nextMonth)

    # Compute the next day
    set nextDay [expr $nowDay + 1]

    # The target run date and time
    set runAt ""

    # Add next day to run at.
    if {[string is integer $day]} {
        # The user specified a day of the month
        if {$day < $nowDay} {
            if {$nextMonth < $nowMonth} {
                # Got to go to the next year
                append runAt "$nextMonth/$day/$nextYear"
            } else {
                # Got to go to the next month
                append runAt "$nextMonth/$day/$nowYear"
            }
        } else {
            # Just select the right day
            append runAt "$nowMonth/$day/$nowYear"
        }
    } elseif {"$day" == "first"} {
        if {"$nowDay" == "1"} {
            if {$nowHour > $hour ||
                ($nowHour == $hour &&
                 $nowMinute > $minute)} {
                # We need to go to the next month
                if {$nextMonth < $nowMonth} {
                    # We need to go to the next year
                    append runAt "$nextMonth/1/$nextYear"
                } else {
                    append runAt "$nextMonth/1/$nowYear"
                }
            } else {
                # Just go to the first of this month
                append runAt "$nowMonth/1/$nowYear"
            }
        } else {
            # Just go to the first of the next month
            if {$nextMonth < $nowMonth} {
                # We need to go to the next year
                append runAt "$nextMonth/1/$nextYear"
            } else {
                append runAt "$nextMonth/1/$nowYear"
            }
        }
    } elseif {"$day" == "last"} {
        if {"$nowDay" == "$lastDay"} {
            if {$nowHour > $hour ||
                ($nowHour == $hour &&
                 $nowMinute > $minute)} {
                # We need to go to the next month
                if {$nextMonth < $nowMonth} {
                    # We need to go to the next year
                    append runAt "$nextMonth/$nextLastDay/$nextYear"
                } else {
                    append runAt "$nextMonth/$nextLastDay/$nowYear"
                }
            } else {
                # Just go to the last day
                append runAt "$nowMonth/$lastDay/$nowYear"
            }
        } else {
            # Just go to the last day
            append runAt "$nowMonth/$lastDay/$nowYear"
        }
    } elseif {"$day" == "any"} {
        if {($nowHour > $hour ||
             ($nowHour == $hour &&
              $nowMinute > $minute))} {
            # We need to go to the next day
            if {$nextDay > $lastDay} {
                if {$nextMonth < $nowMonth} {
                    # We need to go to the next year
                    append runAt "$nextMonth/1/$nextYear"
                } else {
                    # We need to go the next month
                    append runAt "$nextMonth/1/$nowYear"
                }
            } else {
                append runAt "$nowMonth/$nextDay/$nowYear"
            }
        } else {
            # Just go to the correct time
            append runAt "$nowMonth/$nowDay/$nowYear"
        }
    } elseif {"$day" == "Sun"} {
        set nextWeekdayOffset [expr 7 - $nowWeekday]
        if {$nextWeekdayOffset > 7} {
            set nextWeekdayOffset [expr $nextWeekdayOffset - 7]
        }
        if {($nowHour > $hour ||
             ($nowHour == $hour &&
              $nowMinute > $minute))} {
            set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
        } else {
            if {$nowWeekday == 0} {
                set newTime $now
            } else {
                set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
            }
        }
        set runAt [clock format $newTime -format "%m/%d/%Y"]
    } elseif {"$day" == "Mon"} {
        set nextWeekdayOffset [expr 8 - $nowWeekday]
        if {$nextWeekdayOffset > 7} {
            set nextWeekdayOffset [expr $nextWeekdayOffset - 7]
        }
        if {($nowHour > $hour ||
             ($nowHour == $hour &&
              $nowMinute > $minute))} {
            set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
        } else {
            if {$nowWeekday == 1} {
                set newTime $now
            } else {
                set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
            }
        }
        set runAt [clock format $newTime -format "%m/%d/%Y"]
    } elseif {"$day" == "Tue"} {
        set nextWeekdayOffset [expr 9 - $nowWeekday]
        if {$nextWeekdayOffset > 7} {
            set nextWeekdayOffset [expr $nextWeekdayOffset - 7]
        }
        if {($nowHour > $hour ||
             ($nowHour == $hour &&
              $nowMinute > $minute))} {
            set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
        } else {
            if {$nowWeekday == 2} {
                set newTime $now
            } else {
                set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
            }
        }
        set runAt [clock format $newTime -format "%m/%d/%Y"]
    } elseif {"$day" == "Wed"} {
        set nextWeekdayOffset [expr 10 - $nowWeekday]
        if {$nextWeekdayOffset > 7} {
            set nextWeekdayOffset [expr $nextWeekdayOffset - 7]
        }
        if {($nowHour > $hour ||
             ($nowHour == $hour &&
              $nowMinute > $minute))} {
            set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
        } else {
            if {$nowWeekday == 3} {
                set newTime $now
            } else {
                set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
            }
        }
        set runAt [clock format $newTime -format "%m/%d/%Y"]
    } elseif {"$day" == "Thu"} {
        set nextWeekdayOffset [expr 11 - $nowWeekday]
        if {$nextWeekdayOffset > 7} {
            set nextWeekdayOffset [expr $nextWeekdayOffset - 7]
        }
        if {($nowHour > $hour ||
             ($nowHour == $hour &&
              $nowMinute > $minute))} {
            set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
        } else {
            if {$nowWeekday == 4} {
                set newTime $now
            } else {
                set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
            }
        }
        set runAt [clock format $newTime -format "%m/%d/%Y"]
    } elseif {"$day" == "Fri"} {
        set nextWeekdayOffset [expr 12 - $nowWeekday]
        if {$nextWeekdayOffset > 7} {
            set nextWeekdayOffset [expr $nextWeekdayOffset - 7]
        }
        if {($nowHour > $hour ||
             ($nowHour == $hour &&
              $nowMinute > $minute))} {
            set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
        } else {
            if {$nowWeekday == 5} {
                set newTime $now
            } else {
                set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
            }
        }
        set runAt [clock format $newTime -format "%m/%d/%Y"]
    } elseif {"$day" == "Sat"} {
        set nextWeekdayOffset [expr 13 - $nowWeekday]
        if {$nextWeekdayOffset > 7} {
            set nextWeekdayOffset [expr $nextWeekdayOffset - 7]
        }
        if {($nowHour > $hour ||
             ($nowHour == $hour &&
              $nowMinute > $minute))} {
            set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
        } else {
            if {$nowWeekday == 6} {
                set newTime $now
            } else {
                set newTime [expr $now + ($nextWeekdayOffset * (60 * 60 * 24))]
            }
        }
        set runAt [clock format $newTime -format "%m/%d/%Y"]
    } else {
        Log "ERROR: unknown day specification: $day"
    }

    # Add the hour, minutes and seconds
    append runAt " $hour:$minute:00"

    # and substract the current time from it. That gives us
    # the number of seconds from now we need to wait.
    set runTime [clock scan $runAt]

    # Add a random variation. The variation is a max range up and down
    if {$variation > 0} {
        set rand [expr rand()]
        set randomVariation [expr int((($rand + 1) * 10000)) % $variation]
        if {$::gTestPrint} {
            puts "RANDOM: $randomVariation"
        }
        if {[expr rand()] < 0.5} {
            set randomVariation [expr $randomVariation * -1]
        }
        if {$::gTestPrint} {
            puts "RANDOM: $randomVariation"
        }
        set runTime [expr int($runTime + $randomVariation)]
    }

    if {$::gTestPrint} {
        puts "RESULT: $runTime = [clock format $runTime]"
        puts "RUNAT WEEKDAY: [clock format $runTime -format %w]"
    }

    return [list $runTime [expr ($runTime - $now) * 1000]]
}

proc main {} {
    global tasks
    global waitFor

    log "Loading commands file."
    if {[catch {source commands.tcl} res]} {
        log "ERROR: $res"
        exit 1
    }

    log "Building command schedule."
    foreach task [array names tasks] {
        set definition [set tasks($task)]
        set day [lindex $definition 2]
        set hour [lindex $definition 3]
        set minute [lindex $definition 4]
        set variation [lindex $definition 5]

        set runTime [computeRunTime $task $day $hour $minute $variation]
        if {[lindex $runTime 1] < 10000} {
            set runTime [computeRunTime $task $day $hour \
                             [expr $minute + ($variation / 60) + 1] \
                             $variation]
        }
        log "Scheduling $task to run at [clock format [lindex $runTime 0]]"
        after [lindex $runTime 1] [list doRun $task]
    }

    set waitFor 1
    vwait waitFor
}

if {$::gTest} {
    puts "======================================"
    puts -nonewline "Specific time same day..."
    set ::gClockSeconds [clock scan "12/1/2001 01:55:55"]
    set result [computeRunTime test any 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007241180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next day..."
    set ::gClockSeconds [clock scan "12/1/2001 23:55:55"]
    set result [computeRunTime test any 13 00 0]
    set result [lindex $result 0]
    if {$result != 1007326800} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next day next month..."
    set ::gClockSeconds [clock scan "11/30/2001 23:55:55"]
    set result [computeRunTime test any 13 00 0]
    set result [lindex $result 0]
    if {$result != 1007240400} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next day accross the year..."
    set ::gClockSeconds [clock scan "12/31/2001 23:55:55"]
    set result [computeRunTime test any 13 00 0]
    set result [lindex $result 0]
    if {$result != 1009918800} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "First day of this month..."
    set ::gClockSeconds [clock scan "12/1/2001 01:55:55"]
    set result [computeRunTime test first 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007241180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "First day of next month day 1..."
    set ::gClockSeconds [clock scan "11/1/2001 14:55:55"]
    set result [computeRunTime test first 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007241180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "First day of next year day 1..."
    set ::gClockSeconds [clock scan "12/1/2001 14:55:55"]
    set result [computeRunTime test first 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009919580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "First day of next month day 2..."
    set ::gClockSeconds [clock scan "11/2/2001 14:55:55"]
    set result [computeRunTime test first 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007241180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "First day of next year day 2..."
    set ::gClockSeconds [clock scan "12/2/2001 14:55:55"]
    set result [computeRunTime test first 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009919580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "First day of next month..."
    set ::gClockSeconds [clock scan "11/1/2001 13:14:00"]
    set result [computeRunTime test first 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007241180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Last day of this month..."
    set ::gClockSeconds [clock scan "11/1/2001 13:14:00"]
    set result [computeRunTime test last 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007154780} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Last day of next month..."
    set ::gClockSeconds [clock scan "11/30/2001 13:14:00"]
    set result [computeRunTime test last 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009833180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Last day of this year..."
    set ::gClockSeconds [clock scan "12/31/2001 1:00:00"]
    set result [computeRunTime test last 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009833180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Last day of next year..."
    set ::gClockSeconds [clock scan "12/31/2001 13:14:00"]
    set result [computeRunTime test last 13 13 0]
    set result [lindex $result 0]
    if {$result != 1012511580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "12th day of this month..."
    set ::gClockSeconds [clock scan "11/1/2001 1:00:00"]
    set result [computeRunTime test 12 13 13 0]
    set result [lindex $result 0]
    if {$result != 1005599580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "12th day of next month..."
    set ::gClockSeconds [clock scan "11/13/2001 1:00:00"]
    set result [computeRunTime test 12 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008191580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "12th day of next year..."
    set ::gClockSeconds [clock scan "12/13/2001 1:00:00"]
    set result [computeRunTime test 12 13 13 0]
    set result [lindex $result 0]
    if {$result != 1010869980} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Sun of this week this month..."
    set ::gClockSeconds [clock scan "12/14/2001 1:00:00"]
    set result [computeRunTime test Sun 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008537180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "This Sun later..."
    set ::gClockSeconds [clock scan "12/16/2001 13:12:00"]
    set result [computeRunTime test Sun 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008537180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Sun of next week this month..."
    set ::gClockSeconds [clock scan "12/16/2001 13:14:00"]
    set result [computeRunTime test Sun 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009141980} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Sun of next week next month..."
    set ::gClockSeconds [clock scan "11/30/2001 1:00:00"]
    set result [computeRunTime test Sun 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007327580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Sun of next week next year..."
    set ::gClockSeconds [clock scan "12/30/2001 13:14:00"]
    set result [computeRunTime test Sun 13 13 0]
    set result [lindex $result 0]
    if {$result != 1010351580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Mon of this week this month..."
    set ::gClockSeconds [clock scan "12/15/2001 1:00:00"]
    set result [computeRunTime test Mon 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008623580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "This Mon later..."
    set ::gClockSeconds [clock scan "12/17/2001 13:12:00"]
    set result [computeRunTime test Mon 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008623580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Mon of next week this month..."
    set ::gClockSeconds [clock scan "12/17/2001 13:14:00"]
    set result [computeRunTime test Mon 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009228380} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Mon of next week next month..."
    set ::gClockSeconds [clock scan "11/30/2001 1:00:00"]
    set result [computeRunTime test Mon 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007413980} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Mon of next week next year..."
    set ::gClockSeconds [clock scan "12/31/2001 13:14:00"]
    set result [computeRunTime test Mon 13 13 0]
    set result [lindex $result 0]
    if {$result != 1010437980} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Tue of this week this month..."
    set ::gClockSeconds [clock scan "12/16/2001 1:00:00"]
    set result [computeRunTime test Tue 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008709980} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "This Tue later..."
    set ::gClockSeconds [clock scan "12/18/2001 13:12:00"]
    set result [computeRunTime test Tue 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008709980} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Tue of next week this month..."
    set ::gClockSeconds [clock scan "12/18/2001 13:14:00"]
    set result [computeRunTime test Tue 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009314780} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Tue of next week next month..."
    set ::gClockSeconds [clock scan "11/30/2001 1:00:00"]
    set result [computeRunTime test Tue 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007500380} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Tue of next week next year..."
    set ::gClockSeconds [clock scan "12/30/2001 1:00:00"]
    set result [computeRunTime test Tue 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009919580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Wed of this week this month..."
    set ::gClockSeconds [clock scan "12/17/2001 1:00:00"]
    set result [computeRunTime test Wed 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008796380} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "This Wed later..."
    set ::gClockSeconds [clock scan "12/19/2001 13:12:00"]
    set result [computeRunTime test Wed 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008796380} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Wed of next week this month..."
    set ::gClockSeconds [clock scan "12/19/2001 13:14:00"]
    set result [computeRunTime test Wed 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009401180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Wed of next week next month..."
    set ::gClockSeconds [clock scan "11/30/2001 1:00:00"]
    set result [computeRunTime test Wed 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007586780} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Wed of next week next year..."
    set ::gClockSeconds [clock scan "12/30/2001 1:00:00"]
    set result [computeRunTime test Wed 13 13 0]
    set result [lindex $result 0]
    if {$result != 1010005980} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Thu of this week this month..."
    set ::gClockSeconds [clock scan "12/18/2001 1:00:00"]
    set result [computeRunTime test Thu 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008882780} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "This Thu later..."
    set ::gClockSeconds [clock scan "12/20/2001 13:12:00"]
    set result [computeRunTime test Thu 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008882780} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Thu of next week this month..."
    set ::gClockSeconds [clock scan "12/20/2001 13:14:00"]
    set result [computeRunTime test Thu 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009487580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Thu of next week next month..."
    set ::gClockSeconds [clock scan "11/30/2001 1:00:00"]
    set result [computeRunTime test Thu 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007673180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Thu of next week next year..."
    set ::gClockSeconds [clock scan "12/30/2001 1:00:00"]
    set result [computeRunTime test Thu 13 13 0]
    set result [lindex $result 0]
    if {$result != 1010092380} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Fri of this week this month..."
    set ::gClockSeconds [clock scan "12/19/2001 1:00:00"]
    set result [computeRunTime test Fri 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008969180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "This Fri later..."
    set ::gClockSeconds [clock scan "12/21/2001 13:12:00"]
    set result [computeRunTime test Fri 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008969180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Fri of next week this month..."
    set ::gClockSeconds [clock scan "12/21/2001 13:14:00"]
    set result [computeRunTime test Fri 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009573980} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Fri of next week next month..."
    set ::gClockSeconds [clock scan "10/30/2001 1:00:00"]
    set result [computeRunTime test Fri 13 13 0]
    set result [lindex $result 0]
    if {$result != 1004735580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Fri of next week next year..."
    set ::gClockSeconds [clock scan "12/30/2001 1:00:00"]
    set result [computeRunTime test Fri 13 13 0]
    set result [lindex $result 0]
    if {$result != 1010178780} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Sat of this week this month..."
    set ::gClockSeconds [clock scan "12/13/2001 1:00:00"]
    set result [computeRunTime test Sat 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008450780} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "This Sat later..."
    set ::gClockSeconds [clock scan "12/15/2001 13:12:00"]
    set result [computeRunTime test Sat 13 13 0]
    set result [lindex $result 0]
    if {$result != 1008450780} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Sat of next week this month..."
    set ::gClockSeconds [clock scan "12/15/2001 13:14:00"]
    set result [computeRunTime test Sat 13 13 0]
    set result [lindex $result 0]
    if {$result != 1009055580} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Sat of next week next month..."
    set ::gClockSeconds [clock scan "11/30/2001 1:00:00"]
    set result [computeRunTime test Sat 13 13 0]
    set result [lindex $result 0]
    if {$result != 1007241180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Next Sat of next week next year..."
    set ::gClockSeconds [clock scan "12/30/2001 1:00:00"]
    set result [computeRunTime test Sat 13 13 0]
    set result [lindex $result 0]
    if {$result != 1010265180} {
        puts "ERROR"
    } else {
        puts "OK"
    }

    puts "======================================"
    puts -nonewline "Any day at 14:00 +/- 10 seconds..."
    set ::gClockSeconds [clock scan "12/30/2001 1:00:00"]
    set result [computeRunTime test any 14 0 10]
    set result [lindex $result 0]
    if {$result < 1009749590 || $result > 1009749610} {
        puts "ERROR"
    } else {
        puts "OK"
    }
} else {
    if {[catch main res]} {
        log "ERROR: $res"
    }
}
