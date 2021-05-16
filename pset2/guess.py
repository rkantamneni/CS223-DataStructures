from random import *
tries = 0
ans = randint(1,10)

while tries <= 3:
    if tries == 3:
        print("Done")
        print("Answer is", ans)
        tries+=1

    guess = int(input("What's Your Guess?"))
    if guess == ans:
        print("You got it")
        print("The secret number was", ans)

    if guess > ans:
        print("Too big")
        tries+=1

    if guess < ans:
         print("Too small")
         tries+=1
    