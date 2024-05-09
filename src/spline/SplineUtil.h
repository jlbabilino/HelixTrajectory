// Copyright (c) TrajoptLib contributors

#pragma once

#include <vector>

#include "spline/CubicHermitePoseSplineHolonomic.h"
#include "trajopt/path/InitialGuessPoint.h"

namespace trajopt {

std::vector<CubicHermitePoseSplineHolonomic>
CubicPoseControlVectorsFromWaypoints(
    const std::vector<std::vector<InitialGuessPoint>>
        initialGuessPoints);

}  // namespace trajopt
