.PHONY: all 0_uv 1_intro 2_processus 3_ipc_net 4_kernel_mod 5_bus 6_mem_fs 7_compil_tools full

all: 0_uv 1_intro 2_processus 3_ipc_net 4_kernel_mod 5_bus 6_mem_fs 7_compil_tools full

0_uv:
	cd 0_uv && make && make handout

1_intro:
	cd 1_intro && make && make handout

2_processus:
	cd 2_processus && make && make handout

3_ipc_net:
	cd 3_ipc_net && make && make handout

4_kernel_mod:
	cd 4_kernel_mod && make && make handout

5_bus:
	cd 5_bus && make && make handout

6_mem_fs:
	cd 6_mem_fs && make && make handout

7_compil_tools:
	cd 7_compil_tools && make && make handout

full:
	cd full && make && make handout

clean: 0_uv_clean 1_intro_clean 2_processus_clean 3_ipc_net_clean  \
	4_kernel_mod_clean 5_bus_clean 6_mem_fs 7_compil_tools_clean full_clean

0_uv_clean:
	cd 0_uv && make clean

1_intro_clean:
	cd 1_intro && make clean

2_processus_clean:
	cd 2_processus && make clean

3_ipc_net_clean:
	cd 3_ipc_net && make clean

4_kernel_mod_clean:
	cd 4_kernel_mod && make clean

5_bus_clean:
	cd 5_bus && make clean

6_mem_fs:
	cd 6_mem_fs && make clean

7_compil_tools_clean:
	cd 7_compil_tools && make clean

full_clean:
	cd full && make clean
