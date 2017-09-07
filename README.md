A2 for CIS2750
==================
- Part of Blog Project
- Added ability to add/remove authors, post to streams and CLI Python posts Viewer


COMPILE INSTRUCTIONS
----------------------------
- make
- 'make clean' to remove compiled components


ADDAUTHOR PROGRAM
----------------------------
- ./addauthor <name>
- name can be multiple words
- streams have to be comma delimited NOT SPACE ex: List Streams: cats,dogs,others


POST PROGRAM
----------------------------
- ./post <name>
- Followed by entering stream name and text 
- Can only post to stream where author has permission


VIEW PROGRAM
----------------------------
- ./view.py <name>
- select stream of posts you want to view
- all functionalities work :)
- NOTE: To move up and down on a stream, MUST USE +/- keys instead of PgUp/PgDown


WHAT DOESNT WORK:
----------------------------
- view.py program:
	- Viewer is unable to view all streams that user has permission to
