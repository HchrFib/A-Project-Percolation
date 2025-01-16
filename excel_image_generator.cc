#include "excel_image_generator.h"
#include <iostream>

using namespace std;

static lxw_workbook *workbook = nullptr;  // Global pointer to work book 

void initialize_excel(const std::string& filename) {
    
    workbook = workbook_new(filename.c_str());  // Create a new workbook and store it in the global variable
    if (!workbook) {
        cerr << "Error creating workbook: " << filename << endl;
    }
}

void create_excel(const vector<double>& x_data, const vector<double>& y_data, const string& x_label, const string& y_label, const string& sheet_name, double threshold_probability) {
    if (!workbook) {
        cerr << "Workbook not initialized. Call initialize_excel firSt." << endl;
        return;
    }
   
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, sheet_name.c_str()); // Create a new worksheet within the workbook

    // Write the headers for the x and y data 
    worksheet_write_string(worksheet, 0, 0, x_label.c_str(), NULL);
    worksheet_write_string(worksheet, 0, 1, y_label.c_str(), NULL);
    worksheet_write_string(worksheet, 0, 2,"threshold_probability", NULL);



    // Write the data into the worksheet
    for (size_t i = 0; i < x_data.size(); i++) {
        worksheet_write_number(worksheet, i + 1, 0, x_data[i], NULL);
        worksheet_write_number(worksheet, i + 1, 1, y_data[i], NULL);
    }

    worksheet_write_number(worksheet, 1, 2, threshold_probability, NULL);

    // Create a chart object
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_SCATTER);
    
    // Set the chart type (scatter with smooth lines)
    chart->type = LXW_CHART_SCATTER_SMOOTH;

    // Configure the chart data range
    // X values (first column)
    string x_range = "=\'" + sheet_name + "\'!$A$2:$A$" + to_string(x_data.size() + 1);
    // Y values (second column)
    string y_range = "=\'" + sheet_name + "\'!$B$2:$B$" + to_string(y_data.size() + 1);

    // Add series to the chart
    chart_add_series(chart, x_range.c_str(), y_range.c_str());

    // Add axis labe
    chart_axis_set_name(chart->x_axis, x_label.c_str());
    chart_axis_set_name(chart->y_axis, y_label.c_str());

    // Insert the chart into the worksheet at cell E2
    worksheet_insert_chart(worksheet, 1, 4, chart);
    cout << "Worksheet " << sheet_name << " created with chart." << endl;

    
}

void finalize_excel() {
    if (workbook) {
        // Close the workbook
        workbook_close(workbook);
        workbook = nullptr;
        cout << "Excel file closed successfully." << endl;
    } else {
        cerr << "Workbook not open." << endl;
    }
}
