import sys, os
module = 'Utility2' if sys.version_info[0] < 3 else 'Utility3'
try:
	pr = __import__(module, fromlist = ['*'])
except:
	dirpath = os.path.dirname(os.path.abspath(__file__))
	basedir = os.path.basename(dirpath)
	try:
		pr = __import__(basedir + '.' + module, fromlist = ['*'])
	except:
		dirpath = os.path.dirname(dirpath)
		basedir = os.path.basename(dirpath) + '.' + basedir
		pr = __import__(basedir + '.' + module, fromlist = ['*'])
for k in dir(pr):
	globals()[k] = getattr(pr, k)

if 'Executable' in globals() and globals()['Executable'] == "<class 'cx_Freeze.freezer.Executable'>":
	from . import Utility3
