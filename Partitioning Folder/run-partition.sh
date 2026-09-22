
# Please use g++ version 12
g++ metis_partitioning_file_extended.cpp -o mt_ptn.out -I/usr/include -L/usr/lib -lmetis
g++ BalancePartition.cpp -o dlb_ptn.out
g++ BalancePartitionTG.cpp -o tdlb_ptn.out


# METIS RUN
./mt_ptn.out ../data/com-lj.ungraph.txt com-lj_mt.txt
./mt_ptn.out ../data/com-orkut.ungraph.txt com-orkut_mt.txt
./mt_ptn.out ../data/com-friendster.ungraph.txt com-fs_mt.txt


# Degree Based Partitioning Run
./dlb_ptn.out ../data/com-lj.ungraph.txt com-lj_dc.txt 3
./dlb_ptn.out ../data/com-orkut.ungraph.txt com-orkut_dc.txt 3
./dlb_ptn.out ../data/com-friendster.ungraph.txt com-fs_dc.txt 3


# Transformed Degree Based Partitioning Run
./tdlb_ptn.out ../data/com-lj.ungraph.txt com-lj_tdc.txt 3
./tdlb_ptn.out ../data/com-orkut.ungraph.txt com-orkut_tdc.txt 3
./tdlb_ptn.out ../data/com-friendster.ungraph.txt com-fs_tdc.txt 3




# These commands will create all the partitioned files in the "Partition Folder" directory.
