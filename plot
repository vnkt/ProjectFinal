mkfifo fifo
graphTrial fifo $2 &
cat $1 > fifo