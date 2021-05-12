# vim: set noexpandtab sw=4 ts=4:
from subprocess import Popen, PIPE
import subprocess
import os
import sys
import time
from array import array


class Option(object):

	def __init__(self, name, desc, default_value = None, trse_ini_key = None):
		self.name = name
		self.default_value = default_value
		self.value = default_value
		self.desc = desc
		self.trse_ini_key = trse_ini_key


options = [
	Option('trse', 'Path to the TRSE binary. Required.'),
	Option('trse.ini', 'Path to the trse.ini file. If provided, used to determine all tools path although later options can override them.'),
	Option('x64', 'Path to the x64 binary (VICE emulator for C64)', trse_ini_key='emulator'),
	Option('cap32', 'Path to the cap32 binary (Amstrad CPC emulator)', trse_ini_key='amstradcpc_emulator'),
	Option('dosbox', 'Path to the dosbox binary (DOS emulator)', trse_ini_key='dosbox'),
	Option('sameboy', 'Path to the sameboy binary (GameBoy emulator)', trse_ini_key='gameboy_emulator'),
	Option('assemble', 'Whether to assemble when compiling ("yes" or "no")', default_value='yes'),
]


def Usage(rc):
	print("Usage: python validate_all.py --trse [trse exe file] <--trse.ini [ trse.ini ]> <--x64 [ x64 vice emulator ]> <--cap32 [ cap32 emulator ]> <--dosbox [ dosbox emulator ]> <--sameboy [ sameboy emulator ]> <--assemble [yes | no]>")
	print("must be run in the 'validate_all' directory.")
	for opt in options:
		print("  %s: %s [default: %s]" % (opt.name, opt.desc, opt.default_value))
	exit(rc)


def GetOption(name):
	for opt in options:
		if opt.name == name:
			return opt.value
	raise KeyError("Unknown option '%s'" % name)


def ParseOptions():
	global options
	valid = True
	i = 1
	while i < len(sys.argv):
		opt = sys.argv[i]
		i += 1
		try:
			optarg = sys.argv[i]
			for o in options:
				if opt == '--%s' % o.name:
					print("Set '%s' to '%s'" % (o.name, optarg))
					o.value = optarg
					i += 1
					break
			else:
				print("Unrecognized option '%s'" % opt)
				valid = False
		except IndexError:
			print("Option without value: '%s'" % opt)
			valid = False
			break
	if not valid:
		Usage(1)


def ParseTrseIni(trse_ini):
	global options
	print("Parsing %s" % trse_ini)
	with open(trse_ini) as f:
		for line in f.readlines():
			for opt in options:
				if not opt.trse_ini_key:
					continue
				if line.startswith('%s = ' % opt.trse_ini_key):
					value = line.split('=')[1].strip()
					print("Found '%s': '%s' (%s)" % (opt.name, value, opt.trse_ini_key))
					if opt.value:
						print("Option '%s' already set to '%s', not overriding with '%s'" % (opt.name, opt.value, value))
						break
					opt.value = value
					break


ParseOptions()
if GetOption('trse.ini'):
	ParseTrseIni(GetOption('trse.ini'))

trse = GetOption('trse')
x64 = GetOption('x64')
cap32 = GetOption('cap32')
assemble = GetOption('assemble')
dosbox = GetOption('dosbox')
sameboy = GetOption('sameboy')

if (dosbox.endswith(".app")):
	dosbox+="/Contents/MacOS/DOSBox"

if not trse:
	Usage(1)


lp = "../tutorials/"


tests = [ ]


def fillRasList(idx,path):
	for file in os.listdir(lp+tests[idx][0]+"/"+path):
		if file.endswith(".ras"):
			tests[idx][1].append(path+"/"+file)


# SPECTRUM

tests.append([ "SPECTRUM/tutorials/",[]])
fillRasList(len(tests)-1,".")


# CPC

tests.append([ "AMSTRADCPC/Morketid",["main.ras"]])
tests.append([ "AMSTRADCPC/tutorials",[]])
fillRasList(len(tests)-1,".")
tests.append([ "AMSTRADCPC/UnitTests", ["unittests.ras"]]);


# AMIGA

