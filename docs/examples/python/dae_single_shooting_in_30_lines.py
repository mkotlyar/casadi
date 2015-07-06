#
#     This file is part of CasADi.
#
#     CasADi -- A symbolic framework for dynamic optimization.
#     Copyright (C) 2010-2014 Joel Andersson, Joris Gillis, Moritz Diehl,
#                             K.U. Leuven. All rights reserved.
#     Copyright (C) 2011-2014 Greg Horn
#
#     CasADi is free software; you can redistribute it and/or
#     modify it under the terms of the GNU Lesser General Public
#     License as published by the Free Software Foundation; either
#     version 3 of the License, or (at your option) any later version.
#
#     CasADi is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#     Lesser General Public License for more details.
#
#     You should have received a copy of the GNU Lesser General Public
#     License along with CasADi; if not, write to the Free Software
#     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
#
from casadi import *
from operator import itemgetter

"""
This example mainly intended for CasADi presentations. 
It implements direct single shooting for DAEs in only about 30 lines of code, 
using a minimal number of concepts.

We do not recommend using this example as a template for solving optimal control problems in practise.

Joel Andersson, 2012
"""

# Declare variables
x = SX.sym("x",2) # Differential states
z = SX.sym("z")   # Algebraic variable
u = SX.sym("u")   # Control
t = SX.sym("t")   # Time

# Differential equation
f_x = vertcat((z*x[0]-x[1]+u, x[0]))

# Algebraic equation
f_z = x[1]**2 + z - 1

# Lagrange cost term (quadrature)
f_q = x[1]**2 + x[1]**2 + u**2

# DAE callback function
f = SXFunction('f', [x,z,u,t],[f_x,f_z,f_q])

# Create an integrator
opts = {"tf":0.5} # interval length
I = Integrator('I', "idas", f, opts)

# All controls
U = MX.sym("U",20)

# Construct graph of integrator calls
X  = MX([0,1])
J = 0
for k in range(20):
  X,Q = itemgetter('xf','qf')(I({'x0':X,'p':U[k]}))
  J += Q   # Sum up quadratures
  
# NLP callback functions
nlp = MXFunction('nlp', nlpIn(x=U), nlpOut(f=J, g=X))

# Allocate an NLP solver
solver = NlpSolver("solver", "ipopt", nlp)

# Pass bounds, initial guess and solve NLP
arg = {}
arg["lbx"] = -0.75    # Lower variable bound
arg["ubx"] =  1.0     # Upper variable bound
arg["lbg"] =  0.0     # Lower constraint bound
arg["ubg"] =  0.0     # Upper constraint bound
arg["x0"] = 0.0       # Initial guess
res = solver(arg)
