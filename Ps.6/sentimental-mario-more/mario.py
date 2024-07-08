while True:
    try:
        height = int(input("Height: "))
    except ValueError:
        print("Must be a positive integer between 1 and 8")
    else:
        if 1 <= height <= 8:
            break

for i in range(height):
    spaces = ' ' * (height - i - 1)
    left_hashes = '#' * (i + 1)
    right_hashes = '#' * (i + 1)
    middle_spaces = ' ' * (i - 1)

    print(spaces + left_hashes + '  ' + right_hashes + '')
