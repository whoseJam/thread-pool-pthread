# thread-pool-pthread
a thread pool based on pthread

### intro
As we know that init and destroy a thread is time-comsuming.To avoid frequent init/destroy , I write a thread pool to help me manage these trivals.
A thread pool contain a group of threads waiting for jobs.The exceeded
