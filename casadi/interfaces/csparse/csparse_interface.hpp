/*
 *    This file is part of CasADi.
 *
 *    CasADi -- A symbolic framework for dynamic optimization.
 *    Copyright (C) 2010-2014 Joel Andersson, Joris Gillis, Moritz Diehl,
 *                            K.U. Leuven. All rights reserved.
 *    Copyright (C) 2011-2014 Greg Horn
 *
 *    CasADi is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    CasADi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with CasADi; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


#ifndef CASADI_CSPARSE_INTERFACE_HPP
#define CASADI_CSPARSE_INTERFACE_HPP

/** \defgroup plugin_Linsol_csparse
 * Linsol with CSparse Interface
*/

/** \pluginsection{Linsol,csparse} */

/// \cond INTERNAL
#include <cs.h>
#include "casadi/core/function/linsol_impl.hpp"
#include <casadi/interfaces/csparse/casadi_linsol_csparse_export.h>

namespace casadi {
  /** \brief \pluginbrief{Linsol,csparse}
   * @copydoc Linsol_doc
   * @copydoc plugin_Linsol_csparse
   */
  class CASADI_LINSOL_CSPARSE_EXPORT CsparseInterface : public Linsol {
  public:

    // Create a linear solver given a sparsity pattern and a number of right hand sides
    CsparseInterface(const std::string& name, const Sparsity& sp, int nrhs);

    /** \brief  Create a new Linsol */
    static Linsol* creator(const std::string& name, const Sparsity& sp, int nrhs) {
      return new CsparseInterface(name, sp, nrhs);
    }

    // Destructor
    virtual ~CsparseInterface();

    // Initialize the solver
    virtual void init(const Dict& opts);

    /** \brief Allocate memory block */
    virtual Memory* memory() const;

    // Factorize the linear system
    virtual void linsol_factorize(Memory& mem, const double* A) const;

    // Solve the linear system
    virtual void linsol_solve(Memory& mem, double* x, int nrhs, bool tr) const;

    /// A documentation string
    static const std::string meta_doc;

    // Get name of the plugin
    virtual const char* plugin_name() const { return "csparse";}
  };

  struct CASADI_LINSOL_CSPARSE_EXPORT CsparseMemory : public Memory {
    // Destructor
    virtual ~CsparseMemory();

    // The linear system CSparse form (CCS)
    cs A;

    // The symbolic factorization
    css *S;

    // The numeric factorization
    csn *N;

    // Temporary
    std::vector<double> temp_;

    // Has the solve function been called once
    bool called_once_;
  };

} // namespace casadi

/// \endcond

#endif // CASADI_CSPARSE_INTERFACE_HPP
