#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array {
  int capacity;  // How many elements can this array hold?
  int count;  // How many states does the array currently hold?
  char **elements;  // The string elements contained in the array
} Array;

/* researching unions in C right now for stretch goal #2, looking at how the array could handle multiple data types */

/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array (int capacity) {
  // Allocate memory for the Array struct
  Array *arr = malloc(sizeof(Array));
  // Set initial values for capacity and count
  /* using arrow to retrieve the structure member as per last week's questions */
  arr->capacity = capacity;
  arr->count = 0; /* initialize counter to 0 as normal */
  // Allocate memory for elements
  arr->elements = calloc(capacity, sizeof(char *));

  return arr;
}


/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr) {

  // Free all elements
  free(arr->elements);
  // Free array
  free(arr);
}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
void resize_array(Array *arr) {

  // Create a new element storage with double capacity
  char **new_storage = calloc((arr->capacity * 2), sizeof(char *)); 
  // Copy elements into the new storage
  for(int i = 0; i < arr->count; i++) {
    new_storage[i] = arr->elements[i];
  }
  // Free the old elements array (but NOT the strings they point to)
  free(arr->elements);
  // Update the elements and capacity to new values
  arr->elements = new_storage;
  arr->capacity *= 2;
}



/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index) {

  // Throw an error if the index is greater than the current count
  if(index > arr->count) {
    fprintf(stderr, "Index out of range.\n");
    exit(0);
  }
  // Otherwise, return the element at the given index
  else {
    return arr->elements[index]; /* not sure this else is necessary, can probably just do return outside of any inner block */
  }
}


/*****
 * Insert an element to the array at the given index
 *****/
void arr_insert(Array *arr, char *element, int index) {

  // Throw an error if the index is greater than the current count
  if(index > arr->count) {
    fprintf(stderr, "Index out of range!\n");
    exit(0);
    /* looking at how to do the above while using errno as well */
  }
  // Resize the array if the number of elements is over capacity
  if(arr->capacity == arr->count) {
    resize_array(arr);
  }
  // Move every element after the insert index to the right one position
  for(int i = arr->count; i >= index; i--) {
    arr->elements[i+1] = arr->elements[i]; /* shifts each elements one position right */
  }
  // Copy the element and add it to the array
  arr->elements[index] = element;
  // Increment count by 1
  arr->count++;
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element) {

  // Resize the array if the number of elements is over capacity
  // or throw an error if resize isn't implemented yet.
  /* will redo this with resizing when resize_array has been completed */
  if(arr->capacity == arr->count) {
    fprintf(stderr, "Array is full! Please free some space.\n");
    exit(1);
  }
  // Copy the element and add it to the end of the array
  arr->elements[arr->count] = element;
  // Increment count by 1
  arr->count++;
}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element) {

  // Search for the first occurence of the element and remove it.
  for(int i = 0; i < arr->count; i++) {
    if(arr->elements[i] == element) {
      arr->elements[i] = NULL;
      free(arr->elements[i]); /* when the desired element is found, null the value and free the memory block */
      for(int x = i; x < arr->count; x++) {
        arr->elements[x] = arr->elements[x + 1]; /* shifts the values after target occurrence */
      }
    }
  }
  // Don't forget to free its memory!

  // Shift over every element after the removed element to the left one position

  // Decrement count by 1
  arr->count--;
}


/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr) {
  printf("[");
  for (int i = 0 ; i < arr->count ; i++) {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1) {
      printf(",");
    }
  }
  printf("]\n");
}


#ifndef TESTING
int main(void)
{

  Array *arr = create_array(1);

  arr_insert(arr, "STRING1", 0);
  arr_append(arr, "STRING4");
  arr_insert(arr, "STRING2", 0);
  arr_insert(arr, "STRING3", 1);
  arr_print(arr);
  arr_remove(arr, "STRING3");
  arr_print(arr);

  destroy_array(arr);

  return 0;
}
#endif
