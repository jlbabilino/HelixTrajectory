// Copyright (c) TrajoptLib contributors

#include "optimization/CasADiOpti.h"

#include <casadi/casadi.hpp>

#include "optimization/Cancellation.h"
#include "optimization/CasADiIterCallback.h"

namespace trajopt {

casadi::MX CasADiOpti::DecisionVariable() {
  return opti.variable();
}

void CasADiOpti::Minimize(const casadi::MX& objective) {
  opti.minimize(objective);
}

void CasADiOpti::SubjectTo(const casadi::MX& constraint) {
  opti.subject_to(constraint);
}

void CasADiOpti::SetInitial(const casadi::MX& expression, double value) {
  opti.set_initial(expression, value);
}

[[nodiscard]]
expected<void, std::string> CasADiOpti::Solve() {
  GetCancellationFlag() = 0;
  const auto callback =
      new const CasADiIterCallback("f", opti.nx(), opti.ng(), opti.np());

  auto pluginOptions = casadi::Dict();
  pluginOptions["iteration_callback"] = *callback;
  pluginOptions["ipopt.print_level"] = 0;
  pluginOptions["ipopt.sb"] = "yes";
  pluginOptions["print_time"] = 0;

  // I don't try-catch this next line since it should always work.
  // I'm assuming the dynamic lib is on the path and casadi can find it.
  opti.solver("ipopt", pluginOptions);

  try {
    solution = opti.solve();
  } catch (const std::exception& e) {
    return unexpected{e.what()};
  }

  return {};
}

double CasADiOpti::SolutionValue(const casadi::MX& expression) const {
  if (!solution.has_value()) {
    return 0.0;
  }

  try {
    return static_cast<double>(solution->value(expression));
  } catch (...) {
    return 0.0;
  }
}

}  // namespace trajopt
