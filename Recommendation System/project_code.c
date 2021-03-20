#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // for calculating square root with sqrt(x)

#define SIZE 5000 // size of char array
#define STRLENGTH 50 // length for user name

/* ---FUNCTION PROTOTYPES --- */

/* SECTION I ---> File Operations and Extracting the Input */

void getFileIntoString(char *str); // Gets file and reads into char string
char **getItems(char *str, int *totalItems); // Extracts the string and returns the item names and changes the set value of totalItems
int getNumberOfUsers(char *str); // returns the number of total users
int getnumOfNewUser(char *str); // returns the number of new users
// Extract input (char *str) into users and newUsers integer matrix, extract names into char matrices
void extractInput(char *str, int **users, int **newUsers, int numOfUsers, char **userNames, char **newUserNames);
int searchInNames(char **names, char *name, int num); // For given *name search for in **names and returns the starting index

int **createIntegerMat(int row, int col); // For given row and column values returns the integer matrix
char **createCharMat(int row, int col); // For given row and column value returns the char matrix

/* SECTION II  ---> Scientific Calculation and related functions */

void swapInt(int *a, int *b); // Swaps two integer numbers
void swapFloat(float *a, float *b); // Swaps two float numbers
float v1_mean(int *A, int N); // Version 1 of mean
float v2_mean(int *arr, int N, int *commonBooks); // Version 2 of mean calculates mean with the common books
float pearson(int *X, int *Y, int N, int option); // Given X and Y arrays calculates the pearson correlation option declares which mean function to use
int *finKthLargestElement(float *arr, int N, int k); // Finds k largest element with O(k*n) and returns the indices of these elements as integer array

// Wrapper function. Wraps pearson correlation for all users, print k nearest users, calculate prediction 
void calculatePrediction(int *newUser, int **usersMat, char **userNames, char **items, int numOfUsers, int numOfItems, int kLargest);


/* SECTION I ---> File Operations and Extracting the Input */

// Extracts the string and returns the item names and set value for totalItems
char **getItems(char *str, int *totalItems) { // Returns the number of rows which is in case number of items
  int i;
  char **items = createCharMat(50,50); // 
  i = 0;
  int k = 0, beginIndex = 0, endIndex = 0;  // k items index and also refers to number of items
   while (str[i] != '\n') { // First line is where the books are
    if (str[i] == ';') {
      endIndex = i - 1;
      strncpy(items[k], str + beginIndex, endIndex - beginIndex + 1);
      beginIndex = i + 1;
      k++;
    }
    i++;
  }
  items[0] = NULL; // USERS field
  strncpy(items[k], str + beginIndex, i-beginIndex-1);
  items[k+1] = NULL; // Last element is null
  (*totalItems) = k;
  return items;
}

// returns the number of total users
int getNumberOfUsers(char *str) {
  int i = 0, row = 0;
  while (str[i] != '\0') {
    if (str[i] == '\n')
      row++;
    i++;
  }
  return row;
}

// Return the number of nuw users
int getnumOfNewUser(char *str) {
  int i = 0;
  while (str[i] != '\n') // Skips column names
    i++;
  i++;
  int counter = 0;
  while (str[i] != '\0') {
    if (str[i] == 'N') // I search for the 'N' for extracting new users
      counter++;
    i++;
  }
  return counter;
}

