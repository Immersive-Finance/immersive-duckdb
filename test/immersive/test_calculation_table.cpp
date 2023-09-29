#include "duckdb/common/string_util.hpp"
#include "duckdb/common/types/value.hpp"
#include "duckdb/common/to_string.hpp"
#include "CalculationTable.h"


#include "catch.hpp"

#include "duckdb/common/vector.hpp"
#include <string>
#include <cstring>

using namespace duckdb;

TEST_CASE("Test calculation table") {

	int x = 9;
	REQUIRE(x == 9);
}

TEST_CASE("Test calculation table pricing", "[immersive]") {

	/*
	 * first we create a calculation table
	 *
	 * we then insert 100 rows into itusing this function

	 inline void CalculationTable::insert_row(int scenario_id, int trade_id, double notional, int cp, double K, double T,
	                                     double sqrt_T, double spot, double fwd, double vol, std::string greek)

	 sceanrio_id = 1, trade_id is an ascending integer, notional is beween 0.5 and 50, cp is 1 or -1, K is between 10 and 200
	 T is between 0.02 and 1.5, sqrt_T the square root of T, spot is between 60 and 150, fwd is between spot * 0.85 and spot * 1.5 , vol is between 0.1 and 1.5,
	 and greek is PV

	 */
	CalculationTable calc_table;
	calc_table.create_table();

	// just as we do below for the call to calculate, we shoudl also add code here to say how long this loop took to insert that data
	// into the table
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::tuple<int, int, double, int, double, double, double, double, double, double, std::string>> batch_rows;

	int scenario_id = 1;
	int trade_id;
	double notional;
	int cp;
	double K;
	double T;
	double sqrt_T;
	double spot;
	double fwd;
	double vol;
	std::string greek = "PV"; // Constant, no need to redefine

	for (int i = 0; i < 1000000; ++i) {
		trade_id = i;
		notional = 0.5 + (50 - 0.5) * (double)rand() / RAND_MAX;
		cp = rand() % 2 == 0 ? 1 : -1;
		K = 10 + (200 - 10) * (double)rand() / RAND_MAX;
		T = 0.02 + (1.5 - 0.02) * (double)rand() / RAND_MAX;
		sqrt_T = sqrt(T);
		spot = 60 + (150 - 60) * (double)rand() / RAND_MAX;
		fwd = spot * 0.85 + (spot * 1.5 - spot * 0.85) * (double)rand() / RAND_MAX;
		vol = 0.1 + (1.5 - 0.1) * (double)rand() / RAND_MAX;

		batch_rows.emplace_back(scenario_id, trade_id, notional, cp, K, T, sqrt_T, spot, fwd, vol, greek);
	}



	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "GENERATE - elapsed time: " << elapsed.count() << " s\n";


	start = std::chrono::high_resolution_clock::now();
	calc_table.insert_rows_using_appender(batch_rows);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "INSERT - elapsed time: " << elapsed.count() << " s\n";

	// now we perform calculate on the table. But we add some timing code that captures the time before,
	// and the time after and prints out the elaped time for the call

	start = std::chrono::high_resolution_clock::now();
	calc_table.calculate();
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "CALCULATE - elapsed time: " << elapsed.count() << " s\n";


}

