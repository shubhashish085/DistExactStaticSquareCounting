#ifndef DESC_DIST_RUN
#define DESC_DIST_RUN

#include <unordered_map>
#include <unordered_set>
#include "graph.h"
#include "types.h"
#include <dirent.h>
#include <sys/stat.h>
#include "distributioncoordinator.hpp"


double run_mpi_wb_network(const char* filename, DistributionCoordinator &coordinator, int workerNum, int lenBuf, 
    double &srcCompCost, double &workerCompCostMax, double &workerCompCostSum);

double run_mpi_db_graph(const char* filename, DistributionCoordinator &coordinator, int workerNum, int lenBuf, 
    double &srcCompCost, double &workerCompCostMax, double &workerCompCostSum);

void run_exp_wb_graph(const char* input, const char* outPath, DistributionCoordinator &coordinator, int workerNum, int bufLen=1000);

void run_exp_db_graph(const char* input, const char* outPath, DistributionCoordinator &coordinator, int workerNum, int bufLen=1000);


#endif