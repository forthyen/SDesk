# command = [list command reschedule day hour minute variation]

# Run this task at any day, at 6 in the morning, with 300 seconds variance
set tasks(livingOn) [list {livingRoomOn} 1 any 6 00 300 0]

# Run this task at any day, at 11 in the evening, with 200 seconds variance
set tasks(livingOff) [list {livingRoomOff} 1 any 23 00 200 0]

# Run this task at any day, at 11 in the evening, with 200 seconds variance
#set tasks(livingTest1) [list {livingRoomOff} 1 any 15 45 0 0]

# Run this task at any day, at 14:13:00, with 10 seconds variance,
# and then every 10 seconds thereafter.
#set tasks(livingTest2) [list {puts testliving} 1 any 14 18 10 10]

