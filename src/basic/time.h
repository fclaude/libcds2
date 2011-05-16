/**
 *    time.h
 *    Copyright (C) 2011  Francisco Claude F.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <libcds/libcds.h>

namespace cds_basic
{

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
      initial = clock();
    }

    /** Stops the timer.
     */
    void stop() {
      final = clock();
    }

    /** Computes the number of miliseconds elapsed from start to stop.
     */
    double elapsedTime() {
      return 1000.*(final-initial)/CLOCKS_PER_SEC;
    }

  protected:
    clock_t initial, final;
  };

};
