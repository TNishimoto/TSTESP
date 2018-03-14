/* get_time.hpp
 * Copyright (C) 2015, Yoshimasa Takabatake, all rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above Copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above Copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 */

#ifndef GETTIME_HPP
#define GETTIME_HPP

#ifndef _WINDOWS
#include <sys/resource.h>
#endif // !_WINDOWS

class Gettime{
public:
#ifndef _WINDOWS
  static double get_time()
  {
    double usertime;
    double systime;
    struct rusage usage;

    getrusage (RUSAGE_SELF, &usage);
    usertime = (double) usage.ru_utime.tv_sec + (double) usage.ru_utime.tv_usec / 1000000.0;
    systime  = (double) usage.ru_stime.tv_sec + (double) usage.ru_stime.tv_usec / 1000000.0;

    return (usertime + systime);
  }
#else
	static double get_time()
	{
		return 1;
	}
#endif

};

#endif // GETTIME_HPP