tests.append([ "AMIGA/tutorials",[]])
fillRasList(len(tests)-1,".")
tests.append([ "AMIGA/small_intro",["intro.ras"]])

# ATARI

tests.append([ "ATARI520ST/tutorials",[]])
fillRasList(len(tests)-1,".")

# BBC

tests.append([ "BBCM/Tutorials",[]])
fillRasList(len(tests)-1,".")
# Has dependencies, ignore
#tests.append([ "BBCM/BBCDemoSetup",["demo.ras"]])

# MEGA64

tests.append([ "MEGA65/Tutorials",[]])
fillRasList(len(tests)-1,".")

# ATARI2600

tests.append([ "ATARI2600/tutorials",[]])
fillRasList(len(tests)-1,".")




# X86
tests.append([ "X86/CGA",[]])
fillRasList(len(tests)-1,".")

tests.append([ "X86/VGA_386",[]])
fillRasList(len(tests)-1,".")

tests.append([ "X86/unittests", ["utests.ras"]])


# PET

# fails for some reason
#tests.append([ "PET/examples/",[]])
#fillRasList(len(tests)-1,".")

tests.append([ "PET/pbm_examples/",[]])
fillRasList(len(tests)-1,".")

tests.append([ "PET/PETFrog/",["petfrog.ras"]])

# NES

tests.append([ "NES/intro_tutorial/",[]])
fillRasList(len(tests)-1,".")

# OK64

tests.append([ "OK64/Tutorials/",[]])
fillRasList(len(tests)-1,".")
tests.append([ "OK64/OkComputer/",["main.ras"]])

# ************ BUILD TEST LIST 
# C64 
tests.append([ "C64/TutorialGame_RogueBurgerOne",["RogueBurgerOne.ras"]])
tests.append([ "C64/Tutorials",[]])
fillRasList(len(tests)-1,"easy")
fillRasList(len(tests)-1,"intermediate")
fillRasList(len(tests)-1,"advanced")
tests.append([ "C64/DemoEffects_raytracer",[]])
fillRasList(len(tests)-1,".")
tests.append([ "C64/16kb_cartridge_project", []]);
fillRasList(len(tests)-1,".")
tests.append([ "C64/4kDreams", ["intro.ras"]]);
tests.append([ "C64/DemoEffects_raytracer", []]);
fillRasList(len(tests)-1,".")
# Fails with various errors
#tests.append([ "C64/DemoMaker", []]);
#fillRasList(len(tests)-1,".")
tests.append([ "C64/Disk_loader_project", []]);
fillRasList(len(tests)-1,".")
tests.append([ "C64/Floskel", []]);
fillRasList(len(tests)-1,".")
tests.append([ "C64/MusicPlayer", []]);
fillRasList(len(tests)-1,".")
tests.append([ "C64/Olimp", []]);
fillRasList(len(tests)-1,".")
tests.append([ "C64/TutorialGame_Introduction", []]);
fillRasList(len(tests)-1,".")
tests.append([ "C64/UnitTests", ["unittests.ras"]]);
#fillRasList(len(tests)-1,"arithmetic")
#fillRasList(len(tests)-1,"conditional")
#fillRasList(len(tests)-1,"keywords")
#fillRasList(len(tests)-1,"structures")


# VIC 20

tests.append([ "VIC20/PurplePlanetYo",["demo.ras"]])
tests.append([ "VIC20/VicNibbler",["nibbler.ras"]])
tests.append([ "VIC20/cheesy",["main.ras"]])
tests.append([ "VIC20/PumpKid",["pumpkid.ras"]])
tests.append([ "VIC20/tutorials",[]])
fillRasList(len(tests)-1,".")

# Gameboy

tests.append([ "GAMEBOY/tutorials",[]])
fillRasList(len(tests)-1,".")
tests.append([ "GAMEBOY/yo-grl",["demo.ras"]])



def c(path,f1):
	projectFile = ""
	for file in os.listdir("."):
		if file.endswith(".trse"):
			projectFile = file

	if (projectFile==""):
		print("Could not find project file for : "+path+" : " +f1)
		return 1

	if (not os.path.exists(f1)):
		print("Could not find file : "+path+" : " +f1)
		return 1


	result = subprocess.run([trse,"-cli",'--define','__CI','op=project','project='+projectFile,'input_file='+f1,'assemble='+assemble], stdout=PIPE, stderr=subprocess.STDOUT)
	if result.stdout: print(result.stdout.decode('utf-8'))
	return result.returncode

