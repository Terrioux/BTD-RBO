/// \file 
/// \brief Definitions of auxiliary functions for handling signals
/// \ingroup tools

#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <csignal>
#include "restart_based_optimizer.h"

extern volatile sig_atomic_t has_received_signal;    ///< returns 0 if no signal has been received yet, a positive value corresponding to the received signal otherwise

void signal_handler(int signal);    ///< allows to handle the given signal

void set_handler (Restart_Based_Optimizer * rbo, COP * inst);      ///< sets the handler


#endif

