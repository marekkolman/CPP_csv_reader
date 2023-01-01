#include "csv_reader.h"

int main()
{
    vector<vector<int>> mat = readcsv_matrix<int>(R"(/Users/marekkolman/CLionProjects/untitled6/data.csv)", true);

    return 0;
}