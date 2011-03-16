bct_test_setup

ITER = 10;

% latmio_dir
for i = 1:size(m)(2)
	[id od deg] = degrees_dir(m{i});
	[is os str] = strengths_dir(m{i});
	R = latmio_dir_cpp(m{i}, ITER);
	[id_R od_R deg_R] = degrees_dir(R);
	[is_R os_R str_R] = strengths_dir(R);
	bct_test(sprintf("latmio_dir %s id", mname{i}), id == id_R)
	bct_test(sprintf("latmio_dir %s od", mname{i}), od == od_R)
	bct_test(sprintf("latmio_dir %s os", mname{i}), os == os_R)
end

% latmio_dir_connected
for i = 1:size(m)(2)
	[id od deg] = degrees_dir(m{i});
	[is os str] = strengths_dir(m{i});
	R = latmio_dir_connected_cpp(m{i}, ITER);
	[id_R od_R deg_R] = degrees_dir(R);
	[is_R os_R str_R] = strengths_dir(R);
	bct_test(sprintf("latmio_dir_connected %s id", mname{i}), id == id_R)
	bct_test(sprintf("latmio_dir_connected %s od", mname{i}), od == od_R)
	bct_test(sprintf("latmio_dir_connected %s os", mname{i}), os == os_R)
end

% latmio_und
for i = 1:size(m)(2)
	ms = m{i} | m{i}';
	deg = degrees_und(ms);
	R = latmio_und_cpp(ms, ITER);
	deg_R = degrees_und(R);
	bct_test(sprintf("latmio_und %s", mname{i}), deg == deg_R)
end

% latmio_und_connected
for i = 1:size(m)(2)
	ms = m{i} | m{i}';
	deg = degrees_und(ms);
	R = latmio_und_connected_cpp(ms, ITER);
	deg_R = degrees_und(R);
	bct_test(sprintf("latmio_und_connected %s", mname{i}), deg == deg_R)
end

% randmio_dir
for i = 1:size(m)(2)
	[id od deg] = degrees_dir(m{i});
	[is os str] = strengths_dir(m{i});
	R = randmio_dir_cpp(m{i}, ITER);
	[id_R od_R deg_R] = degrees_dir(R);
	[is_R os_R str_R] = strengths_dir(R);
	bct_test(sprintf("randmio_dir %s id", mname{i}), id == id_R)
	bct_test(sprintf("randmio_dir %s od", mname{i}), od == od_R)
	bct_test(sprintf("randmio_dir %s os", mname{i}), os == os_R)
end

% randmio_dir_connected
for i = 1:size(m)(2)
	[id od deg] = degrees_dir(m{i});
	[is os str] = strengths_dir(m{i});
	R = randmio_dir_connected_cpp(m{i}, ITER);
	[id_R od_R deg_R] = degrees_dir(R);
	[is_R os_R str_R] = strengths_dir(R);
	bct_test(sprintf("randmio_dir_connected %s id", mname{i}), id == id_R)
	bct_test(sprintf("randmio_dir_connected %s od", mname{i}), od == od_R)
	bct_test(sprintf("randmio_dir_connected %s os", mname{i}), os == os_R)
end

% randmio_und
for i = 1:size(m)(2)
	ms = m{i} | m{i}';
	deg = degrees_und(ms);
	R = randmio_und_cpp(ms, ITER);
	deg_R = degrees_und(R);
	bct_test(sprintf("randmio_und %s", mname{i}), deg == deg_R)
end

% randmio_und_connected
for i = 1:size(m)(2)
	ms = m{i} | m{i}';
	deg = degrees_und(ms);
	R = randmio_und_connected_cpp(ms, ITER);
	deg_R = degrees_und(R);
	bct_test(sprintf("randmio_und_connected %s", mname{i}), deg == deg_R)
end

bct_test_teardown