// Extracts the string into integer user matrix, integer newUser matrix and gets the userNames and newUsernames
void extractInput(char *str, int **users, int **newUsers, int numOfUsers, char **userNames, char **newUserNames) {
  int i = 0, j = 0;
  // Traverse in string
  while (str[i] != '\n') // Skips column names
    i++;
  i++;
  int row1 = 0, col1 = 0, row2 = 0, col2 = 0;
  int tmpCol; // For getting user names
  int counter = 0; // for checking if numOfUsers exceed new numOfUsers if it happens time to get new users
  while (str[i] != '\0') {
    if (str[i] == 'U' && counter < numOfUsers) { // range of current users
      j = i;
      tmpCol = 0;
      while (str[j] != ';') { // For getting user names
        userNames[row1][tmpCol] = str[j];
        tmpCol++;
        j++;
      }
      userNames[row1][tmpCol] = '\0';
      counter++;
      col1 = 0;
      while (str[j] != '\n' && str[j] != '\0') {
        if (str[j] == ' ') 
          str [j] = '0'; 
        if ( (str[j] >= '0' && str[j] <= '9') ) {
          users[row1][col1] = str[j] - '0';
          col1++;
        }
        j++;
      }
      row1++;
    } 
    else if (str[i] == 'N' && counter >= numOfUsers) { // get new users
      j = i;
      tmpCol = 0;
      while (str[j] != ';') { // For getting user names
        newUserNames[row2][tmpCol] = str[j];
        tmpCol++;
        j++;
      }
      newUserNames[row2][tmpCol] = '\0';
      col2 = 0;
      while (str[j] != '\n' && str[j] != '\0') {
        if (str[j] == ' ') 
          str [j] = '0'; 
        if ( (str[j] >= '0' && str[j] <= '9') ) {
          newUsers[row2][col2] = str[j] - '0';
          col2++;
        }
        j++;
      }
      row2++;
    }
    i++;
  }
}

// Gets file and reads into char string
void getFileIntoString(char *str) {
  int option = 0;
  while (option == 0) {
    printf("\nPress 1 for using current file which is recdataset.csv\n");
    printf("Press 2 for using another file. Make sure your file is well formatted otherwise results will be wrong \n");
    scanf("%d",&option);
    if ( (option != 1) && (option != 2) ) {
      printf("Error! You gave wrong input please try again\n");
      option = 0; // Resetting option
    }
  }
  FILE *fp;
  if (option == 2) {
    char fname[20];
    printf("Please enter the file name. Make sure your file is .csv file \n");
    scanf("%s",fname);
    fp = fopen(fname, "r"); // Read only
    if (fp==NULL) {
      printf("Error! File is not valid.\n"); // If file is not valid then exit
      exit(0);
    }
  } 
  else if (option == 1) {
    fp = fopen("recdataset.csv", "r"); // Getting the current file
    if (fp==NULL) {
      printf("Error! File is not valid.\n"); // If file is not valid then exit
      exit(0);
    }
  }
  // Reading file into char string
  char c;
  int i = 0;
  while ((c = getc(fp)) != EOF) {
    str[i] = c;
    i++;
  }

  fclose(fp);
}

// For given string array and string, find the string in string array and returns corresponding index
int searchInNames(char **names, char *name, int num) {
  int i = 0;
  for (i=0; i<num; i++) {
    if (strcmp(names[i],name) == 0) {
      return i;
    }
  }
  return -1;
}

// Utility function - for given row and column returns integer matrix
int **createIntegerMat(int row, int col) {
  int **mat = (int**) calloc(row, sizeof(int *)); 
  int i;
  for (i=0; i<row; i++) 
    mat[i] = (int*) calloc(col, sizeof(int)); 
  return mat;
}

// Utility function - for given row and column returns char matrix (which is string array)
char **createCharMat(int row, int col) {
  char **mat = (char**) calloc(row, sizeof(char*)); 
  int i;
  for (i=0; i<row; i++) 
    mat[i] = (char*) calloc(col, sizeof(char)); 
  return mat;
}

/* SECTION II  ---> Scientific Calculation and related functions */

// Takes array A and size of the array returns mean 
float v1_mean(int *A, int N) {
  float sum = 0;
  int i, counter = 0;
  for (i=0; i<N; i++) {
    if (A[i] != 0) { // if user reads the book
      sum += A[i];
      counter++;
    }
  }
  return sum / counter;
}
// Takes array A and size of the array returns mean 
float v2_mean(int *arr, int N, int *commonBooks) {
  float sum = 0;
  int i, counter = 0;
  for (i=0; i<N; i++) {
    if (commonBooks[i]) { // if user reads the book
      sum += arr[i];
      counter++;
    }
  }
  return sum / counter;
}

