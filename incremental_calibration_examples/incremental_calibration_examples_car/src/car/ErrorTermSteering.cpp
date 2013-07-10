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

#include "aslam/calibration/car/ErrorTermSteering.h"

#include <Eigen/Dense>

#include <sm/kinematics/rotations.hpp>

#include <aslam/calibration/data-structures/VectorDesignVariable.h>

namespace aslam {
  namespace calibration {

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

    ErrorTermSteering::ErrorTermSteering(
        const aslam::backend::EuclideanExpression& v_oo,
        const aslam::backend::EuclideanExpression& om_oo,
        VectorDesignVariable<11>* params,
        const Input& odo, const Covariance& Q) :
        _v_oo(v_oo),
        _om_oo(om_oo),
        _params(params),
        _odo(odo),
        _Q(Q) {
      setInvR(_Q.inverse());
      aslam::backend::DesignVariable::set_t dv;
      v_oo.getDesignVariables(dv);
      om_oo.getDesignVariables(dv);
      dv.insert(params);
      setDesignVariablesIterator(dv.begin(), dv.end());
    }

    ErrorTermSteering::ErrorTermSteering(const ErrorTermSteering& other) :
        ErrorTermFs<1>(other),
        _v_oo(other._v_oo),
        _om_oo(other._om_oo),
        _params(other._params),
        _odo(other._odo),
        _Q(other._Q) {
    }

    ErrorTermSteering& ErrorTermSteering::operator =
        (const ErrorTermSteering& other) {
      if (this != &other) {
        ErrorTermFs<1>::operator=(other);
       _v_oo = other._v_oo;
       _om_oo = other._om_oo;
       _params = other._params;
       _odo = other._odo;
       _Q = other._Q;
      }
      return *this;
    }

    ErrorTermSteering::~ErrorTermSteering() {
    }

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

    const ErrorTermSteering::Input& ErrorTermSteering::getInput() const {
      return _odo;
    }

    ErrorTermSteering::Input& ErrorTermSteering::getInput() {
      return _odo;
    }

    void ErrorTermSteering::setInput(const Input& odo) {
      _odo = odo;
    }

    const ErrorTermSteering::Covariance& ErrorTermSteering::getCovariance()
        const {
      return _Q;
    }

    ErrorTermSteering::Covariance& ErrorTermSteering::getCovariance() {
      return _Q;
    }

    void ErrorTermSteering::setCovariance(const Covariance& Q) {
      _Q = Q;
    }

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

    double ErrorTermSteering::evaluateErrorImplementation() {
      // useful pre-computations
      const double v_oo_x = _v_oo.toValue()(0);
      const double om_oo_z = _om_oo.toValue()(2);
      const double L = _params->getValue()(0);
      const double a0 = _params->getValue()(3);
      const double a1 = _params->getValue()(4);
      const double a2 = _params->getValue()(5);
      const double a3 = _params->getValue()(6);
      const double phi = atan(L * om_oo_z / v_oo_x);

      // build the error term
      error_t error;
      error(0) = a0 + a1 * _odo(0) + a2 * _odo(0) * _odo(0) +
        a3 * _odo(0) * _odo(0) * _odo(0) - phi;
      error(0) = sm::kinematics::angleMod(error(0));
      setError(error);
      return evaluateChiSquaredError();
    }

    void ErrorTermSteering::evaluateJacobiansImplementation() {
      // useful pre-computations
      const double v_oo_x = _v_oo.toValue()(0);
      const double om_oo_z = _om_oo.toValue()(2);
      const double L = _params->getValue()(0);

      // Jacobian with respect to odometry parameters
      Eigen::Matrix<double, 1, 11> Ht = Eigen::Matrix<double, 1, 11>::Zero();

      // Jacobian with respect to v_oo
      Eigen::Matrix<double, 1, 3> Hv = Eigen::Matrix<double, 1, 3>::Zero();

      // Jacobian with respect to om_oo
      Eigen::Matrix<double, 1, 3> Ho = Eigen::Matrix<double, 1, 3>::Zero();

      // steering measurement
      const double sDenom = v_oo_x * v_oo_x + (L * om_oo_z) * (L * om_oo_z);
      Ht(0, 0) = -v_oo_x * om_oo_z / sDenom;
      Ht(0, 3) = 1;
      Ht(0, 4) = _odo(0);
      Ht(0, 5) = _odo(0) * _odo(0);
      Ht(0, 6) = _odo(0) * _odo(0) * _odo(0);
      Hv(0, 0) = L * om_oo_z / sDenom;
      Ho(0, 2) = -L * v_oo_x / sDenom;

      // pass the Jacobians with the chain rule
      _v_oo.evaluateJacobians(_jacobians, Hv);
      _om_oo.evaluateJacobians(_jacobians, Ho);
      _jacobians.add(_params, Ht);
    }

  }
}
