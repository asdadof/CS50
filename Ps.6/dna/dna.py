import csv
import sys


def main():

    if len(sys.argv) != 3:
        print("Usage: python dna.py database sequence")

    database = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for row in reader:
            database.append(row)

    file = open(sys.argv[2], 'r')
    sequence = file.read()
    file.close()

    subsequences = list(database[0].keys())[1:]

    base = database[0]

    list(base)[1:]

    results = []

    for i in range(len(base)-1):
        subsequence = list(base)[i+1]
        results.append(longest_match(sequence, subsequence))
    counter = 1
    for person in database:
        temp = [int(num) for num in list(person.values())[1:]]
        if temp == results:
            name = person["name"]
            return name
        counter += 1
    return "No match"


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


print(main())
