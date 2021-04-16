from subprocess import Popen, PIPE
import subprocess
import os
import sys

#trse = "/opt/homebrew/bin/trse"
#trse = "/Users/leuat/code/TRSE/ReleaseM1/trse.app/Contents/MacOS/trse"

assemble = "yes"

if len(sys.argv) < 2:
	print("Usage: python validate_all.py [ trse exe file] ")
	print("must be run in the 'validate_all' directory.")
	exit(1)
trse = sys.argv[1]

if len(sys.argv)>=3:
	if (sys.argv[2] == "no_assembling"):
		assemble="no"



lp = "../tutorials/"


tests = [ ]



def fillRasList(idx,path):
	for file in os.listdir(lp+tests[idx][0]+"/"+path):
		if file.endswith(".ras"):
			tests[idx][1].append(path+"/"+file)



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

# CPC

tests.append([ "AMSTRADCPC464/Morketid",["main.ras"]])
tests.append([ "AMSTRADCPC464/tutorials",[]])
fillRasList(len(tests)-1,".")


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


#	process = Popen([trse, '-cli',  ], stdout=PIPE, stderr=PIPE)
	return  subprocess.run([trse,"-cli",'op=project','project='+projectFile,'input_file='+f1,'assemble='+assemble], stdout=PIPE, stderr=PIPE)

#	print(rVal)
#	stdout, stderr = process.communicate()
#	print stdout




orgPath = os.getcwd()
print(orgPath)
print("Welcome to the TRSE auto compiler validator!")
print("Compiling up a ton of tutorials...")
failed = []
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





