/******************************************************************************
 * Copyright (C) 2013 by Jerome Maye                                          *
 * jerome.maye@gmail.com                                                      *
 ******************************************************************************/

/** \file DigammaFunction.h
    \brief This file defines the DigammaFunction class, which represents the
           digamma function
  */

#ifndef ASLAM_CALIBRATION_FUNCTIONS_DIGAMMAFUNCTION_H
#define ASLAM_CALIBRATION_FUNCTIONS_DIGAMMAFUNCTION_H

#include "aslam/calibration/functions/ContinuousFunction.h"

namespace aslam {
  namespace calibration {

    /** The DigammaFunction class represents the digamma function.
        \brief Digamma function
      */
    template <typename X> class DigammaFunction :
      public ContinuousFunction<double, X> {
      /** \name Private constructors
        @{
        */
      /// Copy constructor
      DigammaFunction(const DigammaFunction& other);
      /// Assignment operator
      DigammaFunction& operator = (const DigammaFunction& other);
      /** @}
        */

    public:
      /** \name Types
        @{
        */
      /// Variable type
      typedef typename ContinuousFunction<double, X>::Domain VariableType;
      /** @}
        */

      /** \name Constructors/destructor
        @{
        */
      /// Default constructor
      DigammaFunction();
      /// Destructor
      virtual ~DigammaFunction();
      /** @}
        */

      /** \name Accessors
        @{
        */
      /// Access the function value for the given argument
      virtual double getValue(const VariableType& argument) const;
      /** @}
        */

    protected:

    };

  }
}

#include "aslam/calibration/functions/DigammaFunction.tpp"

#endif // ASLAM_CALIBRATION_FUNCTIONS_DIGAMMAFUNCTION_H
