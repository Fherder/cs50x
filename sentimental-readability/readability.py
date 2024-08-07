def main():

    text = str(input("Text: "))

    letters = letterCounter(text)
    words = wordCounter(text)
    sentences = sentenceCounter(text)

    L = (letters / words) * 100
    S = (sentences / words) * 100

    index = 0.0588 * L - 0.296 * S - 15.8

    if index >= 16:
        print("Grade 16+\n")

    elif index < 1:
        print("Before Grade 1\n")

    else:
        print(f"Grade {round(index)}")


def letterCounter(text):

    counter = 0.0
    lenght = len(text)

    for i in range(lenght):
        # check whether a character is alphabetical
        if text[i].isalpha():
            counter += 1

    return counter


def wordCounter(text):

    counter = 0.0

    lenght = len(text)

    for i in range(lenght - 1):

        # check whether a character is whitespace (e.g., a newline, space, or tab) and checks the existence of a character before
        # it
        if text[i + 1].isspace() and (text[i].isalnum() or ord(text[i]) > 20):

            counter += 1

    counter += 1

    return counter


def sentenceCounter(text):
    length = len(text)
    counter = 0.0

    for i in range(length):
        #  check whether a character a period, exclamation point or question mark
        if text[i] == '.' or text[i] == '!' or text[i] == '?':
            counter += 1

    return counter


main()
