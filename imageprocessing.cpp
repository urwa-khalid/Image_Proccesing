#include <iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<cmath>
#include<string>
#include <numbers>
#include<cstdlib>
#define M_PI 3.14159265358979323846
#define _USE_MATH_DEFINES

using namespace std;

struct Image {
    char ImageFileName[100];
    vector<vector<int>> ImageData;
    int cols, rows, maxGray;
    vector<char> comment;

    bool imageLoaded;
    bool imageModified;

    void changeBrightness(double factor) {
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++) {
                ImageData[r][c] *= factor;
                if (ImageData[r][c] > maxGray)
                    ImageData[r][c] = maxGray;
            }
    }

    int loadImage(char ImageName[]) {

        ifstream FCIN(ImageName);

        if (!FCIN.is_open())
            return -1;

        char MagicNumber[5];
        char Comment[100];

        FCIN.getline(MagicNumber, 4);
        FCIN.getline(Comment, 100);
        FCIN >> cols >> rows >> maxGray;

        ImageData.clear();
        ImageData.resize(rows, vector<int>(cols, 0));

        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                FCIN >> ImageData[r][c];

        if (FCIN.fail())
            return -2;

        FCIN.close();
        imageLoaded = true;
        imageModified = false;
        strcpy_s(ImageFileName, sizeof(ImageFileName), ImageName);

        return 0;
    }
    int saveImage(char ImageName[]) {
        ofstream FCOUT(ImageName);
        if (!FCOUT.is_open())
            return -1;

        FCOUT << "P2\n# This is a comment\n"
            << cols << " " << rows << endl << maxGray << endl;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++)
                FCOUT << ImageData[r][c] << " ";
            FCOUT << endl;
        }
        FCOUT.close();
        imageModified = false;
        return 0;
    }

    void ImageScaling(double factor) {
 
        if (factor <= 0.0) {
            cout << "Invalid scaling factor. Please use a positive factor." << endl;
            return;
        }
        vector<vector<int>> scaledImageData;

        int newRows = static_cast<int>(rows * factor);
        int newCols = static_cast<int>(cols * factor);

        scaledImageData.resize(newRows, vector<int>(newCols, 0));

        for (int r = 0; r < newRows; r++) {
            for (int c = 0; c < newCols; c++) {

                int originalRow = static_cast<int>(r / factor);
                int originalCol = static_cast<int>(c / factor);

                scaledImageData[r][c] = ImageData[originalRow][originalCol];
            }
        }
        ImageData = scaledImageData;
        rows = newRows;
        cols = newCols;
        imageModified = true;
    }

    void contrastStretching() {
        int minPixel = maxGray;
        int maxPixel = 0;

        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++) {
                if (ImageData[r][c] < minPixel)
                    minPixel = ImageData[r][c];
                if (ImageData[r][c] > maxPixel)
                    maxPixel = ImageData[r][c];
            }

        double scale = 255.0 / (maxPixel - minPixel);

        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++) {
                ImageData[r][c] = static_cast<int>((ImageData[r][c] - minPixel) * scale);
            }
        imageModified = true;
    }

    void ImageTranslating(int horizontalShift, int verticalShift) {

        vector<vector<int>> newImageData(rows, vector<int>(cols, 0));

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
         
                int newR = (r + verticalShift);
                int newC = (c + horizontalShift);

                if (newR >= 0 && newR < rows && newC >= 0 && newC < cols) {
                    newImageData[newR][newC] = ImageData[r][c];
                }
            }
        }

        ImageData = newImageData;
        imageModified = true;

    }
   
    void RotationofImage(double theta, bool reverse = false) {
      
        theta = (theta * M_PI) / 180.0;

        vector<vector<int>> rotatedImageData(rows, vector<int>(cols, 255));

        double centerX = cols / 2.0;
        double centerY = rows / 2.0;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
               
                double x = c - centerX;
                double y = r - centerY;

                double newX, newY;
                if (reverse) {
                    newX = x * cos(theta) - y * sin(theta);
                    newY = x * sin(theta) + y * cos(theta);
                }
                else {
                    newX = x * cos(theta) + y * sin(theta);
                    newY = -x * sin(theta) + y * cos(theta);
                }

                int originalR = static_cast<int>(newY + centerY);
                int originalC = static_cast<int>(newX + centerX);

                if (originalR >= 0 && originalR < rows && originalC >= 0 && originalC < cols) {
                    rotatedImageData[r][c] = ImageData[originalR][originalC];
                }
            }
        }
        ImageData = rotatedImageData;
        imageModified = true;
    }

    void SharpeningofImage(double sigma = 1.0, double strength = 1.5) {
       
        vector<vector<int>> blurredImageData(rows, vector<int>(cols, 0));

        applyGaussianBlur(sigma, blurredImageData);
        vector<vector<int>> sharpenedImageData(rows, vector<int>(cols, 0));

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int diff = ImageData[r][c] - blurredImageData[r][c];
                int newValue = ImageData[r][c] + static_cast<int>(strength * diff);
                sharpenedImageData[r][c] = min(max(newValue, 0), maxGray); 
            }
        }
        ImageData = sharpenedImageData;
        imageModified = true;
    }

    void applyGaussianBlur(double sigma, vector<vector<int>>& outputImageData) {
        
        double kernel[3][3] = {
            {1.0 / 16, 2.0 / 16, 1.0 / 16},
            {2.0 / 16, 4.0 / 16, 2.0 / 16},
            {1.0 / 16, 2.0 / 16, 1.0 / 16}
        };
        for (int r = 1; r < rows - 1; r++) {
            for (int c = 1; c < cols - 1; c++) {
                double sum = 0.0;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        sum += kernel[i + 1][j + 1] * ImageData[r + i][c + j];
                    }
                }
                outputImageData[r][c] = static_cast<int>(sum);
            }
        }
    }

    void applyMeanFilter() {
        vector<vector<int>> newImageData(rows, vector<int>(cols, 0));

        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                int sum = 0;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        sum += ImageData[r + i][c + j];
                    }
                }
                newImageData[r][c] = sum / 9; 
            }
        }
        ImageData = newImageData;
    }

    void applyMedianFilter() {
        vector<vector<int>> newImageData(rows, vector<int>(cols, 0));

        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                int neighborhood[9];
                int index = 0;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        neighborhood[index++] = ImageData[r + i][c + j];
                    }
                }

                for (int i = 0; i < 8; ++i) {
                    for (int j = 0; j < 8 - i; ++j) {
                        if (neighborhood[j] > neighborhood[j + 1]) {
                            int temp = neighborhood[j];
                            neighborhood[j] = neighborhood[j + 1];
                            neighborhood[j + 1] = temp;
                        }
                    }
                }
                newImageData[r][c] = neighborhood[4];
            }
        }

        ImageData = newImageData;
    }

    void applyLinearFilter(const vector<vector<double>>& filter) {
        int rows = ImageData.size();
        int cols = ImageData[0].size();
        int filterSize = filter.size();
        int filterRadius = filterSize / 2;

        vector<vector<int>> newImageData(rows, vector<int>(cols, 0));

        for (int r = filterRadius; r < rows - filterRadius; ++r) {
            for (int c = filterRadius; c < cols - filterRadius; ++c) {
                double sum = 0.0;

                for (int i = -filterRadius; i <= filterRadius; ++i) {
                    for (int j = -filterRadius; j <= filterRadius; ++j) {
                        sum += filter[i + filterRadius][j + filterRadius] * ImageData[r + i][c + j];
                    }
                }
                newImageData[r][c] = static_cast<int>(sum);
            }
        }

        ImageData = newImageData;
    }

    int  applyLinearFilterFromFile(const string& filterFileName) {
        cout << filterFileName << endl;

        ifstream filterFile(filterFileName);

        if (!filterFile.is_open()) {
            cout << "Unable to open filter file." << endl;
            return -10;
        }

        int filterSize;

        filterFile >> filterSize;
        cout << filterSize << endl;

        vector<vector<double>> linearFilter(filterSize, vector<double>(filterSize));

        for (int i = 0; i < filterSize; ++i) {
            for (int j = 0; j < filterSize; ++j) {
                filterFile >> linearFilter[i][j];
            }
        }

        filterFile.close();

        applyLinearFilter(linearFilter);
    }

    void enhanceImageWithFilter(const vector<vector<double>>& customFilter) {
        int filterSize = customFilter.size();
        int filterRadius = filterSize / 2;

        vector<vector<int>> newImageData(rows, vector<int>(cols, 0));

        for (int r = filterRadius; r < rows - filterRadius; ++r) {
            for (int c = filterRadius; c < cols - filterRadius; ++c) {
                double sum = 0.0;
                for (int i = -filterRadius; i <= filterRadius; ++i) {
                    for (int j = -filterRadius; j <= filterRadius; ++j) {
                        sum += customFilter[i + filterRadius][j + filterRadius] * ImageData[r + i][c + j];
                    }
                }
                newImageData[r][c] = int(sum);
            }
        }

        ImageData = newImageData;
    }

    void applyDerivativeFilter() {
        int dX[3][3] = { { -1, 0, 1 },{ -2, 0, 2 },{ -1, 0, 1 } };
        int dY[3][3] = { { -1, -2, -1 },{ 0, 0, 0 },{ 1, 2, 1 } };

        int rows = ImageData.size();
        int cols = ImageData[0].size();
        vector<vector<int>> newImageDataX(rows, vector<int>(cols, 0));
        vector<vector<int>> newImageDataY(rows, vector<int>(cols, 0));

        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                int sumX = 0, sumY = 0;

                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        sumX += dX[i + 1][j + 1] * ImageData[r + i][c + j];
                        sumY += dY[i + 1][j + 1] * ImageData[r + i][c + j];
                    }
                }

                newImageDataX[r][c] = sumX;
                newImageDataY[r][c] = sumY;
            }
        }
        vector<vector<int>> magnitude(rows, vector<int>(cols, 0));
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                magnitude[r][c] = sqrt(newImageDataX[r][c] * newImageDataX[r][c] + newImageDataY[r][c] * newImageDataY[r][c]);
            }
        }
        ImageData = magnitude;
    }
     
    void findEdges() {

        vector<vector<int>> sobelX = { { -1, 0, 1 },
                                        { -2, 0, 2 },
                                        { -1, 0, 1 } };
        vector<vector<int>> sobelY = { { -1, -2, -1 },
                                        { 0, 0, 0 },
                                        { 1, 2, 1 } };
        int rows = ImageData.size();
        int cols = ImageData[0].size();

        vector<vector<int>> gradX(rows, vector<int>(cols, 0));
        vector<vector<int>> gradY(rows, vector<int>(cols, 0));

        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                int gx = 0, gy = 0;

                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        gx += sobelX[i + 1][j + 1] * ImageData[r + i][c + j];
                        gy += sobelY[i + 1][j + 1] * ImageData[r + i][c + j];
                    }
                }
                gradX[r][c] = gx;
                gradY[r][c] = gy;
            }
        }
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                ImageData[r][c] = sqrt(pow(gradX[r][c], 2) + pow(gradY[r][c], 2));
            }
        }
    }

    void FlipImageHorizontally() {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols / 2; c++) {
                int temp = ImageData[r][c];
                ImageData[r][c] = ImageData[r][cols - c - 1];
                ImageData[r][cols - c - 1] = temp;
            }
        }
    }
    void FlipImageVertically() {
        for (int r = 0; r < rows / 2; r++) {
            for (int c = 0; c < cols; c++) {
                int temp = ImageData[r][c];
                ImageData[r][c] = ImageData[rows - r - 1][c];
                ImageData[rows - r - 1][c] = temp;
            }
        }
    }
    void rotate90Clockwise() {
        vector<vector<int>> newImageData(cols, vector<int>(rows, 0));

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                newImageData[c][rows - r - 1] = ImageData[r][c];
            }
        }
        ImageData = newImageData;
        int temp = rows;
        rows = cols;
        cols = temp;
    }

    void rotate90CounterClockwise() {
        vector<vector<int>> newImageData(cols, vector<int>(rows, 0));

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                newImageData[cols - c - 1][r] = ImageData[r][c];
            }
        }

        ImageData = newImageData;
        int temp = rows;
        rows = cols;
        cols = temp;
    }

    void rotateByAngle(double theta) {
        const double pi = 3.14159265358979323846;
        double angleRad = theta * (pi / 180.0);

        int newRows = ceil(abs(cols * sin(angleRad)) + abs(rows * cos(angleRad)));
        int newCols = ceil(abs(cols * cos(angleRad)) + abs(rows * sin(angleRad)));

        vector<vector<int>> newImageData(newRows, vector<int>(newCols, 0));
        double centerRow = rows / 2.0;
        double centerCol = cols / 2.0;

        for (int r = 0; r < newRows; r++) {
            for (int c = 0; c < newCols; c++) {
                int origRow = round((r - newRows / 2.0) * cos(angleRad) - (c - newCols / 2.0) * sin(angleRad) + centerRow);
                int origCol = round((r - newRows / 2.0) * sin(angleRad) + (c - newCols / 2.0) * cos(angleRad) + centerCol);

                if (origRow >= 0 && origRow < rows && origCol >= 0 && origCol < cols) {
                    newImageData[r][c] = ImageData[origRow][origCol];
                }
            }
        }
        ImageData = newImageData;
        rows = newRows;
        cols = newCols;
    }
    void convertToBinary() {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (ImageData[r][c] < maxGray / 2) {
                    ImageData[r][c] = 0; 
                }
                else {
                    ImageData[r][c] = maxGray; 
                }
            }
        }
        imageModified = true;
    }
    void resizeImage(double ratio) {
        int newRows = int(rows * ratio);
        int newCols = int(cols * ratio);

        vector<vector<int>> newImageData(newRows, vector<int>(newCols, 0));

        for (int r = 0; r < newRows; ++r) {
            for (int c = 0; c < newCols; ++c) {
                int originalRow = int(r / ratio);
                int originalCol = int(c / ratio);

                newImageData[r][c] = ImageData[originalRow][originalCol];
            }
        }
        ImageData = newImageData;
        rows = newRows;
        cols = newCols;
    }

    void CroppinganImage(int startRow, int endRow, int startCol, int endCol) {
        if (startRow < 0 || endRow >= rows || startCol < 0 || endCol >= cols ||
            startRow > endRow || startCol > endCol) {
            cout << "Invalid crop parameters." << endl;
            return;
        }

        int newRows = endRow - startRow + 1;
        int newCols = endCol - startCol + 1;

        vector<vector<int>> newImageData(newRows, vector<int>(newCols, 0));

        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                newImageData[r - startRow][c - startCol] = ImageData[r][c];
            }
        }
        ImageData = newImageData;
        rows = newRows;
        cols = newCols;
    }

    void CombiningImagesSidebySide(const Image& otherImage) {
        int newRows = max(rows, otherImage.rows);
        int newCols = cols + otherImage.cols;

        vector<vector<int>> newImageData(newRows, vector<int>(newCols, 0));

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                newImageData[r][c] = ImageData[r][c];
            }
        }

        for (int r = 0; r < otherImage.rows; ++r) {
            for (int c = 0; c < otherImage.cols; ++c) {
                newImageData[r][cols + c] = otherImage.ImageData[r][c];
            }
        }
        ImageData = newImageData;
        rows = newRows;
        cols = newCols;
    }

    void CombiningImagesToptoBottom(const Image& otherImage) {
        int newRows = rows + otherImage.rows;
        int newCols = max(cols, otherImage.cols);

        vector<vector<int>> newImageData(newRows, vector<int>(newCols, 0));

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                newImageData[r][c] = ImageData[r][c];
            }
        }

        for (int r = 0; r < otherImage.rows; ++r) {
            for (int c = 0; c < otherImage.cols; ++c) {
                if (c < cols) {
                    newImageData[rows + r][c] = otherImage.ImageData[r][c];
                }
            }
        }
        ImageData = newImageData;
        rows = newRows;
        cols = newCols;
    }
};

