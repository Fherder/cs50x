#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

float letterCounter(string text);
float wordCounter(string text);
float sentenceCounter(string text);

int main()
{
    string text = get_string("Text: ");
    float letters, words, sentences, L, S;
    float index;

    letters = letterCounter(text);
    words = wordCounter(text);
    sentences = sentenceCounter(text);

    L = (letters / words) * 100;
    S = (sentences / words) * 100;

    index = 0.0588 * L - 0.296 * S - 15.8;

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %.0f\n", index);
    }
}

float letterCounter(string text)
{
    int lenght;
    float counter = 0.0;

    lenght = strlen(text);

    for (int i = 0; i < lenght; i++)
    {
        // check whether a character is alphabetical
        if (isalpha(text[i]))
        {
            counter++;
        }
    }

    return counter;
}

float wordCounter(string text)
{
    int lenght;
    float counter = 0.0;

    lenght = strlen(text);

    for (int i = 0; i < lenght; i++)
    {
        // check whether a character is whitespace (e.g., a newline, space, or tab) and checks the existence of a character before
        // it
        if (isspace(text[i + 1]) && (isalnum(text[i]) || ((int) text[i] > 20)))
        {
            counter++;
        }
    }

    counter++;

    return counter;
}

float sentenceCounter(string text)
{
    int lenght;
    float counter = 0.0;

    lenght = strlen(text);

    for (int i = 0; i < lenght; i++)
    {
        // check whether a character a period, exclamation point or question mark
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            counter++;
        }
    }

    return counter;
}
