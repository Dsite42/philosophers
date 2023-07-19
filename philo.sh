#!/bin/bash

for i in {1..10}
do
   echo "Execution $i:"
   
   ./philo-sh 4 410 200 200 >out$i &
   PHIL_PID=$!

   {
      sleep 600
      kill $PHIL_PID
   } &
   SLEEP_PID=$!

   wait $PHIL_PID
   PHIL_STATUS=$?

   if kill -0 $SLEEP_PID 2>/dev/null; then
       kill $SLEEP_PID
       if ((PHIL_STATUS > 128)); then
           echo "'philo' was killed after running for 10 minutes"
       fi
   else
       echo "'philo' finished within 10 minutes"
   fi
done
