bct_test_setup

% betweenness_bin
for i = 1:size(m)(2)
	[EBC BC] = edge_betweenness_bin(m{i});
	bct_test(sprintf("betweenness_bin %s", mname{i}), BC == betweenness_bin_cpp(m{i})')
end

% betweenness_wei
for i = 1:size(m)(2)
	[EBC BC] = edge_betweenness_wei(m{i});
	bct_test(sprintf("betweenness_wei %s", mname{i}), BC == betweenness_wei_cpp(m{i})')
end

% edge_betweenness_bin
for i = 1:size(m)(2)
	bct_test(sprintf("edge_betweenness_bin %s", mname{i}), edge_betweenness_bin(m{i}) == edge_betweenness_bin_cpp(m{i}))
end

% edge_betweenness_wei
for i = 1:size(m)(2)
	bct_test(sprintf("edge_betweenness_wei %s", mname{i}), edge_betweenness_wei(m{i}) == edge_betweenness_wei_cpp(m{i}))
end

% erange
for i = 1:size(m)(2)
	[Erange eta Eshort fs] = erange(m{i});
	[Erange_cpp eta_cpp Eshort_cpp fs_cpp] = erange_cpp(m{i});
	bct_test(sprintf("erange %s Erange", mname{i}), Erange == Erange_cpp)
	bct_test(sprintf("erange %s eta", mname{i}), eta == eta_cpp)
	bct_test(sprintf("erange %s Eshort", mname{i}), Eshort == Eshort_cpp)
	bct_test(sprintf("erange %s fs", mname{i}), fs == fs_cpp)
end

bct_test_teardown
