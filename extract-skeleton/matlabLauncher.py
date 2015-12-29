#!/usr/bin/python

"""matlabLauncher.py [-x] <m-file> [arglist]

Runs the given matlab script with an optional argument list.
This program exists since Matlab won't pass arguments to
scripts on the command line by itself.  Note that Matlab
is started from the PWD, so make sure you're in your
$MATLABHOME if you require customizations to the
environment (e.g. pathdef.m) that are stored there.

By default, Matlab is started with no graphics support, and
hence no ability to display plots, images, or interactive
windows.

The -x argument turns on graphics support.  Note that
M-files run this way may display plots, but you should be
sure to include a 'pause' command in the script after you
generate your figure, or Matlab won't get a chance to
display it before it exits.
"""
# Written December 2007 by Jadrian Miles
# Modified July 2009 by Jadrian Miles to fix a backgrounding bug

import sys, os, tempfile

def matlabLauncher(mfile, args=[], useX=False):
	"""matlabLauncher(mfile, args=[], useX=False): Execute the Matlab
script 'mfile' in a non-interactive Matlab instance, passing in the
argument list 'args'.  Note that all elements of "args" should be
strings, just as they come in from the command line."""
	
	DEBUG = True
	
	if not os.path.isfile(mfile):
		raise TypeError('%s is not an existing file' % mfile)
	
	# Create the launch script for Matlab to run
	(mdir, mfile, mext) = splitPath(os.path.abspath(mfile))
	(handle, tmppath) = tempfile.mkstemp('.m')
	while not os.path.basename(tmppath)[:-2].isalpha():
		os.remove(tmppath)
		(handle, tmppath) = tempfile.mkstemp('.m')
	tmp = os.fdopen(handle, 'w')
	tmp.write("cd %s\n" % os.getcwd())
	tmp.write("path('%s', path);\n" % mdir)
	tmp.write("%s(%s);\n" % (mfile, convertArgs(args)))
	tmp.write("exit;\n")
	tmp.close()
	
	# Run Matlab with the launch script
	modeOption = '-nojvm'
	if useX:
		modeOption = '-nodesktop'
	# We use input redirection rather than the -r option because -r doesn't
	# allow backgrounding the process.
	# See <http://www.mathworks.com/support/solutions/data/1-43HA6J.html>
	cmd = '/usr/local/bin/matlab %s -nosplash < "%s"' % (modeOption, tmppath)
	
	if DEBUG:
		print cmd
		print "Contents of %s are:" % tmppath
		scriptdump = open(tmppath)
		print scriptdump.read()
		scriptdump.close()
	
	print "Starting Matlab..."
	sys.stdout.flush()
	os.system(cmd)
	
	# Clean up the temporary launch script file
	os.remove(tmppath)

def splitPath(path):
	"""Split a path into <directory>/<name><extension>"""
	(directory, name) = os.path.split(path)
	(name, extension) = os.path.splitext(name)
	return (directory, name, extension)

def convertArgs(args):
	"""Convert the argument array into an equivalent string for inclusion
in the Matlab launch script"""
	converted = []
	for arg in args:
		try:
			# if we can convert to float, we want to pass this argument
			# to the Matlab function as a number, not a string
			float(arg)
			converted = converted + [arg]
		except:
			# if conversion to float failed, the argument is a string
			# and must be quoted
			converted = converted + ["'%s'" % arg]
	return ", ".join(converted)

if __name__ == "__main__":
	try:
		mfile = sys.argv[1]
	except IndexError:
		sys.stderr.write(__doc__)
		sys.exit(1)
	
	useX = False
	margs = 2
	if mfile == '-x':
		useX = True
		margs = 3
		try:
			mfile = sys.argv[2]
		except IndexError:
			sys.stderr.write(__doc__)
			sys.exit(1)
	
	args = sys.argv[margs:]
	
	sys.exit(matlabLauncher(mfile, args, useX))
