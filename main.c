// Test 1 - Michal Zarnowski, 991552312
// I will make an honest and truthful effort to complete this assessment on my own.
// Option 3

#include <stdio.h>
#include <string.h>

// STRUCTS
typedef struct color {
    int r, g, b;
}Color;

typedef struct sampleImage {
    char fileName[25];
    int height, width;
    Color color;
}SampleImage;

// METHODS
void printMenuOptions();
void generateSampleImage();
SampleImage constructSampleImageStruct(char fileName[], int height, int width, Color color);
void constructSampleImageFile(SampleImage sampleImage);
void convertToGreyscale();
void getFileNameFromUser(char fileName[], char fileNameWithExtension[]);
int checkFileFormat(FILE *originalFile);
void convertImageToGreyscale(FILE *originalFile, char fileName[]);
void copyFileHeader(FILE *originalFile, FILE *convertedFile, int dimensions[]);
void flipImage();
void flipImageFile(FILE *originalFile, char fileName[]);
void createPixelArray(Color pixels[], FILE *originalFile);
void reverseArray(Color array[], int end);

int main() {

    // Main menu
    int menuOption;
    do {
        printMenuOptions();

        scanf("%d", &menuOption);

        // Handle menu input
        switch (menuOption) {
            case 1:
                generateSampleImage();
                break;
            case 2:
                convertToGreyscale();
                break;
            case 3:
                flipImage();
                break;
            case 4:
                printf("\nGoodbye!\n");
                break;
            default:
                printf("\nOops, something went wrong!\nPlease choose available option.\n\n");
        }
    }while(menuOption != 4);

    return 0;
}

void printMenuOptions() {

    printf("Please choose one of the following options:\n");
    printf("\t1. Generate test image\n");
    printf("\t2. Convert image to greyscale\n");
    printf("\t3. Flip image horizontally\n");
    printf("\t4. Quit\n");
    printf("Enter your choice: ");
}

void generateSampleImage() {

    // Image data
    char fileName[25];
    int height, width;
    Color tempColor;

    // Get image data from user
    printf("\nSAMPLE IMAGE GENERATOR\n\n");
    printf("Enter the file name you'd like to save as:");
    scanf("%s", fileName);

    printf("Enter image height:");
    scanf("%d", &height);

    printf("Enter image width:");
    scanf("%d", &width);

    printf("Enter line color in R G B format (ex. 255 255 255):");
    scanf("%d", &tempColor.r);
    scanf("%d", &tempColor.g);
    scanf("%d", &tempColor.b);

    // Construct SampleImage struct
    SampleImage sampleImage = constructSampleImageStruct(fileName, height, width, tempColor);

    // Construct sample image file
    constructSampleImageFile(sampleImage);

//    printf("\n\n%s\n%d\n%d\n%d\n%d\n%d\n",sampleImage.fileName, sampleImage.height, sampleImage.width, sampleImage.color.r, sampleImage.color.g, sampleImage.color.b);
}

SampleImage constructSampleImageStruct(char fileName[], int height, int width, Color color) {

    // Construct SampleImage struct
    SampleImage sampleImage;
    strcpy(sampleImage.fileName, strcat(fileName, ".ppm"));
    sampleImage.height = height;
    sampleImage.width = width;
    sampleImage.color = color;

    return sampleImage;
}

void constructSampleImageFile(SampleImage sampleImage) {

    // Create a file
    FILE *outputFile = fopen(sampleImage.fileName,"w");

    // Add file header to file
    char fileHeader[15];
    snprintf(fileHeader, 15, "P3\n%d %d\n255\n", sampleImage.width, sampleImage.height);
    fprintf(outputFile, "%s", fileHeader);

    // Compose the image
    int lineCounter = 0;
    char tempColor[15];

    for (int i = 0; i < sampleImage.height; i++) {
        for (int j = 0; j < sampleImage.width; j++) {

            // Print colored pixels on even lines, white pixels on odd lines
            if (lineCounter == 0 || lineCounter % 2 == 0) {
                snprintf(tempColor, 15, "%d %d %d\n",
                        sampleImage.color.r, sampleImage.color.g, sampleImage.color.b);
            } else {
                snprintf(tempColor, 15, "255 255 255\n");
            }
            fprintf(outputFile, "%s", tempColor);
        }
        lineCounter++;
    }

    // Close file
    fclose(outputFile);
}

void convertToGreyscale() {

    printf("\nGREYSCALE IMAGE CONVERTER\n");

    // Ask user for file to open
    char fileName[40], fileNameEx[40] = "";
    getFileNameFromUser(fileName, fileNameEx);

    // Try to open the file
    FILE *originalFile = fopen(fileNameEx,"r");

    int valid;
    if (originalFile == NULL) {
        printf("\nFile not found\n\n");
    }
    else {
        // Validate file format
        valid = checkFileFormat(originalFile);
        if (valid == 0) {
            convertImageToGreyscale(originalFile, fileName);
            printf("\nImage converted successfully\n\n");
        } else {
            printf("\nInvalid file format\n\n");
        }
    }

    //Close files
    fclose(originalFile);
}

void getFileNameFromUser(char fileName[], char fileNameWithExtension[]) {
    printf("Enter file name to convert without file extension:");
    scanf("%s", fileName);
    strcat(fileNameWithExtension, fileName);
    strcat(fileNameWithExtension, ".ppm");
}

int checkFileFormat(FILE *originalFile) {

    char one, two;

    // Get first two characters from file and rewind the file pointer
    fscanf(originalFile, "%c", &one);
    fscanf(originalFile, "%c", &two);
    fseek(originalFile, 0, SEEK_SET);

    // If the characters are 'P3' then it's a valid file
    if (one == 'P' && two == '3') {
        return 0;
    }

    return 1;
}

