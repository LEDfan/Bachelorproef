%module Sim

// headers for generated file
%{
#include "sim/Sim.h"
extern stride::Sim CreateSim(std::string config);
%}

%include <std_string.i>

%include <std_shared_ptr.i>
%shared_ptr(stride::Sim)

%include "sim/Sim.h";
%include "cpp/create_sim.h"
