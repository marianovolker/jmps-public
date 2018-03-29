import sys, os
module = 'pupyspot2' if sys.version_info[0] < 3 else 'protocol'
try:
	ps = __import__(module, fromlist = ['*'])
except:
	ps = __import__(os.path.basename(os.path.dirname(os.path.abspath(__file__))) + '.' + module, fromlist = ['*'])
for k in dir(ps):
	if k == 'Dispatcher':
		globals()['PupySPOT'] = getattr(ps, k)
	else:
		globals()[k] = getattr(ps, k)

if 'Executable' in globals() and globals()['Executable'] == "<class 'cx_Freeze.freezer.Executable'>":
	from . import protocol
