# Mastermind
Guesses a randomly generated sequence of numbers for up to length 15 and value 15.

The AI utilises four different algorithms of varying efficiency to account for the range of values, thus there is a tradeoff between speed and number of guesses.

1) Knuth's five-move algorithm

2) MaxParts algorithm

3) Simple consistency algorithm

4) Brute force algorithm

Rules of the Game
-----------------
The codemaker conceals a code, and the codebreaker tries to identify all symbols correctly by trying appropriate test patterns. If the guess is wrong, the codemaker tells the codebreaker how close he is by revealing:

1) The number of "black hits" - the number of matching values and positions, and

2) The number of "white hits" - the number of matching values even if their positions do not match

This repeats until the codebreaker gets all black hits (i.e. the correct sequence)
