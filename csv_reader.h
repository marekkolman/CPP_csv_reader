#pragma once
//
// Created by Marek Kolman on 31.12.2022.
//
// sada dvou funkci pro cteni CSV do formatu vector nebo matice, tj. vector<vector>>
//
// vector<vector<double>> mat = readcsv_matrix<double>(R"(c:\temp\test\data.csv)", true);
// vector<double> res = readcsv_vector<double>(R"(c:\temp\test\data.csv)", false);
//

#include <vector>
#include <string>
#include <type_traits>
#include <fstream>

using namespace std;


class CSVRow
{
public:
    std::string operator[](std::size_t index) const
    {
        return std::string(&m_line[m_data[index] + 1], m_data[index + 1] - (m_data[index] + 1));
    }
    std::size_t size() const
    {
        return m_data.size() - 1;
    }
    void readNextRow(std::istream& str)
    {
        std::getline(str, m_line);

        m_data.clear();
        m_data.emplace_back(-1);
        std::string::size_type pos = 0;
        while ((pos = m_line.find(',', pos)) != std::string::npos)
        {
            m_data.emplace_back(pos);
            ++pos;
        }
        // This checks for a trailing comma with no data after it.
        pos = m_line.size();
        m_data.emplace_back(pos);
    }
private:
    std::string         m_line;
    std::vector<int>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}

class CSVIterator
{
public:
    typedef std::input_iterator_tag     iterator_category;
    typedef CSVRow                      value_type;
    typedef std::size_t                 difference_type;
    typedef CSVRow* pointer;
    typedef CSVRow& reference;

    CSVIterator(std::istream& str) :m_str(str.good() ? &str : nullptr) { ++(*this); }
    CSVIterator() :m_str(nullptr) {}

    // Pre Increment
    CSVIterator& operator++() { if (m_str) { if (!((*m_str) >> m_row)) { m_str = nullptr; } }return *this; }
    // Post increment
    CSVIterator operator++(int) { CSVIterator    tmp(*this);++(*this);return tmp; }
    CSVRow const& operator*()   const { return m_row; }
    CSVRow const* operator->()  const { return &m_row; }

    bool operator==(CSVIterator const& rhs) { return ((this == &rhs) || ((this->m_str == nullptr) && (rhs.m_str == nullptr))); }
    bool operator!=(CSVIterator const& rhs) { return !((*this) == rhs); }
private:
    std::istream* m_str;
    CSVRow              m_row;
};


class CSVRange
{
    std::istream& stream;
public:
    CSVRange(std::istream& str)
            : stream(str)
    {}
    CSVIterator begin() const { return CSVIterator{ stream }; }
    CSVIterator end()   const { return CSVIterator{}; }
};

template<typename T>
vector<vector<T>> readcsv_matrix(string filepath, bool skip_header = true) {

    /*
        Funkce precte csv numerickymi daty a vrati matici, tedy vector<vector<>>
        Matice muze byt dat typu int nebo double
        By default je preskocen header, tedy prvni radek
        volame napr takto: vector<vector<double>> mat = readcsv_matrix<double>(R"(c:\temp\test\data.csv)", true);
    */

    std::ifstream  file(filepath);

    vector<vector<T>> mat;

    string s;
    int j = 0;

    for (auto& row : CSVRange(file))
    {
        if ((j == 0) && skip_header) {}
        else
        {
            vector<T> v;
            for (int i = 0; i < row.size();i++) {
                s = row[i];

                if (is_floating_point_v<T>)
                {
                    v.push_back(stod(s));
                }
                else
                {
                    v.push_back(stoi(s));
                }

            }
            mat.push_back(v);
        }
        j++;
    }
    return mat;

}

template<typename T>
vector<T> readcsv_vector(string filepath, bool skip_header = false) {
    /*
        Funkce precte csv s jednim sloupcem a vrati vektor typu int nebo double
        Volame napr takto: vector<double> res = readcsv_vector<double>(R"(c:\temp\test\data.csv)", false);
    */


    std::ifstream  file(filepath);

    vector<T> v;

    string s;
    int j = 0;

    for (auto& row : CSVRange(file))
    {
        if ((j == 0) && skip_header) {}
        else
        {
            s = row[0];

            if (is_floating_point_v<T>)
            {
                v.push_back(stod(s));
            }
            else
            {
                v.push_back(stoi(s));
            }
        }
        j++;
    }
    return v;

}
