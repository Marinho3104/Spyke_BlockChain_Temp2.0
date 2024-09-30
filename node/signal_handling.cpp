


#include "signal_handling.h"
#include <csignal>
#include <sched.h>
#include <unistd.h>

void node::handle_SIGINT(int) {

  const pid_t program_pid = getpid(); // Programs pid to start signal calls

  kill( program_pid, SIGUSR1 ); // Sinalize Node Connection Manager

}

void node::handle_SIGUSR1( int ) {}
