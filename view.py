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
	return sys.argv[1]

def clearScreen():
	os.system('cls' if os.name == 'nt' else 'clear')

def printFooter():
	print '+PageUp  -PageDown  o-orderToggle  m-markAll  s-newStream  c-checkForNew  q-quit'

# May need to change the way the name is stored (change tokenize to delimiter rather than space)
def getListOfStreams(name):
	streamList = []

	for streamfile in glob.glob('messages/*StreamUsers'):
		with open(streamfile) as f:
			for line in f.readlines():
				if line.strip().split(' ', 1)[0] == name:
					stream = streamfile[9:].split('StreamUsers', 1)[0]
					streamList.append(stream)

	streamList.append('all')
	streamList.append('\n')
	return streamList


def getLastSeenPost(stream, name):
	filename = 'messages/%sStreamUsers' % stream

	with open(filename) as f:
		for line in f.readlines():
			if line.strip().split(' ', 1)[0] == name:
				return int(line.strip().split(' ', 1)[1])


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
	if len(streamList) == 2:
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

		while (lineCount < 22):
			if f.tell() in positions: 

				if f.tell() == positions[-1]:
					check = 1
				if check == 0:
					print '-'*50
				lineCount = lineCount + 1
				postRead = f.tell()

			print f.readline(),
			lineCount = lineCount + 1

		if check == 1:
			print '\nUser has read all posts, press the Up Arrow to view older posts'

		updateUser(stream, name, postRead)
		print '\n'


def keyPressed(stream, name):
	orig_settings = termios.tcgetattr(sys.stdin)

	tty.setraw(sys.stdin)
	x = 0

	while x != 'q': 
		x = sys.stdin.read(1)[0]
		clearScreen()
		
		if x == '+':
			print 'PAGE UP'
		elif x == '-':
			print 'PAGE DOWN'
		elif x == 'o':
			print 'ORDER TOGGLE'
		elif x == 'm':
			print 'MARK ALL'
		elif x == 's':
			termios.tcsetattr(sys.stdin, termios.TCSADRAIN, orig_settings)
			stream = selectStream(name)
			clearScreen()
			streamSelected(stream, name)
			printFooter()
			tty.setraw(sys.stdin)
		elif x == 'c':
			print 'CHECK FOR NEW'
		elif x == 'q':
			termios.tcsetattr(sys.stdin, termios.TCSADRAIN, orig_settings) 
			sys.exit()

	termios.tcsetattr(sys.stdin, termios.TCSADRAIN, orig_settings)  


def updateUser(stream, name, newRead):
	filename = 'messages/%sStreamUsers' % stream

	with open(filename) as f:
		contents = f.readlines()

	with open(filename, 'w') as f:
		for line in contents:
			if line.strip().split(' ', 1)[0] == name:
				f.write('%s %d\n' % (name, newRead))
			else:
				f.write(line)
			

if __name__ == "__main__":
	name = getName()
	stream = selectStream(name)

	clearScreen()
	streamSelected(stream, name)
	printFooter()

	keyPressed(stream, name)



