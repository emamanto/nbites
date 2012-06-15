import sys
import math

def stdev(mylist):
    avg = sum(mylist)/len(mylist)
    squares = []
    for item in mylist:
        squares.append(pow(item-avg, 2))
    return math.sqrt(sum(squares)/len(squares))

if len(sys.argv) != 2:
    sys.exit("Takes 1 argument!")

f = open(sys.argv[1], "r")

numFrames = 0

leftPostDist = []
leftPostBear = []

rightPostDist = []
rightPostBear = []

leftCornerDist = []
leftCornerBear = []

rightCornerDist = []
rightCornerBear = []

for line in f:
    words = line.split()

    if len(words) < 1:
        numFrames += 1.0
        continue

    if words[0] == 'LEFT':
        if words[1] == 'POST':
            leftPostDist.append(float(words[2]))
            leftPostBear.append(float(words[3]))
        if words[1] == 'CORNER':
            leftCornerDist.append(float(words[2]))
            leftCornerBear.append(float(words[3]))

    if words[0] == 'RIGHT':
        if words[1] == 'POST':
            rightPostDist.append(float(words[2]))
            rightPostBear.append(float(words[3]))
        if words[1] == 'CORNER':
            sumRightCornerDist.append(float(words[2]))
            sumRightCornerBear.append(float(words[3]))

print "========== DATA ANALYZED ========="
print ""
print "Number of Frames: " + str(numFrames)
print ""
print "Number of Left Posts: " + str(len(leftPostDist)
print "             Percent: " + str((len(leftPostDist) / numFrames)*100.0)
if numLeftPosts != 0:
    print "        Avg Distance: " + str(sum(leftPostDist) /
                                         len(leftPostDist))
    print "               STDEV: " + str(stdev(leftPostDist))
    print "         Avg Bearing: " + str(sum(LeftPostBear) /
                                         len(leftPostBear))
    print "               STDEV: " + str(stdev(leftPostBear))
print "Number of Right Posts: " + str(len(rightPostDist)
print "             Percent: " + str((len(rightPostDist) / numFrames)*100.0)
if numRightPosts != 0:
    print "        Avg Distance: " + str(sum(rightPostDist) /
                                         len(rightPostDist))
    print "               STDEV: " + str(stdev(rightPostDist))
    print "         Avg Bearing: " + str(sum(RightPostBear) /
                                         len(rightPostBear))
    print "               STDEV: " + str(stdev(rightPostBear))
print ""
print "Number of Left Corners: " + str(len(leftCornerDist)
print "             Percent: " + str((len(leftCornerDist) / numFrames)*100.0)
if numLeftCorners != 0:
    print "        Avg Distance: " + str(sum(leftCornerDist) /
                                         len(leftCornerDist))
    print "               STDEV: " + str(stdev(leftCornerDist))
    print "         Avg Bearing: " + str(sum(LeftCornerBear) /
                                         len(leftCornerBear))
    print "               STDEV: " + str(stdev(leftCornerBear))
print "Number of Right Corners: " + str(len(rightCornerDist)
print "             Percent: " + str((len(rightCornerDist) / numFrames)*100.0)
if numRightCorners != 0:
    print "        Avg Distance: " + str(sum(rightCornerDist) /
                                         len(rightCornerDist))
    print "               STDEV: " + str(stdev(rightCornerDist))
    print "         Avg Bearing: " + str(sum(RightCornerBear) /
                                         len(rightCornerBear))
    print "               STDEV: " + str(stdev(rightCornerBear))
