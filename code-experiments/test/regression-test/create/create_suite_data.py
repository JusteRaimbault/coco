#!/usr/bin/env python
"""create test solutions and values for all suites in
data files. Optional input parameter is the number of
test solutions per problem, by default 10.

Data is a dictionary with the tupel `problem_index,
solution` as keys and tupel `f_value(s), constraints_values`
as values.

When a new suite is added only the data for this new suite
should be used, i.e. copied to the data folder. Alternatively
only data for this suite can be generated by editing the
`suite_names` variable of this file.
"""
from __future__ import division, print_function
import os, sys
import numpy as np
import cocoex
suite_names = cocoex.known_suite_names
# suite_names = 'bbob-largescale'

def is_finite(res):
    return all(np.all(np.isfinite(x)) for x in res)


def solution_array(dimension, number=10):
    """return an array of `dimension`-dimensional search points"""
    return ((np.random.randn(number) / (np.abs(np.random.randn(number)) + 1e-11)) *
        np.random.randn(number, dimension).T).T


def generate_test_data_for_suite(suite_name, filename, solution_array=solution_array):
    """write regression test data into file.

    Argument `solution_array(dimension)` is a function which returns a
    number of solutions of dimension `dimension`.
    """
    if os.path.exists(filename):
        raise ValueError("file '" + filename + "' exists already")
    suite = cocoex.Suite(suite_name, "", "")
    xfc_dict = {}
    for i, f in enumerate(suite):
        for x in solution_array(f.dimension):
            res = (f(x) if f.number_of_objectives == 1 else list(f(x)),
                   list(f.constraint(x) if f.number_of_constraints > 0 else []))
            if is_finite(res):
                xfc_dict[i, tuple(x)] = res  # tuple, because keys must be immutable
            else:
                print("rejected: ", f.name, i, x, res)

    with open(filename, 'w') as f:
        f.write(repr(xfc_dict))  # caveat: entries are by definition not sorted

if __name__ == "__main__":
    try:
        ndata = int(sys.argv[1])
    except IndexError:
        ndata = 10
    except ValueError:
        print(__doc__)
    try:
        ndata
    except:
        pass
    else:
        for name in suite_names:
            filename = "regression_test_%ddata_for_suite_" %ndata + name + ".py"
            try:
                print('generating %s...' % filename, end='')
                sys.stdout.flush()
                generate_test_data_for_suite(name,
                                             filename,
                                             lambda x: solution_array(x, ndata))
                print('done')
            except ValueError:
                print("file %s for suite %s exists, nothing done" % (filename, name))
