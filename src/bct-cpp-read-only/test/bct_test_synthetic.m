bct_test_setup

N = 2 ^ (4 + floor(4 * rand()));
sz_cl = log2(N) - 2;
K = (N * (N - 1) + 4 * (2 ^ sz_cl * (2 ^ sz_cl - 1))) / 2;

% makeevenCIJ
CIJ = makeevenCIJ(N, K, sz_cl);
bct_test("makeevenCIJ", size(CIJ) == [N N] && sum(degrees_dir(CIJ)) == K)

mx_lvl = log2(N);
E = 3;

% makefractalCIJ
[CIJ K] = makefractalCIJ(mx_lvl, E, sz_cl);
bct_test("makefractalCIJ", size(CIJ) == [N N] && sum(degrees_dir(CIJ)) == K)

N = 10 + floor(91 * rand());
Kdir = floor((N * (N - 1) + 1) * rand());
Kund = floor(Kdir / 2);

% makelatticeCIJ
CIJ = makelatticeCIJ(N, Kdir);
bct_test("makelatticeCIJ", size(CIJ) == [N N] && sum(degrees_dir(CIJ)) == Kdir)

% makerandCIJ_bd
CIJ = makerandCIJ_bd_cpp(N, Kdir);
bct_test("makerandCIJ_bd", size(CIJ) == [N N] && sum(degrees_dir(CIJ)) == Kdir)

% makerandCIJ_bu
CIJ = makerandCIJ_bu_cpp(N, Kund);
bct_test("makerandCIJ_bu", size(CIJ) == [N N] && sum(degrees_und(CIJ)) == 2 * Kund)

in = floor(rand(N, 1) * floor(N / 3));
out = floor(rand(N, 1) * floor(N / 3));
diff = sum(in) - sum(out);
if diff > 0
	if diff > N
		out += floor(diff / N);
		diff = mod(diff, N);
	end
	out(randperm(N)(1:diff)) += 1;
else
	diff = -diff;
	if diff > N
		in += floor(diff / N);
		diff = mod(diff, N);
	end
	in(randperm(N)(1:diff)) += 1;
end

% makerandCIJdegreesfixed
cij = makerandCIJdegreesfixed_cpp(in, out);
[id od] = degrees_dir(cij);
bct_test("makerandCIJdegreesfixed", in == id' && out == od')

% makeringlatticeCIJ
CIJ = makeringlatticeCIJ(N, Kdir);
bct_test("makeringlatticeCIJ", size(CIJ) == [N N] && sum(degrees_dir(CIJ)) == Kdir)

K = (N - 1) * 2;
s = 0.1;

% maketoeplitzCIJ
CIJ = maketoeplitzCIJ(N, K, s);
bct_test("maketoeplitzCIJ", size(CIJ) == [N N] && sum(degrees_dir(CIJ)) == K)

bct_test_teardown
