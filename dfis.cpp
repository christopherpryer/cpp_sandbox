#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
using namespace std;

using vec = vector<string>;
using matrix = vector<vec>;

matrix readCSV(string filename)
{
    char separator = ',';
    matrix result;
    string row, item;

    ifstream in(filename);
    while(getline(in,row))
    {
        vec R;
        stringstream ss(row);
        while (getline(ss, item, separator)) R.push_back(item);
        result.push_back(R);
    }
    in.close();
    return result;
}

void printMatrix(const matrix &M)
{
    for (vec row : M)
    {
        for (string s: row) cout << setw(12) << left << s << " ";
        cout << '\n';
    }
}



/*
𝐴𝐷𝐼=𝑝𝑛/𝑑𝑛
𝑝𝑛 : number of periods
𝑑𝑛 : number of demands
𝐴𝐷𝐼 : Average Demand Interval
*/
double calculateADI(matrix M, int qCol)
{
    int rowCount = 0;
    int numPeriods = 0;
    int numDemands = 0;

    for (vec row: M)
    {
        if (rowCount > 0)
        {
            numPeriods++;
            numDemands+=std::stoi(row[qCol]);
        }
        rowCount++;
    }

    return (float)numPeriods/(float)numDemands;
}

/*
𝐶𝑉2=(𝜎𝑝/𝜇𝑝)2
𝜎𝑝: standard deviation of population
𝜇𝑝: average of population
𝐶𝑉2: coefficient of variation
*/
double calculateCV2(matrix M, int qCol)
{
    int rowCount = 0;
    int total = 0;
    int count = 0;

    for (vec row: M)
    {
        if (rowCount > 0)
        {
            total+=std::stoi(row[qCol]);
            count++;
        }
        rowCount++;
    }

    rowCount = 0;
    float var = 0.0;
    float mean = (float)total/(float)count;
    for (vec row: M)
    {
        if (rowCount > 0)
        {
            int val = std::stoi(row[qCol]); 
            var += (val - mean) * (val - mean);
        }
        rowCount++;
    }

    var /= count;
    return pow((sqrt(var)/mean), 2);;
}

bool isSmooth(float adi, float cv2)
{
    if ((adi < 1.32) && (cv2 < 0.49))
    {
        return true;
    }
    else 
    {
        return false;
    }
}

int main()
{
    matrix data = readCSV("test_data.csv");
    
    //printMatrix(data);
    float adi = calculateADI(data, 1);
    float cv2 = calculateCV2(data, 1);

    printf("adi:\t%f\n", adi);
    printf("cv2:\t%f\n", cv2);
    
    printf("is smooth:\t%i\n", isSmooth(adi, cv2));
}