// Calculate pearson correlation for given option and return the output
// option 1 calculating pearson correlation with mean of users who read the book
// option 2 calculating pearson correlation with mean of users who both read the book
float pearson(int *X, int *Y, int N, int option) {
  float upperPart = 0; // = sum( (x-x') * (y-y') ) --Upper part
  float lowerPart = 0; // = sqrt( sum(x-x')^2 * sum(y-y')^2 ) --Lower part

  float sum1 = 0;  // // part of lowerPart --> sum(x-x')^2
  float sum2 = 0; // part of lowerPart --> sum(y-y')^2
  // Calculation for lowerPart = sqrt(sum1 * sum2)
  float r = 0;         // r = upperPart / lowerPart

  int *commonBooks = (int*) calloc (N, sizeof(int));
  int i = 0;
  for (i=0; i<N; i++) {
    if (X[i] != 0 && Y[i] != 0) { // common book
      commonBooks[i] = 1;
    }
  }
  // choosing which method to use
  float mean_x, mean_y;
  if (option == 1) { // Version 1 of pearson correlation with mean of users who read the book
    mean_x = v1_mean(X,N);
    mean_y = v1_mean(Y,N);
  } 
  else if (option == 2) { // Version 2 of pearson correlation with mean of users who both read the book
    mean_x = v2_mean(X,N,commonBooks);
    mean_y = v2_mean(Y,N,commonBooks);
  }
  for (i=0; i<N; i++) {
    if (commonBooks[i]) {
      upperPart += (X[i] - mean_x) * (Y[i] - mean_y);

      sum1 +=  pow((X[i] - mean_x),2);
      sum2 += pow((Y[i] - mean_y),2);
    }
  }
  lowerPart = sqrt(sum1*sum2);
  r = upperPart / lowerPart;

  return r; // Returns the pearson correlation value
}

// Wrapper function to calculate prediction
void calculatePrediction(int *newUser, int **usersMat, char **userNames, char **items, int numOfUsers, int numOfItems, int kLargest) {
  float (*similarities) = (float*) calloc (numOfUsers, sizeof(float));

  int option = 0;
  while (option != 1 && option != 2) { // For getting valid input
    printf("\nThere are two versions for calculating the Pearson Correlation\n");
    printf("Press 1 for first version  -> Calculating Correlation with the 'mean of the users who read the book'\n");
    printf("Press 2 for second version -> Calculating Correlation with the 'mean of the users who BOTH read the SAME book'\n");
    scanf("%d",&option);
    if (option != 1 && option != 2) 
      printf("Error! Wrong input please try again\n");
  }

  int i;
  if (option == 1) { // if user select the mean of the users who read the book
    for (i=0; i<numOfUsers; i++) {
      similarities[i] = pearson(usersMat[i], newUser, numOfItems, option);
      // printf("%d %f\n",i+1, similarities[i]); prints all the users similarities
    }
  } 
  else if (option == 2) { // if user select the mean of the users who BOTH read the SAME book
    for (i=0; i<numOfUsers; i++) { 
      similarities[i] = pearson(usersMat[i], newUser, numOfItems, option);
    }
  }
  // Finding k largest elements function returns max element's indices
  int *maxIndices = finKthLargestElement(similarities, numOfUsers, kLargest);

  // Printing similarities
  printf("\n---Most Similar %d Users---\n",kLargest);
  printf("User Name\tSimilarity\n");
  for (i=0; i<kLargest; i++) {
    printf("%s\t\t%f \n",userNames[maxIndices[i]], similarities[maxIndices[i]]);
  }
  printf("\n");
  // --- Prediction calculation ---
  int books[10] = {0}; // stores books indices
  float rates[10] = {0}; // stores rates (corresponds the same indices as books array) 

  float mean_a = v1_mean(newUser, numOfItems); // Because of the formula mean of a calculated outside of the loop
  float mean_b, pearson;
  float lower = 0, upper = 0; // For calculating prediction formula
   
  int cnt = 0, j; // cnt is used for book indices

  for (i=0; i<numOfItems; i++) {
    lower = 0; // resetting lower part of the formula
    upper = 0; // resetting upper part of the formula
    if (newUser[i] == 0) { // We want to calculate prediction only books that user hasn't been read yet
      for (j=0; j<kLargest; j++) { // Calculating prediction with the help of closest k users
        pearson = similarities[maxIndices[j]];
        mean_b = v1_mean(usersMat[maxIndices[j]], numOfItems);
        upper += (pearson) * (usersMat[maxIndices[j]][i] - mean_b);
        lower += pearson;
      }
      books[cnt] = i+1; // Plus 1 because I get input in this way
      rates[cnt] = mean_a + (upper/lower);
      printf("Book %s's predicted value is: %f\n", items[i+1], rates[cnt]); // Printing user's predicted value
      cnt++;
    }
  }
  // Finding max rates and printing recommended book
  float max = rates[0];
  int indexOfMax = 0;
  for (i=1; i<cnt; i++) {
    if (rates[i] > max) {
      max = rates[i];
      indexOfMax = i;
    }
  }
  printf("\nRecommended book is %s\n", items[books[indexOfMax]]);
}

