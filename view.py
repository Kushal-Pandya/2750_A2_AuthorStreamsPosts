#!/usr/bin/python

import sys
import os
import glob

import tty
import sys
import termios



def getName():
	if len(sys.argv) < 2:
		sys.exit()
	elif len(sys.argv) > 2:
		name = ""
		for word in sys.argv:
			if word != sys.argv[0]:
				if name == "":
					name = name + word
				else:
					name = name + " " + word
		return name
	else:
		return sys.argv[1]

def clearScreen():
	os.system('cls' if os.name == 'nt' else 'clear')

def printFooter():
	print '-PageUp  +PageDown  o-orderToggle  m-markAll  s-newStream  c-checkForNew  q-quit'

# May need to change the way the name is stored (change tokenize to delimiter rather than space)
def getListOfStreams(name):
	streamList = []

	for streamfile in glob.glob('messages/*StreamUsers'):
		with open(streamfile) as f:
			for line in f.readlines():
				if line.strip().split(',', 1)[0] == name:
					stream = streamfile[9:].split('StreamUsers', 1)[0]
					streamList.append(stream)

	streamList.append('\n')
	return streamList


def getLastSeenPost(stream, name):
	filename = 'messages/%sStreamUsers' % stream

	with open(filename) as f:
		for line in f.readlines():
			if line.strip().split(',', 1)[0] == name:
				return int(line.strip().split(',', 1)[1])


# See if this function is useful
def getPostPositions(stream):
	filename = 'messages/%sStreamData' % stream
	positions = []

	if os.path.isfile(filename) == False:
		print '%s does not exist.. exiting' % filename
		sys.exit()

	with open(filename) as f:
		for line in f.readlines():
			positions.append(int(line.strip()))

	return positions


def selectStream(name):
	# getting and printing list of streams
	streamList = getListOfStreams(name)
	if len(streamList) == 1:
		print '%s does not exist in any streams.. exiting' % name
		sys.exit()

	for stream in streamList:
		print stream,

	stream = raw_input()
	if stream not in streamList:
		print 'Invalid stream'
		sys.exit()

	return stream


def streamSelected(stream, name):
	filename = 'messages/%sStream' % stream
	lastSeenIndex = getLastSeenPost(stream, name)
	lineCount = 0
	postRead = 0
	check = 0

	if os.path.isfile(filename) == False:
		print '%s does not exist.. exiting' % filename
		sys.exit()

	with open(filename) as f:
		f.seek(lastSeenIndex, 0)

		positions = getPostPositions(stream)

		while (lineCount < 21):
			if f.tell() in positions: 

				if f.tell() == positions[-1]:
					check = 1
				if check == 0:
					print '-'*50
				lineCount = lineCount + 1
				postRead = f.tell()

			print f.readline(),
			lineCount = lineCount + 1

		updateUser(stream, name, postRead)
		print '\n'
		lineCount = lineCount + 1

	if check == 1:
		print 'All posts in current stream are read, press - for previous posts\n'

	return postRead	


def movePageDown(stream, name, postRead):
	filename = 'messages/%sStream' % stream
	lineCount = 0
	check = 0

	positions = getPostPositions(stream)

	if postRead == positions[-1]:
		return postRead

	with open(filename) as f:
		f.seek(postRead, 0)

		while (lineCount < 21):
			if f.tell() in positions: 

				if f.tell() == positions[-1]:
					check = 1
				if check == 0:
					print '-'*50
				lineCount = lineCount + 1
				postRead = f.tell()

			print f.readline(),
			lineCount = lineCount + 1
		print '\n'
		lineCount = lineCount + 1

		if postRead > getLastSeenPost(stream, name):
			updateUser(stream, name, postRead)

	return postRead	


def movePageDownToggle(filePointer):
	filename = 'messages/temp.txt'
	lineCount = 0

	with open(filename) as f:

		f.seek(filePointer, 0)
		while lineCount < 22:
			print f.readline(),
			lineCount = lineCount + 1
		toReturn = f.tell()

	offset = toReturn - filePointer
	return toReturn, offset


def movePageUp(stream, name, postRead):
	filename = 'messages/%sStream' % stream
	lineCount = 0
	seekTo = 0
	check = 0

	positions = getPostPositions(stream)

	if postRead == positions[0]:
		seekTo = 0
	else:
	 	for pos in positions:
			if postRead == pos:
				seekTo = positions[positions.index(pos)-1]

	with open(filename) as f:
		f.seek(seekTo, 0)

		while (lineCount < 21):
			if f.tell() in positions: 

				if f.tell() == positions[-1]:
					check = 1
				if check == 0:
					print '-'*50
				lineCount = lineCount + 1
				postRead = f.tell()

			print f.readline(),
			lineCount = lineCount + 1
		print '\n'
		lineCount = lineCount + 1

	if seekTo == 0:
		return positions[0]
	return seekTo


def movePageUpToggle(filePointer, offsetCheck):
	filename = 'messages/temp.txt'
	lineCount = 0

	if offsetCheck:
		filePointer = 0

	with open(filename) as f:

		f.seek(filePointer, 0)
		while lineCount < 22:
			print f.readline(),
			lineCount = lineCount + 1
		toReturn = f.tell()

	return toReturn


