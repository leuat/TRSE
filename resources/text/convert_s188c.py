file1 = open('opcodes_s1c88_org.txt', 'r')
Lines = file1.readlines()
 

def wash(p):
	return p.replace("*08","*").replace("*16","**").replace("[","(").replace("]",")")

count = 0
out = []
# Strips the newline character
for line in Lines:
	line = line.strip().replace("\n","")
	line = " ".join(line.split())
	if (line==""):
		continue
	if (line.startswith("#")):
		continue
	# not supported yet
	if (":" in line):
		continue

	lst = line.split(";")
#    line = line.strip()

	first = lst[0].split(" ")

	print(first)
	o = first[0];
	if len(first)>=2:
		o += " "+wash(first[1])
	if len(first)>=3:
		if first[2].strip()!="":
			o += ","+wash(first[2])

 	second=lst[1].split('//')
#	s1 = second
 	s1 = second[0].split("=")[0]
 	s1 = s1.replace("$","0x")
 	o+="->"+s1
 	o+="\n"
	print(o)
	out.append(o)




file2 = open('opcodes_s1c88.txt', 'w')
file2.writelines(out)
file2.close()
 


