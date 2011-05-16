/**
 *    test_timeh.h
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

int sleepTime = 0;
int keepWaiting = 1;

void * myThread(void*) {
  sleep(sleepTime);
  keepWaiting = 0;
  return NULL;
}

void testTimer(int i) {
  sleepTime = i;
  keepWaiting = 1;
  pthread_t th1;
  pthread_create(&th1, NULL, myThread, NULL);
  Timer t;
  while(keepWaiting);
  t.stop();
  pthread_join(th1, NULL);
  double diff = abs(t.elapsedTime()-1000*i);
  EXPECT_GE(10,diff);
}

TEST(Sleep,0_10Seconds) {
  for(int i=0;i<=10;i++)
    testTimer(i);
}