#	print(rVal)
#	stdout, stderr = process.communicate()
#	print stdout


def HackSymbolFileChangeDir(symFile, path):
	with open(symFile, "r") as f:
		contents = f.read()

	contents = contents.replace("$DIR",path)

	with open(symFile, "w") as f:
		f.write(contents)


failed = []
orgPath = os.getcwd()
print(orgPath)


def C64UnitTests():
	os.chdir(orgPath)
	if x64 and os.path.exists(x64):
		path =  os.path.abspath(lp+'C64/UnitTests/')
		test6502 =  path + "/unittests"
		HackSymbolFileChangeDir(test6502+".sym",path)
		resultFile = path+"/results.bin"
		if (os.path.exists(resultFile)):
			os.remove(resultFile)

		try:
			# Note: we deactivate sound so that it works on GitHub Actions
			# Yes, '+sound' means deactivate sound ...
			result = subprocess.run([x64,"+sound","-autostartprgmode","1","-moncommands",test6502+".sym",test6502+".prg",], timeout=10*60, stdout=PIPE, stderr=subprocess.STDOUT)
			if result.stdout: print(result.stdout.decode('utf-8'))
		except subprocess.TimeoutExpired as err:
			print("ERROR: Timeout for C64 unit tests expired.")
			failed.append([path, "unittest.prg"])
			if err.stdout: print(err.stdout.decode('utf-8'))
#		print(os.path.exists(resultFile))
		if (not os.path.exists(resultFile)):
			time.sleep(4)
		with open(resultFile, "rb") as f:
			data = array('B')
			# byte 2 should have 0 for success 
			data.fromfile(f, 3)
			if (data[2]==1):
				failed.append([path, "unittest.prg"])
				print("******* SEVERE ERROR : 6502 Execution unit test FAILED! Please fix up unittest.prg")
			else:
				print("6502 Unittest SUCCESS!")
	else:
		print("Skipping C64 tests: emulator path '%s'" % x64)

def DOSUnitTests():
	os.chdir(orgPath)
	if dosbox and os.path.exists(dosbox):
		path =  os.path.abspath(lp+'X86/unittests/')
		resultFile = path+"/RESULT.BIN"
		if (os.path.exists(resultFile)):
			os.remove(resultFile)

		try:
			result = subprocess.run([dosbox,"-noautoexec","-exit",path+"/utests.exe"], timeout=10*60, stdout=PIPE, stderr=subprocess.STDOUT)
			if result.stdout: print(result.stdout.decode('utf-8'))
		except subprocess.TimeoutExpired as err:
			print("ERROR: Timeout for DOS unit tests expired.")
			if err.stdout: print(err.stdout.decode('utf-8'))
#		print(os.path.exists(resultFile))
		if (not os.path.exists(resultFile)):
			time.sleep(1)
		with open(resultFile, "rb") as f:
			data = array('B')
			# byte 2 should have 0 for success 
			data.fromfile(f, 1)
			if (data[0]==1):
				failed.append([path, "utests.exe"])
				print("******* SEVERE ERROR : DOS Execution unit test FAILED! Please fix up unittests")
			else:
				print("DOS Unittest SUCCESS!")
	else:
		print("Skipping DOS tests: emulator path '%s'" % dosbox)



def CPCUnitTests():
	os.chdir(orgPath)
	if cap32 and os.path.exists(cap32):
		path =  os.path.abspath(lp+'AMSTRADCPC/UnitTests/')
		os.chdir(os.path.dirname(cap32))
		resultFile = os.path.dirname(cap32)+"/printer.dat"
		if (os.path.exists(resultFile)):
			os.remove(resultFile)

		try:
			#print([cap32,"-i",path+"/unittests.bin","-o","0x4000",])
			result = subprocess.run([cap32,"-O","system.printer=1","-O","file.printer_file=printer.dat","-i",path+"/unittests.bin","-o","0x4000","-a","CAP32_WAITBREAK CAP32_EXIT"], timeout=10*60, stdout=PIPE, stderr=subprocess.STDOUT)
			if result.stdout: print(result.stdout.decode('utf-8'))
		except subprocess.TimeoutExpired as err:
			print("ERROR: Timeout for CPC unit tests expired.")
			failed.append([path, "unittests.ras"])
			if err.stdout: print(err.stdout.decode('utf-8'))
