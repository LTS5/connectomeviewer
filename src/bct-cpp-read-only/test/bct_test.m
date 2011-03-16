function bct_test(name, result)
% bct_test(name, result) prints whether a test succeeded and maintains a count
% of the number of failures in the global variable 'failures'.
global failures
printf("%s: ", name)
if result
	printf("success\n")
else
	printf("FAILURE\n")
	failures++;
endif
