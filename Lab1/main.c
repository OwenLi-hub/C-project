#include <stdio.h>

int array_average(const int *array, size_t count) {
    // TODO: return the average of the 'count' elements of 'array'.
    int sum=0;

    //Creat an loop to add every number in the array and divide for average
    for(int i=0; i<count; i++){
        sum+=array[i];
    }
    return sum/count;
}

//function will find the max in array
int max(const int *array, size_t count ){

    //Creat an loop to go through the array
        for (int n=0; n<count; n++) {
            //Creat an if else starment that verified the max number in the array
            if (array[n] < array[n + 1]) {
                continue;
            }
            else return array[n];
        }
  }

int main() {
    // TODO: construct an array, call 'array_average' on it, and print the result.
    int array[7] ={1,3,5,8,15,11,7};
    //Get the size of the array
    int count = sizeof(array)/sizeof(array[0]);

    //Recall the function to give us the Average and max value
    int aV = array_average(array, count);
    int mV = max(array, count);
    printf("The average array value is %d\n", aV);
    printf("The max array value is %d\n", mV);

    return 0;
}
