bct_test_setup

function match = mapmatch(v1, v2)
	if length(unique(v1)) ~= length(unique(v2)) || unique(v1) ~= unique(v2)
		match = 0;
	else
		match = 1;
		map = zeros(max(v1), 1);
		for i = 1:length(v1)
			if ~map(v1(i))
				map(v1(i)) = v2(i);
			elseif map(v1(i)) ~= v2(i)
				match = 0;
				break
			end
		end
	end
end

% modularity_dir
for i = 1:size(m)(2)
	[Ci Q] = modularity_dir(m{i});
	[Ci_cpp Q_cpp] = modularity_dir_cpp(m{i});
	bct_test(sprintf("modularity_dir %s Ci", mname{i}), mapmatch(Ci, Ci_cpp'))
	bct_test(sprintf("modularity_dir %s Q", mname{i}), abs(Q - Q_cpp) < 1e-6)
end

% modularity_und
for i = 1:size(m)(2)
	ms = m{i} | m{i}';
	[Ci Q] = modularity_und(ms);
	[Ci_cpp Q_cpp] = modularity_und_cpp(ms);
	bct_test(sprintf("modularity_und %s Ci", mname{i}), mapmatch(Ci, Ci_cpp'))
	bct_test(sprintf("modularity_und %s Q", mname{i}), abs(Q - Q_cpp) < 1e-6)
end

% modularity_und_louvain
for i = 1:size(m)(2)
	ms = m{i} | m{i}';
	[Ci Q] = modularity_und_louvain(ms);
	result = 0;
	tries = 0;
	while (~result && tries < 100)
		[Ci_cpp Q_cpp] = modularity_und_louvain_cpp(ms);
		if (mapmatch(Ci{end}, Ci_cpp) && abs(Q{end} - Q_cpp) < 1e-6)
			result = 1;
		end
		tries = tries + 1;
	end
	bct_test(sprintf("modularity_und_louvain %s", mname{i}), result)
end

% module_degree_zscore
for i = 1:size(m)(2)
	Ci = modularity_dir_cpp(m{i});
	bct_test(sprintf("module_degree_zscore %s", mname{i}), abs(module_degree_zscore(m{i}, Ci) - module_degree_zscore_cpp(m{i}, Ci)') < 1e-6)
end

% participation_coef
for i = 1:size(m)(2)
	Ci = modularity_dir_cpp(m{i});
	bct_test(sprintf("participation_coef %s", mname{i}), participation_coef(m{i}, Ci) == participation_coef_cpp(m{i}, Ci)')
end

bct_test_teardown
