f = open("out.txt", "r")
g = open("nestest.txt", "r")

ln = 1

while True:
    line1 = f.read(4)
    line2 = g.read(4)
    if not line1 or not line2:
        break
    if line1 != line2:
        print("Difference found @", ln, ":")
        print("File 1:", line1.strip())
        print("File 2:", line2.strip())
    ln += 1
    f.readline()
    g.readline()