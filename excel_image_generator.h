#ifndef EXCEL_IMAGE_GENERATOR_H
#define EXCEL_IMAGE_GENERATOR_H

#include <string>
#include <vector>
#include <xlsxwriter.h>

using namespace std;

// Function to initialize the Excel file
void initialize_excel(const std::string& filename);

// Function to create a new worksheet with the given data and generate a chart
void create_excel(const vector<double>& x_data, const vector<double>& y_data, const string& x_label, const string& y_label, const string& sheet_name, double threshold_probability);

// Function to finalize and close the Excel file
void finalize_excel();

#endif 

