#include "set/EllipticalSet2d.h"

#include <cmath>
#include <limits>

#include <fmt/format.h>

#include "IncompatibleTrajectoryException.h"
#include "TestUtil.h"

namespace helixtrajectory {

EllipticalSet2d::EllipticalSet2d(double xRadius, double yRadius, Direction direction)
        : xRadius(xRadius), yRadius(yRadius), direction(direction) {
}

EllipticalSet2d EllipticalSet2d::CircularSet2d(double radius, Direction direction) {
    return EllipticalSet2d(radius, radius, direction);
}

bool EllipticalSet2d::IsCircular() const noexcept {
    return xRadius == yRadius;
}

bool EllipticalSet2d::IsR2() const noexcept {
    return xRadius >= std::numeric_limits<double>::infinity()
        && yRadius >= std::numeric_limits<double>::infinity();
}

void EllipticalSet2d::CheckVector(double x, double y) const {
    bool isCompatible = GetConstraint(x, y);
    if (!isCompatible) {
        switch (direction) {
            case Direction::kInside:
                throw IncompatibleTrajectoryException(
                        fmt::format("({}, {}) is not on or inside an ellipse with x radius of {} and y radius of {}",
                        x, y, xRadius, yRadius));
            case Direction::kCentered:
                throw IncompatibleTrajectoryException(
                        fmt::format("({}, {}) is not on an ellipse with x radius of {} and y radius of {}",
                        x, y, xRadius, yRadius));
            case Direction::kOutside:
                throw IncompatibleTrajectoryException(
                        fmt::format("({}, {}) is not on or outside an ellipse with x radius of {} and y radius of {}",
                        x, y, xRadius, yRadius));
        }
    }
}

bool EllipticalSet2d::IsValid() const noexcept {
    return xRadius > 0.0 && yRadius > 0.0;
}
}