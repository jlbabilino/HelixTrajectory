#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

#include <fmt/format.h>

#include "path/HolonomicPath.h"
#include "drivetrain/SwerveDrivetrain.h"
#include "path/InitialGuessPoint.h"
#include "obstacle/Obstacle.h"
#include "OptimalTrajectoryGenerator.h"
#include "constraint/ObstacleConstraint.h"
#include "constraint/TranslationConstraint.h"
#include "trajectory/HolonomicTrajectory.h"
#include "IncompatibleTrajectoryException.h"
#include "set/ConeSet2d.h"
#include "TestUtil.h"
#include "solution/SwerveSolution.h"

int main() {

    // auto opti = casadi::Opti();

    // auto variable1 = opti.variable(4, 1);
    // auto variable2 = opti.variable(4, 1);

    // std::cout << "pose    : " << variable1 << "\n";
    // std::cout << "variable: " << variable2 << std::endl;
    // std::cout << "pose(0): " << pose(slice0) << "\n";
    // std::cout << "pose(1): " << pose(slice1) << std::endl;

    using namespace helixtrajectory;
    SwerveDrivetrain swerveDrivetrain(45, 6,
            {SwerveModule(+0.6, +0.6, 0.04, 70, 2),
             SwerveModule(+0.6, -0.6, 0.04, 70, 2),
             SwerveModule(-0.6, +0.6, 0.04, 70, 2),
             SwerveModule(-0.6, -0.6, 0.04, 70, 2)});

    fmt::print("{}\n", swerveDrivetrain.modules[0]);

    Obstacle bumpers(0, {{+0.5, +0.5}, {-0.5, +0.5}, {-0.5, -0.5}, {+0.5, -0.5}});

    // // CIRCULAR PATH
    // HolonomicPath holonomicPath(HolonomicPath({
    //     HolonomicWaypoint(
    //         {Constraint(PoseConstraint(RectangularSet2d{ 4,  0},  0.00))},
    //         {HolonomicConstraint(VelocityConstraint{RectangularSet2d(0, 0)}), HolonomicConstraint(AngularVelocityConstraint(0.0))},
    //         {},
    //         {},
    //         0,
    //         {InitialGuessPoint( 4,  0,  0.00)}),
    //     HolonomicWaypoint(
    //         {PoseConstraint(RectangularSet2d{ 0,  4},  1.57)},
    //         {},
    //         {},{},
    //         10,
    //         {InitialGuessPoint( 0,  4,  1.57)}),
    //     HolonomicWaypoint(
    //         {PoseConstraint(RectangularSet2d{-4,  0},  0.00)},
    //         {},
    //         {},
    //         {},
    //         10,
    //         {InitialGuessPoint(-4,  0,  0.00)}),
    //     HolonomicWaypoint(
    //         {PoseConstraint(RectangularSet2d{ 0, -4}, -1.57)},
    //         {},
    //         {},
    //         {},
    //         10,
    //         {InitialGuessPoint( 0, -4, -1.57)}),
    //     HolonomicWaypoint(
    //         {PoseConstraint(RectangularSet2d{ 4,  0},  0.00)},
    //         {VelocityConstraint{RectangularSet2d{0, 0}}, AngularVelocityConstraint(0.0)},
    //         {},
    //         {},
    //         10,
    //         {InitialGuessPoint( 4,  0,  0.00)})},
    //     bumpers));

    // HARD OBSTACLE TEST:
    // const std::vector<InitialGuessPoint> guesses = {
    //     { 0.00,  0.00, 0.00},
    //     { 2.00,  1.50, 0.00},
    //     { 3.60,  0.00, -M_PI_2},
    //     { 2.00, -1.50, -M_PI},
    //     { 0.00,  0.00, -M_PI}
    // };
    // Obstacle cone(0.6, {{2.0, 0.0}});
    // HolonomicPath holonomicPath({
    //     HolonomicWaypoint({PoseConstraint(RectangularSet2d{ 0,  0},  0.00)}, {VelocityConstraint(RectangularSet2d{0, 0}), AngularVelocityConstraint(0.0)}, {                        }, {},   0, {InitialGuessPoint( 0,  0,   0.00)}),
    //     HolonomicWaypoint({PoseConstraint(RectangularSet2d{ 0,  0}, -M_PI)}, {VelocityConstraint(RectangularSet2d{0, 0}), AngularVelocityConstraint(0.0)}, {ObstacleConstraint(cone)}, {}, 40,                             guesses)},
    //     bumpers);

    // SIMPLE OBSTACLE TEST:
    // const std::vector<InitialGuessPoint> guesses = {
    //     { 0.00,  1.60, 0.00},
    //     { 4.00,  0.00, 0.00}
    // };
    // Obstacle cone(1.0, {{2.0, 0.0}});
    // HolonomicPath holonomicPath({
    //     HolonomicWaypoint({PoseConstraint(RectangularSet2d{ 0,  0},  0.00)}, {VelocityConstraint(RectangularSet2d{0, 0}), AngularVelocityConstraint(0.0)}, {                        }, {},   0, {InitialGuessPoint( 0,  0,   0.00)}),
    //     HolonomicWaypoint({PoseConstraint(RectangularSet2d{ 4,  0},  0.00)}, {VelocityConstraint(RectangularSet2d{0, 0}), AngularVelocityConstraint(0.0)}, {ObstacleConstraint(cone)}, {}, 36,                             guesses)},
    //     bumpers);

    // SIMPLE MOTION PROFILE
    // auto con = Constraint(TranslationConstraint(RectangularSet2d(0, 0)));
    auto con = VelocityConstraint(RectangularSet2d(0, 0));
    fmt::print("{}\n\n\n", con);
    HolonomicPath holonomicPath(HolonomicPath({
        HolonomicWaypoint(
            {TranslationConstraint(RectangularSet2d(0, 0)),         HeadingConstraint(0)},
            {   VelocityConstraint(RectangularSet2d(0, 0)), AngularVelocityConstraint(0)},
            {},
            {},
            0,
            {InitialGuessPoint( 0,  0,  0.00)}),
        HolonomicWaypoint(
            {TranslationConstraint(RectangularSet2d(4, 0)),         HeadingConstraint(0)},
            {   VelocityConstraint(RectangularSet2d(0, 0)), AngularVelocityConstraint(0)},
            {},
            {},
            30,
            {InitialGuessPoint( 4,  0,  0.00)})},
        bumpers));

    // SOLVE
    SwerveSolution solution = OptimalTrajectoryGenerator::Generate(swerveDrivetrain, holonomicPath);
    fmt::print("{}\n", solution);
}