bct_test_setup

W = rand(10);
W(logical(eye(10))) = 0;

% threshold_absolute
bct_test("threshold_absolute", threshold_absolute(W, 0.5) == threshold_absolute_cpp(W, 0.5))

% threshold_proportional_dir
bct_test("threshold_proportional_dir", threshold_proportional(W, 0.5) == threshold_proportional_dir_cpp(W, 0.5))

% threshold_proportional_und
W = triu(W) + triu(W)';
bct_test("threshold_proportional_und", threshold_proportional(W, 0.5) == threshold_proportional_und_cpp(W, 0.5))

bct_test_teardown
