#!/usr/bin/python3
"""
Convert a dictionary of Morse Code sequences into a search tree
data structure for use in an Arduino sketch

$Revision: 1.2 $  
"""

#import pdb
#from pprint import pprint

mdict = {

		".-":'A', "-...":'B', "-.-.":'C', "-..":'D', ".":'E', "..-.":'F', "--.":'G', "....":'H',

		"..":'I', ".---":'J', "-.-":'K', ".-..":'L', "--":'M', "-.":'N', "---":'O', ".--.":'P', "--.-":'Q',

		".-.":'R', "...":'S', "-":'T', "..-":'U', "...-":'V', ".--":'W', "-..-":'X', "-.--":'Y', "--..":'Z',

		".-.-.-":'.', "--..--":',', "..--..":'?', "-.-.--":'!',

		".--.-.":'@', "---...":':', "-.-.-.":';', ".-...":'&', "-.--.":'(', "-.--.-":')', "-..-.":'/',
		".-.-.":'+', "-....-":'-', "..--.-":'_', "...-..-":'$', ".----.":"\\'",

		".----":'1', "..---":'2', "...--":'3', "....-":'4', ".....":'5',

		"-....":'6', "--...":'7', "---..":'8', "----.":'9', "-----":'0',
		
		"...---...":"<!SOS!>",
		"........":"<error>\\n",

		} 

mdkeys=sorted(mdict.keys())
mdkeys=sorted(mdkeys, key=len)
#print (mdkeys)

dit=2
dot=1
mtable = [ ["^", -1, -1 ] ] ;	# assuming we won't be keying "^", use it for the root node

for mkey in mdkeys :
	#print (mdict[mkey], " ", mkey)
	node = 0
	for c in list(mkey) :
		if mtable[node][0] == c :
			continue

		elif c == '.' :
			next = mtable[node][dit]
			leg = dit
		else :
			next = mtable[node][dot]
			leg = dot

		if next < 0 :
			mtable.append(["^", -1, -1])
			next = len(mtable) - 1
			mtable[node][leg] = next

		node = next 

	mtable[node][0] = mdict[mkey]

mtable.append(['^', -1, -1]) # node for invalid sequence
invalid_node = len(mtable) - 1 # will point all unassigned legs here
for node in range(0, len(mtable) ) :
	if mtable[node][dit] < 0 : mtable[node][dit] = invalid_node
	if mtable[node][dot] < 0 : mtable[node][dot] = invalid_node
	

# Emit code for arduino sketch

print ("// https://en.wikipedia.org/wiki/Dichotomic_search\n") ;
print ("struct mnode  \n{\n\tchar *mchar ;\n\tbyte  dah ;\n\tbyte  dit ;\n};\n")
print ("mnode mtree[] =\n{")

for nx in range(0,len(mtable)) :
	#print (nx, " ", mtable[nx])
	print ("\t{ \"%s\", %d, %d },\t// %d" % (mtable[nx][0], mtable[nx][1], mtable[nx][2], nx) )
print ("\n} ;")

# vim: number:
