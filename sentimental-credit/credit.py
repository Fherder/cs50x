import re

while True:
    cardNumber = int(input("Number: "))
    if cardNumber < 1:
        print("INVALID NUMBER")
    else:
        break

counter = 0
result = 0
copy = cardNumber // 10
mod = (copy % 10) * 2

if mod >= 10:
    extra = mod // 10 + (mod % 10)
    result = extra
else:
    result = mod

while copy > 0:
    copy = copy // 100
    mod = (copy % 10) * 2

    if mod >= 10:
        extra = mod // 10 + (mod % 10)
        result = result + extra
    else:
        result = result + mod

# Luhn’s Algorithm Second part

copy = cardNumber
result = result + (copy % 10)

while copy > 0:
    copy = copy // 100
    mod = copy % 10
    result = result + mod

# Length Check and first digits

copy = cardNumber
while copy > 0:
    if 9 < copy < 100:
        if (copy % 10) == 0:
            startNum = copy // 10
        else:
            startNum = copy

    copy = copy // 10
    counter += 1

# Patterns for card types
amex_pattern = re.compile(r'^3[47][0-9]{13}$')
mastercard_pattern = re.compile(r'^5[1-5][0-9]{14}$')
visa_pattern = re.compile(r'^4[0-9]{12}(?:[0-9]{3})?$')

# Check the card number against patterns and Luhn's algorithm
if result % 10 != 0:
    print("INVALID")
else:
    if amex_pattern.match(str(cardNumber)):
        print("AMEX")
    elif mastercard_pattern.match(str(cardNumber)):
        print("MASTERCARD")
    elif visa_pattern.match(str(cardNumber)):
        print("VISA")
    else:
        print("INVALID")
