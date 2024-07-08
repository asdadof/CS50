
def main():
    text = input("Input: ")
    grade = grading(L(text), S(text))

    if grade <= 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(grade)}")


def grading(L, S):
    return (0.0588 * L) - (0.296 * S) - 15.8


def L(text):
    words = len(text.split())
    letters = sum(char.isalpha() for char in text)
    return (letters / words) * 100


def S(text):
    words = len(text.split())
    sentences = 0
    for i in text:
        if i in ['.', '!', '?']:
            sentences += 1
    return (sentences/words)*100


main()
