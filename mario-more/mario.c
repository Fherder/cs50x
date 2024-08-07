#include <stdio.h>
#include <cs50.h>

int main()
{
    int height;
    do{
        height = get_int("Height: ");
    }while((height<1) || (height>8));

    int space=height-1;
    int counter=1;

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<space; j++)
        {
            printf(" ");
        }
        
        for(int k=0; k<counter*2; k++)
        {
            printf("#");
            if(k==counter-1)
            {
                printf("  ");
            }
        }
        counter++;
        space--;
        printf("\n");
    }
    return 0;
}
