#include "string.h"
#include <stdio.h>

int main()
{
    char buffer[256];

    FILE *todoFile = fopen("todo.txt", "r");

    if(todoFile == NULL)
    {
        printf("File is NULL");
    }

    while(fgets(buffer, sizeof(buffer), todoFile) != NULL)
    {
        printf(buffer);
    }

    fclose(todoFile);
}


// int main()
// {
//     size_t nums[5] = { 1, 54, 32, 12, -5};

//     int arrLength = sizeof(nums) / sizeof(nums[0]);


//     for(int i = 0; i < arrLength; i++)
//     {
//         printf("%i address is %p\n", nums[i], &nums[i]);
//     }

//     return 0;
// }