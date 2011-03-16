filenames                = assortativity_dir_cpp \
                           assortativity_und_cpp \
                           betweenness_bin_cpp \
                           betweenness_wei_cpp \
                           breadth_cpp \
                           breadthdist_cpp \
                           charpath_ecc_cpp \
                           charpath_lambda_cpp \
                           clustering_coef_bd_cpp \
                           clustering_coef_bu_cpp \
                           clustering_coef_wd_cpp \
                           clustering_coef_wu_cpp \
                           cycprob_fcyc_cpp \
                           cycprob_pcyc_cpp \
                           degrees_dir_cpp \
                           degrees_und_cpp \
                           density_dir_cpp \
                           density_und_cpp \
                           distance_bin_cpp \
                           distance_wei_cpp \
                           efficiency_global_cpp \
                           efficiency_local_cpp \
                           edge_betweenness_bin_cpp \
                           edge_betweenness_wei_cpp \
                           erange_cpp \
                           find_motif34_cpp \
                           findpaths_cpp \
                           findwalks_cpp \
                           jdegree_cpp \
                           jdegree_bl_cpp \
                           jdegree_id_cpp \
                           jdegree_od_cpp \
                           latmio_dir_cpp \
                           latmio_dir_connected_cpp \
                           latmio_und_cpp \
                           latmio_und_connected_cpp \
                           makeevenCIJ_cpp \
                           makefractalCIJ_cpp \
                           makelatticeCIJ_cpp \
                           makerandCIJ_bd_cpp \
                           makerandCIJ_bu_cpp \
                           makerandCIJdegreesfixed_cpp \
                           makeringlatticeCIJ_cpp \
                           maketoeplitzCIJ_cpp \
                           matching_ind_cpp \
                           matching_ind_in_cpp \
                           matching_ind_out_cpp \
                           modularity_dir_cpp \
                           modularity_und_cpp \
                           modularity_und_louvain_cpp \
                           module_degree_zscore_cpp \
                           motif3funct_bin_cpp \
                           motif3funct_wei_cpp \
                           motif3generate_cpp \
                           motif3struct_bin_cpp \
                           motif3struct_wei_cpp \
                           motif4generate_cpp \
                           motif4funct_bin_cpp \
                           motif4funct_wei_cpp \
                           motif4struct_bin_cpp \
                           motif4struct_wei_cpp \
                           participation_coef_cpp \
                           randmio_dir_cpp \
                           randmio_dir_connected_cpp \
                           randmio_und_cpp \
                           randmio_und_connected_cpp \
                           reachdist_cpp \
                           strengths_dir_cpp \
                           strengths_und_cpp \
                           threshold_absolute_cpp \
                           threshold_proportional_dir_cpp \
                           threshold_proportional_und_cpp
objects                 := $(foreach f, $(filenames), $(f).o)
oct_files               := $(foreach f, $(filenames), $(f).oct)
m_files                  = bct_test.m \
                           bct_test_all.m

include ../Makefile.vars

.PHONY: all clean install uninstall

all: $(oct_files)

.SECONDEXPANSION:

$(oct_files): bct_test.h bct_test.cpp $$(basename $$@).cpp
	mkoctfile -Wall -lbct -lgsl -lgslcblas $(basename $@).cpp

install:
	if [ ! -d $(install_dir)/share/bct ]; then \
		mkdir $(install_dir)/share/bct; \
	fi
	cp $(oct_files) $(m_files) $(install_dir)/share/bct

uninstall:
	-rm -rf $(install_dir)/share/bct

clean:
	-rm $(objects) $(oct_files)
