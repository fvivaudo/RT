from math import *
from random import uniform
final = ""
def posStr(pos):
	return str(pos["x"])+","+str(pos["y"])+","+str(pos["z"])

def matStr(col):
	return str(col["red"])+","+str(col["green"])+","+str(col["blue"])+",0,0,0,0,0,0"

def makeSphere(poss, radius, color):
	global final
	global addy
	pos = dict(poss)
	pos["x"] += -600
	pos["y"] += -75
	pos["z"] += -500
	final += "object\n{\n\ttype(sphere);\n"
	final += "\tradius("+str(radius)+");\n"
	final += "\tposition("+posStr(pos)+");\n"
	final += "\tmaterial("+matStr(color)+");\n"
	final += "}\n"

def makeCam(pos, ori):
	global final
	final += "object\n{\n\ttype(camera);\n"
	final += "\tpos("+posStr(pos)+");\n"
	final += "\torientation("+posStr(ori)+");\n"
	final += "}\n"

def makeLight(pos):
	global final
	final += "object\n{\n\ttype(light);\n"
	final += "\tpos("+posStr(pos)+");\n"
	final += "\tintensity(1,1,1);\n"
	final += "}\n"

def stamp():
	global final
	with open("generated", "w") as f:
		f.write(final)

def rotate(poss, angle):
	pos = dict(poss)
	rangle = angle * 0.0174533
	tmp = pos['x'];
	pos['x'] = cos(rangle) * pos['x'] - sin(rangle) * pos['y']
	pos['y'] = sin(rangle) * tmp + cos(rangle) * pos['y']
	return pos

def randColor():
	return {"red" : uniform(0.0, 1.0), "green": uniform(0.0, 1.0), "blue" : uniform(0.0, 1.0)}

scolor = {"red" : 0.1, "green" : 1, "blue" : 0.1}
spos = {"x" : 0, "y" : 100, "z" : 0}
sposs = {"x" : 0, "y" : -100, "z" : 0}

poscam = {"x" : 0, "y" : 0, "z" : 1000}
oricam = {"x" : -0.5, "y" : -0.3, "z" : -1}

poslight = {"x" : -75, "y" : 10, "z" : 100}

makeCam(poscam, oricam)
makeLight(poslight);
makeSphere(spos, 15 ,scolor)

rot = 0
for i in range(120):
	makeSphere(rotate(spos, rot), 22 ,randColor())
	makeSphere(rotate(sposs, rot), 22 ,randColor())
	spos["z"] += -100
	sposs["z"] += -100
	rot += 20



stamp()

#CAMERA		0		0		1000		ORIENTATION -0.5 -0.3 -1

#SPHERE		-75		-75 	-500	ID 2 MATERIAL 1 0 0 0 0 0 0	 0 RADIUS 75 SLICE 0 0 0  1 0 0 SLICE 0 0 0  0 1 0

#SPHERE		-250		-65 	-300	ID 2 MATERIAL 1 0 0 0 0 0 0	 0 RADIUS 75
