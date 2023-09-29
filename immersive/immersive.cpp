// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "CalculationTable.h" // Assuming this is where your class is defined

namespace py = pybind11;

PYBIND11_MODULE(immersive_test_duckdb, m) {
	py::class_<CalculationTable>(m, "CalculationTable")
	    .def(py::init<bool>(), py::arg("share") = true) // constructor with default argument
	    .def("create_table", &CalculationTable::create_table)
	    .def("insert_row", &CalculationTable::insert_row, py::arg("scenario_id"), py::arg("trade_id"),
	         py::arg("notional"), py::arg("cp"), py::arg("K"), py::arg("T"), py::arg("sqrt_T"), py::arg("spot"),
	         py::arg("fwd"), py::arg("vol"), py::arg("greek"))
	    .def("calculate", &CalculationTable::calculate);
}


//#include "CalculationTable.h"
//
//#include <iostream>
//
//int main() {
//	try {
//		CalculationTable calc_table;
//		calc_table.insert_row(1, 1, 1, 1, 104, 1, 1, 101, 102, 0.2, "PV");
//		calc_table.calculate();
//	} catch (const std::exception &ex) {
//		std::cerr << "Caught exception: " << ex.what() << std::endl;
//		return 1;
//	}
//}

// int main()
//{
//     try {
//         duckdb::DuckDB db(nullptr);
//         duckdb::Connection con(db);
//
//         auto res = con.Query("CREATE TABLE myTable(i varchar(1), j INTEGER)");
//
//         if (res->HasError()) {
//             auto errobj = res->GetErrorObject();
//             auto err = res->GetError();
//
//             std::cout << "Create failed!" << std::endl;
//             return 4;
//         }
//
//         auto prepared = con.Prepare("INSERT INTO myTable VALUES (?, ?);");
//         if (!prepared->success) {
//             prepared->GetErrorObject().Throw();
//             std::cout << "Preparation failed: " << prepared->error << std::endl;
//             return 1;
//         }
//
//
//         for (int i = 0; i < 10; i++) {
//             std::string random_group_id = "";
//             int random_num = rand() % 10;
//             if (random_num % 3 == 0) {
//                 random_group_id = "A";
//             }
//             else if (random_num % 2 == 0) {
//                 random_group_id = "B";
//             }
//             else {
//                 random_group_id = "C";
//             }
//
//
//             auto res = prepared->Execute(random_group_id, random_num);
//             if (res->HasError() ){
//				std::cout << "Insert failed!" << std::endl;
//
//				return 3;
//			}
//
//
//         }
//
//         auto results = con.Query("SELECT * FROM myTable");
//         if (results->HasError()) {
//             std::cout << "Query failed!" << std::endl;
//             std::cout << results->GetError() << std::endl;
//             return 2;
//         }
//         results->Print();
//         std::cout << std::endl;
//
//         results = con.Query("CREATE TABLE mytable2 AS SELECT *, j * j as res FROM myTable");
//         if (results->HasError()) {
//             std::cout << "Query failed!" << std::endl;
//             results->GetErrorObject().Throw("Error createing mytable2");
//             return 2;
//         }
//
//         results = con.Query("SELECT * FROM myTable2");
//         if (results->HasError()) {
//             std::cout << "Query failed!" << std::endl;
//             std::cout << results->GetError() << std::endl;
//             return 2;
//         }
//         results->Print();
//         std::cout << std::endl;
//
//
//         return 0;
//     }
//     catch (const std::exception& ex) {
//         std::cerr << "Caught exception: " << ex.what() << std::endl;
//         return 1;
//     }
// }

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files
//   to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
