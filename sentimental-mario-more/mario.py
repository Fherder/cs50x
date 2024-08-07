while True:
    height = input("Height: ")

    if height.isdigit():
        height = int(height)

        if 1 <= height <= 8:
            break


space = height - 1
counter = 1

for i in range(0, height):

    for j in range(0, space):
        print(" ", end="")

    for k in range(0, (counter * 2)):
        print("#", end="")

        if k == (counter - 1):
            print("  ", end="")

    counter = counter + 1
    space = space - 1
    print()
