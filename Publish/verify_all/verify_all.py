from subprocess import Popen, PIPE
import subprocess
import os

#trse = "/opt/homebrew/bin/trse"
trse = "/Users/leuat/code/TRSE/ReleaseM1/trse.app/Contents/MacOS/trse"
lp = "../tutorials/"


tests = [ ]



def fillRasList(idx,path):
	for file in os.listdir(lp+tests[idx][0]+"/"+path):
		if file.endswith(".ras"):
			tests[idx][1].append(path+"/"+file)

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
	return  subprocess.call([trse,"-cli",'op=project','project='+projectFile,'input_file='+f1])

#	print(rVal)
#	stdout, stderr = process.communicate()
#	print stdout




orgPath = os.getcwd()
print(orgPath)
print("Welcome to the TRSE auto compiler validator!")
print("Compiling up a ton of tutorials...")
for v in tests:
	directory = v[0]
	print("Project: "+directory)
	os.chdir(orgPath)

	os.chdir(lp+directory)
	for file in v[1]:
		if (c(directory,file)==1):
			print("******* FAIL ERROR when trying to compile "+file+" in project "+directory)
			exit(1)




print(" _______           _______  _______  _______  _______  _______ ")
print("(  ____ \|\     /|(  ____ \(  ____ \(  ____ \(  ____ \(  ____ \ ")
print("| (    \/| )   ( || (    \/| (    \/| (    \/| (    \/| (    \/")
print("| (_____ | |   | || |      | |      | (__    | (_____ | (_____ ")
print("(_____  )| |   | || |      | |      |  __)   (_____  )(_____  )")
print("      ) || |   | || |      | |      | (            ) |      ) |")
print("/\____) || (___) || (____/\| (____/\| (____/\/\____) |/\____) |")
print("\_______)(_______)(_______/(_______/(_______/\_______)\_______)")
                                                               




