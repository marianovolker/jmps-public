"""
.. module:: GVRlib.Emv.cfgUtils
   :platform: Unix, Windows
   :synopsis: CSV and XML files management.
This module is in charge to manage CSV files and XML files used to create test cases
"""

import os, binascii

try:
    from spotPinpad import Enums
except:
    from .Pinpad import Enums

def evaluate(classtype, classmember):
    value = None
    try:                             # ?str?, ?bool?, ?int? and ?float?
        value = eval(classtype + '("' + classmember + '")')
        if isinstance(value, bool):  # Valid values are: True, False, 0 and 1
            try: value = eval(classtype + '(' + classmember + ')')
            except NameError: pass   # Unknown value, return None
    except SyntaxError: pass         # This value produces a syntax error, return None
#   except NameError:                # raise an exception
#   except AttributeError:           # raise an exception
    except ValueError:
        try:                         # ?int?
            value = eval(classtype + '("' + classmember + '", 16)')
        except ValueError: pass      # Not able to convert this value as ?int?, return None
        except TypeError:
            try:                     # ?class member?
                value = eval(classtype + '.' + classmember)
#           except NameError:        # raise an exception
#           except AttributeError:   # raise an exception
            except SyntaxError: pass # This value produces a syntax error, return None
            except ValueError: pass  # Not able to retrieve this class member, return None
    except TypeError:
        try:                         # ?bytes?
            pre_value = eval(classtype + '("' + classmember + '", "latin-1")')
            value = binascii.unhexlify(pre_value)
#       except NameError:            # raise an exception
        except binascii.Error: pass  # Not able to unhexlify this value, return None
        except TypeError: pass 
    return value

def get_testlist(testlistfile, verbose = False):
    """
    Imports test case from text file formatted as following:
    ``FIELD1 FIELD2 FIELD3.....FIELDn
    TYPE1  TYPE2  TYPE3......TYPEn
    VALUE1 VALUE2 VALUE3.....VALUEn # for test case 1
    VALUE1 VALUE2 VALUE3.....VALUEn # for test case 2
    .................................................
    VALUE1 VALUE2 VALUE3.....VALUEn # for test case n``
    :Parameters:
        - *testlistfile* (string): The complete file path.
        - *verbose* (boolean): Print the number of test imported and the number of the test excluded.
    :Returns: A list of dictionaries. The keys of any single dictionary are the name of the fields put in the first line of the text files. 
    """ 
    if os.path.isfile(testlistfile):
        testlist = []
        with open(testlistfile, 'r') as tf:
            fields = tf.readline().split()
            types = tf.readline().split()
            if len(fields) != len(types):
                raise Exception('Fields and types are not equally assigned!')
            imp = 0
            exc = 0
            for test in tf:
                test = test.strip()
                if test[0] != '#':
                    try:
                        data = test.split()[0:len(fields)]
                        if len(data) == len(fields):
                            values = list(map(evaluate, types, data))
                            testlist.append(dict(zip(fields, values)))
                            imp += 1
                        else:
                            exc += 1
                    except:
                        raise
                        exc +=1
                else:
                    exc += 1
            if verbose:
                print("List imported: " + str(imp) + " imported, " + str(exc) + " excluded.")
        return testlist
    raise Exception(testfile + ' NOT Found!')

if __name__ == '__main__':
    import sys
    if len(sys.argv) == 1:
        print(sys.argv[0] + ": It needs to be provided at least one argument (as file path).")
        sys.exit(1)
    for cfg in sys.argv[1:]:
        if not os.path.exists(cfg):
            print(cfg + ": This file NOT exists.")
            sys.exit(2)
        print('   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -')
        print('   >>>>>>>   ' + cfg + ' parameters loading ...   <<<<<<<')
        print('   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -')
        for line in get_testlist(cfg, verbose = True):
            print(line)
