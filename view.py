#!/usr/bin/python

import sys
import os
import glob
import fileinput


def getName():
	if len(sys.argv) < 2:
		sys.exit()
	return sys.argv[1]

def clearScreen():
	os.system('cls' if os.name == 'nt' else 'clear')

def printFooter():
	print 'Up Arrow   Down Arrow   O-order toggle   M-mark all   S-stream   C-check for new'

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
				return line.strip().split(' ', 1)[1]


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


def streamSelected(stream, name):
	filename = 'messages/%sStream' % stream
	lastSeenIndex = int(getLastSeenPost(stream, name))

	if os.path.isfile(filename) == False:
		print '%s does not exist.. exiting' % filename
		sys.exit()

	with open(filename) as f:
		f.seek(lastSeenIndex, 0)

		positions = getPostPositions(stream)
		del positions[-1]
		for i in range(23):
			if f.tell() in positions:
				print '-'*50
			print f.readline(),
		updateUser(stream, name, f.tell())
		print '\n'


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

	clearScreen()
	streamSelected(stream, name)

	# get user input here


	printFooter()

