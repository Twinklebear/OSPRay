// ======================================================================== //
// Copyright 2009-2015 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "ospray/common/Managed.h"

//! \brief Define a function to create an instance of the InternalClass
//!  associated with ExternalName.
//!
//! \internal The function generated by this macro is used to create an
//!  instance of a concrete subtype of an abstract base class.  This
//!  macro is needed since the subclass type may not be known to OSPRay
//!  at build time.  Rather, the subclass can be defined in an external
//!  module and registered with OSPRay using this macro.
//!
#define OSP_REGISTER_TRANSFER_FUNCTION(InternalClass, ExternalName)     \
  extern "C" TransferFunction *ospray_create_transfer_function_##ExternalName() \
  { return(new InternalClass()); }

namespace ospray {

  //! \brief A TransferFunction is an abstraction that maps a value to
  //!  a color and opacity for rendering.
  //!
  //!  The actual mapping is unknown to this class, and is implemented
  //!  in subclasses.  A type string specifies a particular concrete
  //!  implementation to createInstance().  This type string must be
  //!  registered in OSPRay proper, or in a loaded module using
  //!  OSP_REGISTER_TRANSFER_FUNCTION.
  //!
  class TransferFunction : public ManagedObject {
  public:

    //! Constructor.
    TransferFunction() {};

    //! Destructor.
    virtual ~TransferFunction() {};

    //! Allocate storage and populate the transfer function.
    virtual void commit() = 0;

    //! Create a transfer function of the given type.
    static TransferFunction *createInstance(std::string type);

    //! Get the ISPC transfer function.
    void *getEquivalentISPC() const { return(getIE()); }

    //! A string description of this class.
    virtual std::string toString() const { return("ospray::TransferFunction"); }

  protected:

    //! Create the equivalent ISPC transfer function.
    virtual void createEquivalentISPC() = 0;

    //! Print an error message.
    void emitMessage(const std::string &kind, const std::string &message) const
    { std::cerr << "  " + toString() + "  " + kind + ": " + message + "." << std::endl; }

    //! Error checking.
    void exitOnCondition(bool condition, const std::string &message) const
    { if (!condition) return;  emitMessage("ERROR", message);  exit(1); }

    //! Warning condition.
    void warnOnCondition(bool condition, const std::string &message) const
    { if (!condition) return;  emitMessage("WARNING", message); }

  };

} // ::ospray