// Utility Function to swap the two float numbers
void swapFloat(float *a, float *b) {
  float tmp = *a;
  *a = *b;
  *b = tmp;
}
// Utility Function to swap the two integer numbers
void swapInt(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

// Modified version of selection sort only sorts for k elements 
// time complexity is O(k*n) returns index of largerst elements 
int *finKthLargestElement(float *arr, int N, int k) {
  float *tmpArr = (float*) calloc (N, sizeof(float)); // Stores the copy of arr
  int *indices = (int*) calloc (N, sizeof(int)); // Stores the copy of indices
  int i;
  // Copying both array and indices into array
  for (i=0; i<N; i++) {
    tmpArr[i] = arr[i];
    indices[i] = i;
  }
  int *maxIndices = (int*) calloc (k, sizeof(int)); // We will return the max indices
  float tmp;
  int j, max;
  int cnt = 0;
  for (i = 0; i < k; i++)  {  
      max = i;
      for (j = i+1; j < N; j++) {
        if (tmpArr[j] > tmpArr[max]) {
          max = j;  
          maxIndices[cnt] = max;
          cnt++;
        }
      }
      swapFloat(&tmpArr[max], &tmpArr[i]);
      swapInt(&indices[max], &indices[i]);
  }
  for (i=0; i<k; i++) {
    maxIndices[i] = indices[i]; // Finding max indices
  }
  return maxIndices;
}


// MAIN SECTION
int main () {
  char *str = (char*)calloc(SIZE, sizeof(char));
  getFileIntoString(str);
  // Declaring variables
  int totalItems;
  char **items = getItems(str, &totalItems);  // Stores the item names
  int totalUsers = getNumberOfUsers(str); // Stores the number of all users

  int numOfNewUsers = getnumOfNewUser(str); // Getting number of new users
  int numOfUsers = totalUsers - numOfNewUsers; // Finding our current users

  // Declaring matrices
  int **users = createIntegerMat(numOfUsers, totalItems);  // Integer users matrix will be store ratings
  int **newUsers = createIntegerMat(numOfNewUsers, totalItems); // Integer new users matrix will be store ratings
  char **userNames = createCharMat(numOfUsers, STRLENGTH); // Char matrix will store the names
  char **newUserNames = createCharMat(numOfNewUsers, STRLENGTH); // Char matrix will store the new user names

  // Extracting string into these variables
  extractInput(str, users, newUsers, numOfUsers, userNames, newUserNames);

  // Getting input
  char newUserName[30];
  int index = -1;
  while (index == -1) {
    printf("\nEnter the new user name\n");
    scanf("%s",newUserName);
    index = searchInNames(newUserNames, newUserName, numOfNewUsers); 
    if (index == -1) { // Checking if user exists
      printf("User name does not exists. Please try again\n");
    } // if user not exists then we will try again
  }

  int kNearest = -1;   // setting k to -1 
  while (kNearest == -1) { 
    printf("\nEnter the value of 'k' for finding most similar k users\n");
    scanf("%d",&kNearest);
    // k cannot be larger than number of users or shouldn't be smaller then 1
    if ( (kNearest < 1) || (kNearest > numOfUsers) ) {  // Validating k
      printf("Error! 'k' is not valid. Try again...\n");
      kNearest = -1; // if it is not in our range then we will be setting -1 again
    }
  } 
  // Finally calculating the prediction with this wrapper function
  calculatePrediction(newUsers[index], users, userNames, items, numOfUsers, totalItems, kNearest);

  return 0;
}