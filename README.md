The programming used ’clock_getting’ function to measure the cost of the system function calls and read/write calls in the nanosecond. 

The method is to call function twitce. Then,the second time minus the first one to get the cost of the call.

N is the loop times for function/system/read calls. The N increases cause decreasing in the cost of minimal call, because the accuracy is getting better and better. 

The cost of the function call always less than the cost of the system call.
