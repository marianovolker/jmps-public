import sys, os
module = 'DataStore2' if sys.version_info[0] < 3 else 'DataStore3'
try:
	dt = __import__(module, fromlist = ['*'])
except:
	dt = __import__(os.path.basename(os.path.dirname(os.path.abspath(__file__))) + '.' + module, fromlist = ['*'])
for k in dir(dt):
	globals()[k] = getattr(dt, k)

if 'Executable' in globals() and globals()['Executable'] == "<class 'cx_Freeze.freezer.Executable'>":
	from . import DataStore3