int validateFile(char fileName[]) {
    int strLen = strlen(fileName);

    if (fileName[strLen-4] != '.' && fileName[strLen-3] != 'p' &&
        fileName[strLen-2] != 'p' && fileName[strLen-1] != 'm' ) {
        return 1;
    }

    return 0;
}

void convertImageToGreyscale(FILE *originalFile, char fileName[]) {

    // Open new output file
    char newFileName[50] = "";
    strcat(newFileName, fileName);
    strcat(newFileName, "-greyscale");
    strcat(newFileName, ".ppm");
    FILE *convertedFile = fopen(newFileName,"w");

    // Copy file header
    int dimensions[2];
    copyFileHeader(originalFile, convertedFile, dimensions);

    // Create array of image pixels
    Color pixels[100000];
    createPixelArray(pixels, originalFile);

    //Convert to greyscale
    int arrLen = dimensions[0] * dimensions[1], combinedPixels = 0;
    double greyedPixel;
    char line[15];
    for(int i = 0; i < arrLen; i++){

        // Get greyscale color using luminosity formula
        greyedPixel= 0.21 * pixels[i].r + 0.72 * pixels[i].g + 0.07 * pixels[i].b;

        // Add pixel's greyscale value to the total for luminosity calculation
        combinedPixels += (int)greyedPixel;

        // Construct pixel with new color
        snprintf(line, 15, "%d %d %d\n",
                 (int)greyedPixel, (int)greyedPixel, (int)greyedPixel);

        // Add to converted file
        fprintf(convertedFile, "%s", line);
    }

    // Print average luminosity
    printf("Avergare luminosity: %d%%\n", (int)(combinedPixels/arrLen));

    // Close file
    fclose(convertedFile);
}

void copyFileHeader(FILE *originalFile, FILE *convertedFile, int dimensions[]) {

    // Copy file header
    char h1, h2;
    int width, height, maxColor;

    // Extract header data
    fscanf(originalFile, "%c", &h1);
    fscanf(originalFile, "%c", &h2);
    fscanf(originalFile, "%d", &width);
    fscanf(originalFile, "%d", &height);
    fscanf(originalFile, "%d", &maxColor);

    // Assign dimensions
    dimensions[0] = width;
    dimensions[1] = height;

    // Paste header
    fprintf(convertedFile, "%c", h1);
    fprintf(convertedFile, "%c", h2);
    fprintf(convertedFile, "%c", '\n');
    fprintf(convertedFile, "%d", width);
    fprintf(convertedFile, "%c", ' ');
    fprintf(convertedFile, "%d", height);
    fprintf(convertedFile, "%c", '\n');
    fprintf(convertedFile, "%d", maxColor);
    fprintf(convertedFile, "%c", '\n');
}

void flipImage() {

    printf("\nIMAGE FLIPPER\n");

    // Ask user for file to open
    char fileName[40], fileNameEx[40] = "";
    getFileNameFromUser(fileName, fileNameEx);

    // Try to open the file
    FILE *originalFile = fopen(fileNameEx,"r");

    int valid;
    if (originalFile == NULL) {
        printf("\nFile not found\n\n");
    }
    else {
        // Validate file format
        valid = checkFileFormat(originalFile);
        if (valid == 0) {
            flipImageFile(originalFile, fileName);
            printf("\nImage flipped successfully\n\n");
        } else {
            printf("\nInvalid file format\n\n");
        }
    }

    //Close files
    fclose(originalFile);
}

void flipImageFile(FILE *originalFile, char fileName[]) {

    // Open new output file
    char newFileName[50] = "";
    strcat(newFileName, fileName);
    strcat(newFileName, "-flipped");
    strcat(newFileName, ".ppm");
    FILE *flippedFile = fopen(newFileName,"w");

    // Copy file header
    int dimensions[2];
    copyFileHeader(originalFile, flippedFile, dimensions);

    // Create array of image pixels
    Color pixels[100000];
    createPixelArray(pixels, originalFile);

    // FLIP IMAGE LOGIC
    int width = dimensions[0], height = dimensions[1], count = 0;
    Color tempArray[width];
    char newLine[15];

    // Repeat for each pixel row
    for (int i = 0; i < height; i++) {

        // Create array holding pixel structs for entire row
        for (int j = (i * width); j < ((i + 1) * width); j++) {
            tempArray[count] = pixels[j];
            count++;
        }

        // Reverse the pixels in the row
        reverseArray(tempArray, width);

        // Construct new reversed row
        for (int k = 0; k < width; k++) {
            // Construct new line to add to file
            snprintf(newLine, 15, "%d %d %d\n", tempArray[k].r, tempArray[k].g, tempArray[k].b);

            // Add to flipped file
            fprintf(flippedFile, "%s", newLine);
        }

        count = 0;
    }

    // Close file
    fclose(flippedFile);
}

void createPixelArray(Color pixels[], FILE *originalFile) {

    Color tempPixel;
    int r, g, b, counter = 0;

    // Loop through file, create pixel structs, add to array
    while (fscanf(originalFile, "%d", &r) != EOF) {
        fscanf(originalFile, "%d", &g);
        fscanf(originalFile, "%d", &b);

        tempPixel.r = r;
        tempPixel.g = g;
        tempPixel.b = b;

        pixels[counter] = tempPixel;
        counter++;
    }
}

void reverseArray(Color array[], int end) {

    // Array reversing logic
    for (int min = 0, max = end - 1; min < max; min++, max--) {
        Color temp = array[min];
        array[min] = array[max];
        array[max] = temp;
    }
}
