#! /usr/bin/python

fp = open("power.log","r")
lines = fp.readlines()
start1 = 2
start2 = 3
step = 4
code_list = []
for index in range(len(lines)):
	print(index)

	if index == start1  or index == start2:
		line = lines[index]
		print(line)

		line_list = list(line)

		pos = line.find("NB_send")
		if pos >= 0:
			for i in range(4):
				line_list.insert(pos + i, " ")
		pos1 = line.find("NB_receive")
		if pos1 >= 0:
			for i in range(2):
				line_list.insert(pos1 + i, " ")


		pos2 = line.find("NB_shutdown")
		if pos2 >= 0:
			for i in range(3):
				line_list.insert(pos2+i, " ")

		new_line = ''.join(line_list)
		print(new_line)
		if index == start1:
			start1 = start1 + step
		else:
			start2 = start2 + step
		code_list.append(new_line)
	else:
		code_list.append(lines[index])

fp.close()


f = open("power.log","w")
f.writelines(code_list)
f.close()

