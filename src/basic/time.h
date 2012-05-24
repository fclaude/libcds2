/********************************************************************************
Copyright (c) 2012, Francisco Claude and Rodrigo Canovas.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    * Neither the name of libcds nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

********************************************************************************/


#ifndef SRC_BASIC_TIME_H_
#define SRC_BASIC_TIME_H_

#include <libcds/libcds.h>
#include <sys/time.h>

namespace cds {
namespace basic {

/** Timer class.
 */
class Timer {
  public:
    /** Creates and starts the timer.
     */
    Timer() {
      restart();
    }

    /** Restarts the timer.
     */
    void restart() {
      getrusage(RUSAGE_SELF, &ru);
      initial_ru = ru.ru_utime;
      gettimeofday(&initial, NULL);
    }

    /** Stops the timer.
     */
    void stop() {
      getrusage(RUSAGE_SELF, &ru);
      final_ru = ru.ru_utime;
      gettimeofday(&final, NULL);
    }

    /** Computes the number of microsecond elapsed from start to stop
     * This time is for wall-clock time
     */
    double elapsedTime() {
      return (final.tv_sec - initial.tv_sec) * 1000000 + (final.tv_usec - initial.tv_usec);
    }

    /** Computes the number of microsecond elapsed from start to stop
    * This time is for  process CPU usage
    */
    double elapsedTimeCPU() {
      return (final_ru.tv_sec - initial_ru.tv_sec) * 1000000 + (final_ru.tv_usec - initial_ru.tv_usec);
    }

  protected:
    struct timeval initial, final;
    struct timeval initial_ru, final_ru;
    struct rusage ru;
};
};
};

#endif  // SRC_BASIC_TIME_H_
