/******************************************************************************
 * Copyright (C) 2015 by Jerome Maye                                          *
 * jerome.maye@gmail.com                                                      *
 ******************************************************************************/

/** \file splinesToFile.h
    \brief This file contains a utility to write spline data to file.
  */

#ifndef ASLAM_CALIBRATION_EGOMOTION_SPLINES_TO_FILE_H
#define ASLAM_CALIBRATION_EGOMOTION_SPLINES_TO_FILE_H

#include <fstream>

#include <boost/shared_ptr.hpp>

#include <aslam/splines/OPTBSpline.hpp>
#include <aslam/splines/OPTUnitQuaternionBSpline.hpp>

#include <bsplines/EuclideanBSpline.hpp>
#include <bsplines/UnitQuaternionBSpline.hpp>

namespace bsplines {

  struct NsecTimePolicy;

}
namespace aslam {
  namespace calibration {

    class IncrementalEstimator;

    /** \name Types definitions
      @{
      */
    /// Shared pointer to incremental estimator
    typedef boost::shared_ptr<IncrementalEstimator> IncrementalEstimatorSP;
    /// Rotation spline
    typedef aslam::splines::OPTBSpline<
      bsplines::UnitQuaternionBSpline<Eigen::Dynamic,
      bsplines::NsecTimePolicy>::CONF>::BSpline RotationSpline;
    /// Rotation spline shared pointer
    typedef boost::shared_ptr<RotationSpline> RotationSplineSP;
    /// Translation spline
    typedef aslam::splines::OPTBSpline<
      bsplines::EuclideanBSpline<Eigen::Dynamic, 3,
      bsplines::NsecTimePolicy>::CONF>::BSpline TranslationSpline;
    /// Euclidean spline shared pointer
    typedef boost::shared_ptr<TranslationSpline> TranslationSplineSP;
    /// Rotation spline simple
    typedef bsplines::UnitQuaternionBSpline<Eigen::Dynamic,
      bsplines::NsecTimePolicy>::TYPE RotationSplineS;
    /// Rotation spline simple shared pointer
    typedef boost::shared_ptr<RotationSplineS> RotationSplineSSP;
    /// Translation spline simple
    typedef bsplines::EuclideanBSpline<Eigen::Dynamic, 3,
      bsplines::NsecTimePolicy>::TYPE TranslationSplineS;
    /// Euclidean spline simple shared pointer
    typedef boost::shared_ptr<TranslationSplineS> TranslationSplineSSP;

    /** @}
      */

    /** \name Methods
      @{
      */
    /// Write spline data from incremental estimator to file
    void writeSplines(const IncrementalEstimatorSP& estimator, double dt,
      std::ofstream& stream);
    /// Write spline data from spline structure
    void writeSplines(const TranslationSplineSP& transSpline, const
      RotationSplineSP& rotSpline, double dt, std::ofstream& stream);
    /// Write spline data from simple spline structure
    void writeSplines(const TranslationSplineSSP& transSpline, const
      RotationSplineSSP& rotSpline, double dt, std::ofstream& stream);
    /** @}
      */

  }
}

#endif // ASLAM_CALIBRATION_EGOMOTION_SPLINES_TO_FILE_H