def getSortedNames(stream, positions):
	filename = 'messages/%sStream' % stream
	sortedNames = []

	with open(filename) as f:

		for pos in positions:
			f.seek(pos, 0)
			name = f.readline().split(": ", 1)[1]
			sortedNames.append(name.strip())

	return sorted(sortedNames)


def getSortedNamesDict(stream, positions):
	filename = 'messages/%sStream' % stream
	sortedNames = {}

	with open(filename) as f:

		for pos in positions:
			f.seek(pos, 0)
			name = f.readline().split(": ", 1)[1]
			name = name.strip()
			if name in sortedNames:
				poslist = sortedNames.get(name)
				poslist.append(pos)
				sortedNames[name] = poslist
			else:
				sortedNames[name] = [pos]

	return sortedNames


def writeByNames(namesList, namesDict):
	filename = 'messages/%sStream' % stream
	sortedNames = {}
	otherPositions = getPostPositions(stream)
	namesCompleteted = []

	tempFile = open('messages/temp.txt', 'w')

	with open(filename) as f:
		for name in namesList:
			if name not in namesCompleteted:
				positions = namesDict.get(name)
				if type(positions) is list:
					for pos in positions:
						f.seek(pos, 0)
						tempFile.write(f.readline())
						while f.tell() not in otherPositions:
							tempFile.write(f.readline())
						tempFile.write('-'*50)
						tempFile.write('\n')
				else:
					f.seek(pos, 0)
					tempFile.write(f.readline())
					while f.tell() not in otherPositions:
						tempFile.write(f.readline())
					tempFile.write('-'*50)
					tempFile.write('\n')
				namesCompleteted.append(name)	

	tempFile.close()


def displayStreamByName(filePointer):
	filename = 'messages/temp.txt'
	lineCount = 0

	with open(filename) as f:

		f.seek(filePointer, 0)
		while lineCount < 22:
			print f.readline(),
			lineCount = lineCount + 1
		toReturn = f.tell()

	return toReturn



def keyPressed(stream, name, postRead):
	origSettings = termios.tcgetattr(sys.stdin)
	orderToggle = 0
	orderToggleFP = 0
	toggleOffset = 0
	offsetCheck = 0

	tty.setraw(sys.stdin)
	x = 0

	while x != 'q': 
		x = sys.stdin.read(1)[0]
		
		if x == '-':
			clearScreen()
			termios.tcsetattr(sys.stdin, termios.TCSADRAIN, origSettings)

			if orderToggle:
				orderToggleFP = movePageUpToggle(toggleOffset, offsetCheck)
				offsetCheck = 1
			else:
				postRead = movePageUp(stream, name, postRead)
			printFooter()
			tty.setraw(sys.stdin)

		elif x == '+':
			clearScreen()
			termios.tcsetattr(sys.stdin, termios.TCSADRAIN, origSettings)

			if orderToggle:
				orderToggleFP, toggleOffset = movePageDownToggle(orderToggleFP)
			else:
				postRead = movePageDown(stream, name, postRead)
			printFooter()
			tty.setraw(sys.stdin)

		elif x == 'o':
			clearScreen()
			termios.tcsetattr(sys.stdin, termios.TCSADRAIN, origSettings)

			if orderToggle == 0:
				positions = [0]
				positions.extend(getPostPositions(stream))
				del positions[-1]

				writeByNames(getSortedNames(stream, positions), getSortedNamesDict(stream, positions))
				orderToggleFP = 0
				toggleOffset = 0
				orderToggleFP = displayStreamByName(orderToggleFP)
				orderToggle = 1
			else:
				postRead = streamSelected(stream, name)
				orderToggle = 0

			printFooter()
			tty.setraw(sys.stdin)

		elif x == 'm':
			updateUser(stream, name, getPostPositions(stream)[-1])
		
		elif x == 'c':
			clearScreen()
			termios.tcsetattr(sys.stdin, termios.TCSADRAIN, origSettings)
			newPositions = getPostPositions(stream)

			if len(newPositions) > 1:
				postRead = movePageDown(stream, name, newPositions[-2])
			else:
				postRead = movePageDown(stream, name, newPositions[-1])
			printFooter()
			tty.setraw(sys.stdin)

		elif x == 's':
			clearScreen()
			termios.tcsetattr(sys.stdin, termios.TCSADRAIN, origSettings)
			print 'Select New Stream:'
			stream = selectStream(name)
			clearScreen()
			postRead = streamSelected(stream, name)
			printFooter()
			tty.setraw(sys.stdin)

		elif x == 'q':
			termios.tcsetattr(sys.stdin, termios.TCSADRAIN, origSettings) 
			sys.exit()

	termios.tcsetattr(sys.stdin, termios.TCSADRAIN, origSettings)  


def updateUser(stream, name, newRead):
	filename = 'messages/%sStreamUsers' % stream

	with open(filename) as f:
		contents = f.readlines()

	with open(filename, 'w') as f:
		for line in contents:
			if line.strip().split(',', 1)[0] == name:
				f.write('%s, %d\n' % (name, newRead))
			else:
				f.write(line)
			

if __name__ == "__main__":
	name = getName()
	stream = selectStream(name)

	clearScreen()
	postRead = streamSelected(stream, name)
	printFooter()

	keyPressed(stream, name, postRead)



