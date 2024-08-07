#include <cs50.h>
#include <ctype.h> // i need to use isalpha() function
#include <stdio.h>
#include <string.h>

int numCheck(int lenght, char key[lenght]);
int alphaCheck(int lenght, char key[lenght]);
string cipher(string text, int lenght, char key[lenght]);

int main(int argc, string argv[])
{
    int lenght;
    string plaintext, ciphertext;

    if (argc == 2)
    {
        lenght = strlen(argv[1]) + 1;

        char key[lenght], copy[lenght];

        strcpy(key, argv[1]);  // the original key
        strcpy(copy, argv[1]); // the key copy that will be lowercase

        for (int i = 0; i < lenght - 1; i++)
        {
            copy[i] = tolower(copy[i]);
        }

        if (numCheck(lenght, key))
        {
            printf("Key must only contain alphabetic characters\n");
            return 1;
        }
        else if (lenght != 27)
        {
            printf("Key must contain 26 characters\n");
            return 1;
        }
        else if (alphaCheck(lenght, copy))
        {
            printf("Key must not contain repeated characters.\n");
            return 1;
        }
        else
        {
            plaintext = get_string("plaintext: ");
            ciphertext = cipher(plaintext, lenght, key);

            printf("ciphertext: %s\n", ciphertext);
        }
    }
    else
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }

    return 0;
}

string cipher(string text, int lenght, char key[lenght])
{
    int text_len = strlen(text);
    char og_char;

    for (int i = 0; i < text_len; i++)
    {
        og_char = text[i];

        if (text[i] == 'a' || text[i] == 'A')
        {
            text[i] = key[0];
        }
        else if (text[i] == 'b' || text[i] == 'B')
        {
            text[i] = key[1];
        }
        else if (text[i] == 'c' || text[i] == 'C')
        {
            text[i] = key[2];
        }
        else if (text[i] == 'd' || text[i] == 'D')
        {
            text[i] = key[3];
        }
        else if (text[i] == 'e' || text[i] == 'E')
        {
            text[i] = key[4];
        }
        else if (text[i] == 'f' || text[i] == 'F')
        {
            text[i] = key[5];
        }
        else if (text[i] == 'g' || text[i] == 'G')
        {
            text[i] = key[6];
        }
        else if (text[i] == 'h' || text[i] == 'H')
        {
            text[i] = key[7];
        }
        else if (text[i] == 'i' || text[i] == 'I')
        {
            text[i] = key[8];
        }
        else if (text[i] == 'j' || text[i] == 'J')
        {
            text[i] = key[9];
        }
        else if (text[i] == 'k' || text[i] == 'K')
        {
            text[i] = key[10];
        }
        else if (text[i] == 'l' || text[i] == 'L')
        {
            text[i] = key[11];
        }
        else if (text[i] == 'm' || text[i] == 'M')
        {
            text[i] = key[12];
        }
        else if (text[i] == 'n' || text[i] == 'N')
        {
            text[i] = key[13];
        }
        else if (text[i] == 'o' || text[i] == 'O')
        {
            text[i] = key[14];
        }
        else if (text[i] == 'p' || text[i] == 'P')
        {
            text[i] = key[15];
        }
        else if (text[i] == 'q' || text[i] == 'Q')
        {
            text[i] = key[16];
        }
        else if (text[i] == 'r' || text[i] == 'R')
        {
            text[i] = key[17];
        }
        else if (text[i] == 's' || text[i] == 'S')
        {
            text[i] = key[18];
        }
        else if (text[i] == 't' || text[i] == 'T')
        {
            text[i] = key[19];
        }
        else if (text[i] == 'u' || text[i] == 'U')
        {
            text[i] = key[20];
        }
        else if (text[i] == 'v' || text[i] == 'V')
        {
            text[i] = key[21];
        }
        else if (text[i] == 'w' || text[i] == 'W')
        {
            text[i] = key[22];
        }
        else if (text[i] == 'x' || text[i] == 'X')
        {
            text[i] = key[23];
        }
        else if (text[i] == 'y' || text[i] == 'Y')
        {
            text[i] = key[24];
        }
        else if (text[i] == 'z' || text[i] == 'Z')
        {
            text[i] = key[25];
        }

        if (!islower(og_char))
        {
            text[i] = toupper(text[i]);
        }
        else
        {
            text[i] = tolower(text[i]);
        }
    }
    return text;
}

int numCheck(int lenght, char key[lenght])
{

    for (int i = 0; i < lenght - 1; i++)
    {

        if (!isalpha(key[i]))
        {
            return 1;
        }
    }

    return 0;
}

int alphaCheck(int lenght, char key[lenght])
{
    char alphabet[27] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                         'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\0'};
    int check[26] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    for (int i = 0; i < lenght - 1; i++)
    {
        for (int j = 0; j < lenght - 1; j++)
        {
            if (key[i] == alphabet[j])
            {
                if (check[j] == 0)
                {
                    return 1;
                }
                else
                {
                    check[j] = 0;
                    j = lenght;
                }
            }
        }
    }

    return 0;
}
