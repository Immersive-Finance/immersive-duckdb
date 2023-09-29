#include "duckdb.hpp"
#include <chrono>

#include <iostream>
#include <mutex>
#include <random>
#include <sstream>
#include <string>

#include "CalculationTable.h"

std::mutex id_generation_mutex;
std::string generate_unique_id() {
	std::lock_guard<std::mutex> lock(id_generation_mutex);

	auto time_since_epoch = std::chrono::system_clock::now().time_since_epoch();
	auto time_as_int = std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch).count();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(1000, 9999);

	std::stringstream ss;
	ss << time_as_int << "_" << distr(gen);

	return ss.str();
}

static void calculate_pv(duckdb::DataChunk &input, duckdb::ExpressionState &state, duckdb::Vector &result)
{

	result.SetVectorType(duckdb::VectorType::FLAT_VECTOR);
	auto result_data = duckdb::FlatVector::GetData<double>(result);

	auto notional = duckdb::FlatVector::GetData<double>(input.data[0]);
	auto cp = duckdb::FlatVector::GetData<int>(input.data[1]);
	auto K = duckdb::FlatVector::GetData<double>(input.data[2]);
	auto spot = duckdb::FlatVector::GetData<double>(input.data[3]);
	auto fwd = duckdb::FlatVector::GetData<double>(input.data[4]);
	auto vol = duckdb::FlatVector::GetData<double>(input.data[5]);
	auto T = duckdb::FlatVector::GetData<double>(input.data[6]);
	auto sqrt_T = duckdb::FlatVector::GetData<double>(input.data[7]);

	for (duckdb::idx_t i = 0; i < input.size(); i++) {

		result_data[i] = pv(notional[i], cp[i], K[i], spot[i], fwd[i], vol[i], T[i], sqrt_T[i]);
	}

}

CalculationTable::CalculationTable(bool share) {


	dbname = share ? "immersive_calculation_table.db" : ":memory:";
	auto db = duckdb::make_uniq<duckdb::DuckDB>(dbname);
	conn = duckdb::make_uniq<duckdb::Connection>(*db);
	table_name = share ? "calculation_table_" + generate_unique_id() : "calculation_table";

	create_table();

	conn->CreateVectorizedFunction("update_value",
	                               {duckdb::LogicalType::DOUBLE, duckdb::LogicalType::INTEGER, duckdb::LogicalType::DOUBLE, duckdb::LogicalType::DOUBLE,
	                                duckdb::LogicalType::DOUBLE, duckdb::LogicalType::DOUBLE, duckdb::LogicalType::DOUBLE, duckdb::LogicalType::DOUBLE},
	                               duckdb::LogicalType::DOUBLE, calculate_pv);


}

CalculationTable::~CalculationTable() {

	std::string drop_table_query = "DROP TABLE " + table_name + ";";
	conn->Query(drop_table_query);
}

void CalculationTable::create_table() {
	std::string create_table_query = "CREATE TABLE " + table_name +
	                                 " ("
	                                 "scenario_id INTEGER, "
	                                 "trade_id INTEGER, "
	                                 "notional REAL, "
	                                 "cp INTEGER, "
	                                 "K REAL, "
	                                 "T REAL, "
	                                 "sqrt_T REAL, "
	                                 "spot REAL, "
	                                 "fwd REAL, "
	                                 "vol REAL, "
	                                 "greek TEXT, "
	                                 "pv REAL, "
	                                 "value REAL);";

	conn->Query(create_table_query);
}

void CalculationTable::insert_row(int scenario_id, int trade_id, double notional, int cp, double K, double T,
                                         double sqrt_T, double spot, double fwd, double vol, std::string greek) {

	std::string insert_query = "INSERT INTO " + table_name +
	                           " (scenario_id, trade_id, notional, cp, K, T, sqrt_T, spot, fwd, vol, greek) "
	                           "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

	auto prepared = conn->Prepare(insert_query);
	prepared->Execute(scenario_id, trade_id, notional, cp, K, T, sqrt_T, spot, fwd, vol, greek);
}

void CalculationTable::calculate() {

	auto result =
	    conn->Query("UPDATE " + table_name + " SET pv = update_value(notional, cp, K, spot, fwd, vol, T, sqrt_T);");
	if (result->HasError())
		result->ThrowError("Calc FAILED - ");
	//result->Print();

//	result = conn->Query("SELECT * FROM  " + table_name);
//	if (result->HasError())
//		result->ThrowError();
	//result->Print();
}

void CalculationTable::append_rows(const std::vector<std::tuple<int, int, double, int, double, double, double, double, double, double, std::string>> &rows) {

	duckdb::Appender appender(*conn, table_name);

	// Loop to insert 1 million rows
	for (uint64_t i = 0; i < rows.size(); ++i) {

		//read the variables from the rows vector
		int scenario_id = std::get<0>(rows[i]);
		int trade_id = std::get<1>(rows[i]);
		double notional = std::get<2>(rows[i]);
		int cp = std::get<3>(rows[i]);
		double K = std::get<4>(rows[i]);
		double T = std::get<5>(rows[i]);
		double sqrt_T = std::get<6>(rows[i]);
		double spot = std::get<7>(rows[i]);
		double fwd = std::get<8>(rows[i]);
		double vol = std::get<9>(rows[i]);
		const std::string greek = std::get<10>(rows[i]);


		// Append a row using the Appender
		appender.BeginRow();
		appender.Append(scenario_id);
		appender.Append(trade_id);
		appender.Append(notional);
		appender.Append(cp);
		appender.Append(K);
		appender.Append(T);
		appender.Append(sqrt_T);
		appender.Append(spot);
		appender.Append(fwd);
		appender.Append(vol);
		appender.Append(greek.c_str());
		appender.Append(duckdb::Value());
		appender.Append(duckdb::Value());
		appender.EndRow();
	}

	// Commit the transaction
	appender.Close();

}
const duckdb::shared_ptr<duckdb::Connection> &CalculationTable::getConn() const {
	return conn;
}
const std::string &CalculationTable::getDbname() const {
	return dbname;
}
const std::string &CalculationTable::getTableName() const {
	return table_name;
}