#		print(os.path.exists(resultFile))
		with open(resultFile, "r") as f:
			result = f.read().strip()
			print(result)
			if result != "SUCCESS":
				failed.append([path, "unittests.ras"])
				print("******* SEVERE ERROR : Amstrad CPC Execution unit test FAILED! Please fix up unittests.ras: %s" % result)
			else:
				print("Amstrad CPC Unittest SUCCESS!")
		os.chdir(orgPath)
	else:
		print("Skipping CPC tests: emulator path '%s'" % cap32)


import threading
import queue

def enqueue_output(out, queue):
	for line in iter(out.readline, b''):
		queue.put(line)
	out.close()

def GBUnitTests():
	os.chdir(orgPath)
	if sameboy and os.path.exists(sameboy):
		path =  os.path.abspath(lp+'GAMEBOY/UnitTests/')
		testrun = subprocess.Popen([sameboy,path+"/unittests.gb"], stdin=PIPE, stdout=PIPE, stderr=PIPE, bufsize=0)
		q = queue.Queue()
		t = threading.Thread(target=enqueue_output, args=(testrun.stdout, q))
		#t.daemon = True # thread dies with the program
		t.start()
		# Let some time for sameboy to setup and debugger to properly answer
		time.sleep(2)
		result = None
		begin = time.time()
		timeout = 10*60
		finished = False
		while not finished:
			time.sleep(1)
			testrun.send_signal(2)
			# Ask multiple times so that more output is generated, fighting buffering
			for i in range(10):
				testrun.stdin.write(b'examine $c800\n')
			testrun.stdin.write(b'continue\n')
			more_output = True
			while more_output:
				try:
					line = q.get_nowait().decode('utf-8')
					#print('<%s>' % line)
					if 'c800: fe ed c0 ff ee' in line:
						result = 'SUCCESS'
						finished = True
						break
					if 'c800: ba db ad c0 de' in line:
						result = 'FAILURE'
						failed.append([path, "unittests.ras"])
						finished = True
						break
				except queue.Empty:
					more_output = False
			if time.time() - begin > timeout:
				print("ERROR: Timeout for CPC unit tests expired.")
				result = 'TIMEOUT'
				failed.append([path, "unittests.ras"])
				break
		print(result)
		testrun.send_signal(9)
		t.join()


def UnitTests():
	DOSUnitTests();
	C64UnitTests();
	CPCUnitTests();
	GBUnitTests();


def CompileTests():
	for v in tests:
		directory = v[0]
		print("Project: "+directory)
		os.chdir(orgPath)

		os.chdir(lp+directory)
		for file in v[1]:
			# ignore auto-generated files
			if "auto_generated" not in file:
				if (c(directory,file)!=0):
					print("******* FAIL ERROR when trying to compile "+file+" in project "+directory)
					failed.append([directory, file])
		os.chdir(orgPath)



print("Welcome to the TRSE auto compiler validator!")
print("Compiling up a ton of tutorials...")
CompileTests()
print("Running tests...")
UnitTests()


if failed:
	for f in failed:
		print(" FAILED: %s" % f)
	exit(1)



print(" _______           _______  _______  _______  _______  _______ ")
print("(  ____ \|\     /|(  ____ \(  ____ \(  ____ \(  ____ \(  ____ \ ")
print("| (    \/| )   ( || (    \/| (    \/| (    \/| (    \/| (    \/")
print("| (_____ | |   | || |      | |      | (__    | (_____ | (_____ ")
print("(_____  )| |   | || |      | |      |  __)   (_____  )(_____  )")
print("      ) || |   | || |      | |      | (            ) |      ) |")
print("/\____) || (___) || (____/\| (____/\| (____/\/\____) |/\____) |")
print("\_______)(_______)(_______/(_______/(_______/\_______)\_______)")





