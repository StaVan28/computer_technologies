#pragma once


#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"


double CalcIntegral (double begin, double end, double (*foo) (double x));
int    Integrate    (int n_threads, double begin, double end, double (*foo) (double), double* const result);
