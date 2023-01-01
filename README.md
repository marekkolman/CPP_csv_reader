# C++ csv reader
Simple library to load numeric data to C++ vectors from csv files.

The code is in `csv_reader.h`

The library loads csv as either
- vector: `vector<int>, vector<double>`,
- matrix: `vector<vector<int>>, vector<vector<double>>`


Use calls such as
```
vector<vector<double>> mat = readcsv_matrix<double>(R"(c:\temp\test\data.csv)", true);
vector<double> res = readcsv_vector<double>(R"(c:\temp\test\data.csv)", false);
```
By default header in the csv file is skipped for matrix data and NOT skipped for vector data.


