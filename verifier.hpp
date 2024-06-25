#ifndef __VERIFIER__HPP
#define __VERIFIER__HPP

#include "register_allocation.hpp"

using namespace RA;

bool verifyAllocation(const std::string &path_to_graph,
                                          int num_registers,
                                          const RegisterAssignment &mapping);

#endif