struct Menu {
    vector<string> menuItems;

    Menu(char menuFile[]) {
        loadMenu(menuFile);
    }
    int loadMenu(char menuFile[]) {
        ifstream IN;
        IN.open(menuFile);
        if (!IN.is_open())
            return -1;
        char menuItem[100], TotalItems[10];

        int Choices;

        IN.getline(TotalItems, 8);
        Choices = atoi(TotalItems);
        for (int i = 1; i <= Choices; i++) {
            IN.getline(menuItem, 99);
            menuItems.push_back(menuItem);
        }
        IN.close();
        return Choices;
    }

    int presentMenu() {
        int userChoice;
        int totalChoices = menuItems.size();

        do {
            int k = 1;
            for (int i = 0; i < totalChoices; i++) {
                if (menuItems[i][0] != '*') {
                    cout << k << "\t" << menuItems[i] << endl;
                    k++;
                }
            }
            cout << " Enter Your Choice (1 - " << k - 1 << " ) ";
            cin >> userChoice;
        } while (userChoice < 1 || userChoice > totalChoices);
        return userChoice;
    }
};

int main() {

    const int BACK_TO_MAIN_MENU_OPTION = -1;
    char MenuFile[] = "MainMenu.txt";
    char ImageEnhancementFile[] = "ImageEnhancement.txt";
    char GeometricTransformationFile[] = "GeometricTransformation.txt";
    char ImageFilteringFile[] = "ImageFiltering.txt";
    char ImageTransformationFile[] = "ImageTransformation.txt";

    Image images[2];
    int activeImage = 0;
    int errorCode = 0;
    int userChoice;
    int totalChoices;
    Menu menu(MenuFile);

    Menu enhancementMenu(ImageEnhancementFile);
    Menu transformationMenu(GeometricTransformationFile);
    Menu filteringMenu(ImageFilteringFile);
    Menu transformingMenu(ImageTransformationFile);

    totalChoices = menu.menuItems.size();

    do {
        userChoice = menu.presentMenu();

        if (1 == userChoice) {

            char ImageFileName[100];
            cout << "Specify File Name ";
            cin >> ImageFileName;
            errorCode = images[activeImage].loadImage(ImageFileName);
            if (errorCode == 0) {
                cout << "File Loaded Successfully " << endl;
            }
            else {
                cout << "Load Error: Code " << errorCode << endl;
            }

        }

        else if (2 == userChoice) {

            char ImageFileName[100];
            cout << "Specify File Name ";
            cin >> ImageFileName;
            errorCode = images[activeImage].saveImage(ImageFileName);
            if (errorCode == 0) {
                cout << "File Saved as " << ImageFileName << endl;
            }
            else {
                cout << "Save Error: Code " << errorCode << endl;
            }

        }

        else if (3 == userChoice) {

            char command[200];
            sprintf_s(command, sizeof(command), "i_view64 %s", images[activeImage].ImageFileName);
            system(command);

            cout << "Image opened in IrfanView." << endl;

            continue;
        }

        else if (4 == userChoice) {
            do {
                userChoice = enhancementMenu.presentMenu();
                if (userChoice == BACK_TO_MAIN_MENU_OPTION) {
                    break;  
                }
                else if (1 == userChoice) {
                    double brightnessFactor;
                    cout << "Enter the brightness factor (e.g., 0.5 for darker, 2.0 for brighter): ";
                    cin >> brightnessFactor;

                    images[activeImage].changeBrightness(brightnessFactor);
                    cout << "Brightness changed suucessfully." << endl;
                    cout << "You need to save the image" << endl;

                }
                else if (2 == userChoice) {
                    images[activeImage].contrastStretching();
                    cout << "Contrast stretched successfully." << endl;
                    cout << "You need to save the image" << endl;
                }
                else if (3 == userChoice) {
                    images[activeImage].SharpeningofImage();
                    cout << "Image sharpened successfully" << endl;
                    cout << "You need to save the image" << endl;
                }

            } while (userChoice != enhancementMenu.menuItems.size()  && userChoice != BACK_TO_MAIN_MENU_OPTION);
        }

        else if (5 == userChoice) {
            do {
                userChoice = transformationMenu.presentMenu();
                if (userChoice == BACK_TO_MAIN_MENU_OPTION) {
                    break;  
                }
                else if (1 == userChoice) {
                    int horizontalShift, verticalShift;
                    cout << "Enter horizontal shift: ";
                    cin >> horizontalShift;
                    cout << "Enter vertical shift: ";
                    cin >> verticalShift;

                    images[activeImage].ImageTranslating(horizontalShift, verticalShift);
                
                    cout << "Image Translated Successfully" << endl;
                    cout << "You need to save the image" << endl;
                }
                else if (2 == userChoice) {

                    double scale_factor;
                    cout << "Enter the scaling factor (greater than 0): ";
                    cin >> scale_factor;

                    images[activeImage].ImageScaling(scale_factor);
                    cout << "Image scaled successfully." << endl;
                    cout << "You need to save the image" << endl;
                }
                else if (3 == userChoice) {
                    
                    double angle;
                    cout << "Enter the rotation angle (in degrees): ";
                    cin >> angle;

                    bool reverseRotation;
                    cout << "Rotate in reverse? (0 for forward, 1 for reverse): ";
                    cin >> reverseRotation;

                    images[activeImage].RotationofImage(angle, reverseRotation);
                    cout << "Image rotated successfully" << endl;
                    cout << "you need to save the image" << endl;

                }
            } while (userChoice != transformationMenu.menuItems.size() && userChoice != BACK_TO_MAIN_MENU_OPTION);
        }

        else if (6 == userChoice) {
            do {

                userChoice = filteringMenu.presentMenu();
                if (userChoice == BACK_TO_MAIN_MENU_OPTION) {
                    break;  
                }
                else if (1 == userChoice) {
         
                    images[activeImage].applyMeanFilter();
                    cout << "Mean filter applied." << endl;
                    cout << "You need to save the image" << endl;
                }
                else if (2 == userChoice) {

                    images[activeImage].applyMedianFilter();
                    cout << "Median filter applied." << endl;
                    cout << "You need to save the image" << endl;
                }
                else if (3 == userChoice) {

                    char filterFileName[100];
                    cout << "Enter filter file name: ";
                    cin >> filterFileName;

                    images[activeImage].applyLinearFilterFromFile(filterFileName);
                    cout << "Linear filter applied." << endl;
                    cout << "You need to save the image" << endl;
                }
                else if (4 == userChoice) {

                    cout << "Enter the size of the custom filter matrix (e.g., 3 for a 3x3 matrix): ";
                    int filterSize;
                    cin >> filterSize;
                    vector<vector<double>> customFilter(filterSize, vector<double>(filterSize));

                    cout << "Enter the values for the custom " << filterSize << "x" << filterSize << " filter matrix:" << endl;
                    for (int i = 0; i < filterSize; ++i) {
                        for (int j = 0; j < filterSize; ++j) {
                            cin >> customFilter[i][j];
                        }
                    }
                    images[activeImage].enhanceImageWithFilter(customFilter);

                    cout << "Custom filter applied to the image." << endl;
                    cout << "You need to save the image" << endl;
                }
                else if (5 == userChoice) {
                
                    images[activeImage].applyDerivativeFilter();
                    cout << "Derivatives computed." << endl;
                    cout << "You need to save the image." << endl;
                }
                else if (6 == userChoice) {
               
                    images[activeImage].findEdges();
                    cout << "Edges found using derivatives." << endl;
                    cout << "You need to save the image." << endl;
                }

            } while (userChoice != filteringMenu.menuItems.size() && userChoice != BACK_TO_MAIN_MENU_OPTION);
        }

        else if (7 == userChoice) {
            do {

                userChoice = transformingMenu.presentMenu();
                if (userChoice == BACK_TO_MAIN_MENU_OPTION) {
                    break; 
                }
                else if (1 == userChoice) {
                
                    images[activeImage].FlipImageHorizontally();
                    cout << "Image flipped successfully" << endl;
                    cout << "You need to save the changes " << endl;
                }
                else if (2 == userChoice) {
                   
                    images[activeImage].FlipImageVertically();
                    cout << "Image flipped successfully" << endl;
                    cout << "You need to save the changes" << endl;
                }
                else if (3 == userChoice) {
               
                    images[activeImage].rotate90Clockwise();
                    cout << "image rotated successfully" << endl;
                    cout << "You need to save the changes" << endl;
                }
                else if (4 == userChoice) {
                
                    images[activeImage].rotate90CounterClockwise();
                    cout << "image rotated successfully" << endl;
                    cout << "you need to save the changes" << endl;
                }
                else if (5 == userChoice) {
            
                    double angle;
                    cout << "Enter the angle in degree" << endl;
                    cin >> angle;
                    images[activeImage].rotateByAngle(angle);
                    cout << "image rotated successfully" << endl;
                    cout << "You need to save the changes" << endl;
                }
                else if (6 == userChoice) {
          
                    images[activeImage].convertToBinary();
                    cout << "Image converted to binary format" << endl;
                    cout << "You need to save the changes" << endl;
                }
                else if (7 == userChoice) {
        
                    double resizeRatio;
                    cout << "Enter the resize ratio: " << endl;
                    cin >> resizeRatio;
                    images[activeImage].resizeImage(resizeRatio);
                    cout << "Image resized" << endl;
                    cout << "You need to save the changes" << endl;
                }
                else if (8 == userChoice) {
              
                    int startRow, endRow, startCol, endCol;
                    cout << "Enter starting row, ending row, starting column, ending column (0-indexed) for cropping: ";
                    cin >> startRow >> endRow >> startCol >> endCol;
                    images[activeImage].CroppinganImage(startRow, endRow, startCol, endCol);
                    cout << "Image cropped" << endl;
                    cout << "You need to save the changes" << endl;
                }
                else if (9 == userChoice) {
         
                    char ImageFileName[100];
                    cout << "Specify File Name ";
                    cin >> ImageFileName;
                    errorCode = images[activeImage].loadImage(ImageFileName);
                    if (errorCode == 0) {
                        cout << "File Loaded Successfully " << endl;
                    }
                    else {
                        cout << "Load Error: Code " << errorCode << endl;
                    }
                    char ImageFileName2[100];
                    cout << "Specify File Name for Second Image (to be combined top-to-bottom): ";
                    cin >> ImageFileName2;
                    errorCode = images[1].loadImage(ImageFileName2);
                    if (errorCode == 0) {
                        images[0].CombiningImagesSidebySide(images[1]);
                        cout << "Images combined side by side." << endl;
                        cout << "You need to save the image." << endl;
                    }
                    else {
                        cout << "Load Error: Code " << errorCode << endl;
                    }

                }
                else if (10 == userChoice) {
                   
                    char ImageFileName[100];
                    cout << "Specify File Name ";
                    cin >> ImageFileName;
                    errorCode = images[activeImage].loadImage(ImageFileName);
                    if (errorCode == 0) {
                        cout << "File Loaded Successfully " << endl;
                    }
                    else {
                        cout << "Load Error: Code " << errorCode << endl;
                    }
                    char ImageFileName2[100];
                    cout << "Specify File Name for Second Image (to be combined top-to-bottom): ";
                    cin >> ImageFileName2;
                    errorCode = images[1].loadImage(ImageFileName2);
                    if (errorCode == 0) {
                        images[0].CombiningImagesToptoBottom(images[1]);
                        cout << "Images combined top-to-bottom." << endl;
                        cout << "You need to save the image." << endl;
                    }
                    else {
                        cout << "Load Error: Code " << errorCode << endl;
                    }
                }

            } while (userChoice != transformingMenu.menuItems.size() && userChoice != BACK_TO_MAIN_MENU_OPTION);
        }
        
    } while (userChoice != totalChoices);
    return 0;
}