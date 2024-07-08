

def main():
    number = int(input("Number: "))

    if validate(number):
        print(identify(number))
    else:
        print("INVALID")


def validate(number):
    x = str(number)
    tot1 = 0
    tot2 = 0
    for i in range(len(x)):
        if i % 2 == 0:
            temp = int(x[i]) * 2
            temp1 = 0
            if temp > 9:
                temp1 = str(temp)[0]
                temp = str(temp)[1]
            tot1 += int(temp) + int(temp1)
        else:
            tot2 += int(x[i])
    total = tot1 + tot2
    if str(total)[-1] == "0":
        return True
    else:
        return False


def identify(number):
    number = str(number)
    if len(number) == 13 or len(number) == 16 and number[0] == '4':
        return "VISA"
    if len(number) == 16 and number[0] in ['51', '52', '53', '54', '55']:
        return "MASTERCARD"
    if len(number) == 15 and number[0] == '34' or number[0] == '37':
        return "AMEX"
    else:
        return "INVALID"


main()
