#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int scoreCheck(string word);

int main()
{
    string player1, player2;
    int score1, score2;

    player1 = get_string("Player 1: ");
    player2 = get_string("Player 2: ");

    score1 = scoreCheck(player1);
    score2 = scoreCheck(player2);

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int scoreCheck(string word)
{
    int score = 0;
    int lenght;
    char alphabet[27] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                         'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\0'};

    int scoreTable[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    lenght = strlen(word);

    for (int i = 0; i < lenght; i++)
    {
        word[i] = tolower(word[i]);
    }

    for (int i = 0; i < lenght; i++)
    {
        if (((int) word[i] > 96) && ((int) word[i] < 123))
        {
            for (int j = 0; j < 26; j++)
            {
                if (word[i] == alphabet[j])
                {
                    score = score + scoreTable[j];
                    break;
                }
            }
        }
    }

    return score;
}
