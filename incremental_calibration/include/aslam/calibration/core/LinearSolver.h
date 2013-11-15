/******************************************************************************
 * Copyright (C) 2013 by Jerome Maye                                          *
 * jerome.maye@gmail.com                                                      *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

/** \file LinearSolver.h
    \brief This file defines the LinearSolver class, which is a specific linear
           solver for incremental calibration problems.
  */

#ifndef ASLAM_CALIBRATION_CORE_LINEAR_SOLVER_H
#define ASLAM_CALIBRATION_CORE_LINEAR_SOLVER_H

#include <vector>
#include <string>

#include <Eigen/Core>

#include <cholmod.h>
#include <SuiteSparseQR.hpp>

#include <aslam/backend/LinearSystemSolver.hpp>

namespace sm {

  class PropertyTree;

}
namespace aslam {
  namespace backend {

    class DesignVariable;
    class ErrorTerm;

  }
  namespace calibration {

    /** The class LinearSolver implements a specific linear solver for
        incremental calibration problems.
        \brief Linear solver for incremental calibration
      */
    class LinearSolver :
      public aslam::backend::LinearSystemSolver {
    public:
      /** \name Types definitions
        @{
        */
      /// Options for the linear solver
      struct Options {
          Options() {}
      };
      /// Self type
      typedef LinearSolver Self;
      /** @}
        */

      /** \name Constructors/destructor
        @{
        */
      /// Constructor with options structure
      LinearSolver(const Options& options = Options());
      /// Constructor with property tree configuration
      LinearSolver(const sm::PropertyTree& config);
      /// Copy constructor
      LinearSolver(const Self& other) = delete;
      /// Copy assignment operator
      LinearSolver& operator = (const Self& other) = delete;
      /// Move constructor
      LinearSolver(Self&& other) = delete;
      /// Move assignment operator
      LinearSolver& operator = (Self&& other) = delete;
      /// Destructor
      virtual ~LinearSolver();
      /** @}
        */

      /** \name Methods
        @{
        */
      /// Build the system of equations assuming things have been set
      virtual void buildSystem(size_t numThreads, bool useMEstimator);
      /// Solve the system of equations assuming things have been set
      virtual bool solveSystem(Eigen::VectorXd& x);
      /// Helper function for dog leg implementation / steepest descent solution
      virtual double rhsJtJrhs();
      /// Solve a system of equations
      /** @}
        */

      /** \name Accessors
        @{
        */
      /// Returns the options
      const Options& getOptions() const;
      /// Returns the options
      Options& getOptions();
      /// Returns the name of the solver
      virtual std::string name() const;
      /** @}
        */

    protected:
      /** \name Protected methods
        @{
        */
      /// Initialize the matrix structure for the problem
      virtual void initMatrixStructureImplementation(const
        std::vector<aslam::backend::DesignVariable*>& dvs, const
        std::vector<aslam::backend::ErrorTerm*>& errors, bool
        useDiagonalConditioner);
      /** @}
        */

      /** \name Protected members
        @{
        */
      /// Linear solver options
      Options _options;
      /// Cholmod common structure
      cholmod_common _cholmod;
      /// Caching factorization if needed
      SuiteSparseQR_factorization<double>* _factor;
      /** @}
        */

    };

  }
}

#endif // ASLAM_CALIBRATION_CORE_LINEAR_SOLVER_H
