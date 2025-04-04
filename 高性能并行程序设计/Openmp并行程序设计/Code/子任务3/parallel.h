// parallel.h
#ifndef PARALLEL_H
#define PARALLEL_H

void parallel_for(int start, int end, int increment, void *(*functor)(void*), void *arg, int num_threads);

#endif // PARALLEL_H
