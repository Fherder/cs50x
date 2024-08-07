#include <stdio.h>
#include <cs50.h>

int main()
{
    long result, extra, mod, startNum, copy, cardNumber=0;
    bool flag = true;
    int counter=0;

    do
    {
        cardNumber = get_long("Number: ");
        if(cardNumber < 1)
        {
            printf("INVALID NUMBER");
        }
        else
        {
            flag=false;
        }
    }while(flag);

    // Luhn’s Algorithm First part
    copy = cardNumber/10;
    mod = (copy % 10) * 2;

    if(mod >= 10){
        extra = mod / 10;
        extra = extra + (mod % 10);
        result = extra;
    }
    else
    {
        result = mod;
    }

    while(copy > 0)
    {
        copy = copy/100;

        mod = ((copy % 10) * 2);

        if(mod >= 10)
        {
            extra = mod / 10;
            extra = extra + (mod % 10);
            result = result + extra;
        }
        else
        {
            result = result + mod;
        }
    }

    // Luhn’s Algorithm Second part

    copy = cardNumber;
    result = result + (copy % 10);

    while(copy > 0)
    {
        copy = copy/100;
        mod = copy % 10;
        result = result + mod;
    }

    // Lenght Check and first digits

    copy = cardNumber;
    while(copy>0)
    {
        if(copy > 9 && copy < 100)
        {
            if( (copy % 10) == 0)
            {
                startNum = copy/10;
            }
            else
            {
                startNum = copy;
            }
        }

        copy = copy/10;
        counter++;
    }

    // Validity check and card type

    if((result % 10) != 0)
    {
        printf("INVALID\n");
    }
    else{
        if(counter == 15 && (startNum == 34 || startNum == 37))
        {
            printf("AMEX\n");
        }
        else if(counter == 16 && (startNum == 51 || startNum ==52 || startNum == 53 || startNum == 54 || startNum == 55))
        {
            printf("MASTERCARD\n");
        }
        else if((counter == 13 || counter==16) && (startNum == 4 || startNum/10 == 4))
        {
            printf("VISA\n");
        }
        else{
            printf("INVALID\n");
        }
    }

    return 0;
}
