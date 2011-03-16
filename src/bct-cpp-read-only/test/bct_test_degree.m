bct_test_setup

% assortativity_dir
for i = 1:size(m)(2)
	bct_test(sprintf("assortativity_dir %s", mname{i}), assortativity(m{i}, 1) == assortativity_dir_cpp(m{i}))
end

% assortativity_und
for i = 1:size(m)(2)
	bct_test(sprintf("assortativity_und %s", mname{i}), assortativity(m{i}, 0) == assortativity_und_cpp(m{i}))
end

% degrees_dir
for i = 1:size(m)(2)
	[id od deg] = degrees_dir(m{i});
	bct_test(sprintf("degrees_dir %s", mname{i}), deg == degrees_dir_cpp(m{i}))
end

% degrees_und
for i = 1:size(m)(2)
	bct_test(sprintf("degrees_und %s", mname{i}), degrees_und(m{i}) == degrees_und_cpp(m{i}))
end

% density_dir
for i = 1:size(m)(2)
	bct_test(sprintf("density_dir %s", mname{i}), density_dir(m{i}) == density_dir_cpp(m{i}))
end

% density_und
for i = 1:size(m)(2)
	bct_test(sprintf("density_und %s", mname{i}), density_und(m{i}) == density_und_cpp(m{i}))
end

% jdegree
for i = 1:size(m)(2)
	[J J_od J_id J_bl] = jdegree(m{i});
	bct_test(sprintf("jdegree %s", mname{i}), J == jdegree_cpp(m{i}))
	bct_test(sprintf("jdegree_bl %s", mname{i}), J_bl == jdegree_bl_cpp(J))
	bct_test(sprintf("jdegree_id %s", mname{i}), J_id == jdegree_id_cpp(J))
	bct_test(sprintf("jdegree_od %s", mname{i}), J_od == jdegree_od_cpp(J))
end

% matching_ind
for i = 1:size(m)(2)
	[Min Mout Mall] = matching_ind(m{i});
	bct_test(sprintf("matching_ind %s", mname{i}), Mall == matching_ind_cpp(m{i}))
	bct_test(sprintf("matching_ind_in %s", mname{i}), Min == matching_ind_in_cpp(m{i}))
	bct_test(sprintf("matching_ind_out %s", mname{i}), Mout == matching_ind_out_cpp(m{i}))
end

% strengths_dir
for i = 1:size(m)(2)
	[is os str] = strengths_dir(m{i});
	bct_test(sprintf("strengths_dir %s", mname{i}), str == strengths_dir_cpp(m{i}))
end

% strengths_und
for i = 1:size(m)(2)
	bct_test(sprintf("strengths_und %s", mname{i}), strengths_und(m{i}) == strengths_und_cpp(m{i}))
end

bct_test_teardown
