#! /usr/bin/python2

import sys

inputs = sys.argv

input_file = open(inputs[1],"r")
output = open(inputs[2],"w")

vertices = []
triangles = []
line_num = 0;

for line in input_file:
    line_num+=1;
    print line_num
    if line[0] == 'v':
        vertices.append(line[2:-1])
    if line[0] == 'f' and not "/" in line:
        coords = line[2:-1].split(" ")
        triangles.append(vertices[int(coords[0])-1]+' '+vertices[int(coords[1])-1]+' '+vertices[int(coords[2])-1]+' 255 0 0')
print >> output, len(triangles)
for i in range(len(triangles)):
    print >> output, triangles[i]
