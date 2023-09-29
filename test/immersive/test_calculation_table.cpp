#include "CalculationTable.h"
#include "catch.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/common/to_string.hpp"
#include "duckdb/common/types/value.hpp"
#include "duckdb/common/vector.hpp"
#include "duckdb/main/calculation_table.hpp"

#include <cstring>
#include <string>

using namespace duckdb;

static std::vector<std::tuple<int, int, double, int, double, double, double, double, double, double, std::string>>
generate_options(int n = 10) {
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

	for (int i = 0; i < n; ++i) {
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
	return batch_rows;
}

TEST_CASE("Test calculation table") {

}

TEST_CASE("Test calculation table pricing", "[immersive]") {

	CalculationTable calc_table;
	calc_table.create_table();

	// just as we do below for the call to calculate, we shoudl also add code here to say how long this loop took to insert that data
	// into the table
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::tuple<int, int, double, int, double, double, double, double, double, double, std::string>> batch_rows;
	batch_rows = generate_options();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "GENERATE - elapsed time: " << elapsed.count() << " s\n";

	// APPEND
	start = std::chrono::high_resolution_clock::now();
	calc_table.append_rows(batch_rows);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "APPEND - elapsed time: " << elapsed.count() << " s\n";


	// CALCULATE
	start = std::chrono::high_resolution_clock::now();
	calc_table.calculate();
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "CALCULATE - elapsed time: " << elapsed.count() << " s\n";


}

TEST_CASE("Test calculation table pricing wrapper", "[immersive]")
{

	CalculationTableWrapper calc_table;

	int row_count = 9;

	// just as we do below for the call to calculate, we shoudl also add code here to say how long this loop took to insert that data
	// into the table
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::tuple<int, int, double, int, double, double, double, double, double, double, std::string>>
	    batch_rows = generate_options(row_count);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "GENERATE - elapsed time: " << elapsed.count() << " s\n";

	// APPEND
	start = std::chrono::high_resolution_clock::now();
	calc_table.append_rows(batch_rows);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "APPEND - elapsed time: " << elapsed.count() << " s\n";


	// CALCULATE
	start = std::chrono::high_resolution_clock::now();
	calc_table.calculate();
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "CALCULATE - elapsed time: " << elapsed.count() << " s\n";

	auto result = calc_table.get_conn()->Query("SELECT COUNT(*) FROM " + calc_table.get_table_name());


	REQUIRE(!result->HasError());
	int32_t actual = result->Fetch()->GetValue(0, 0).GetValue<int32_t>();
	REQUIRE(actual == row_count);



}

