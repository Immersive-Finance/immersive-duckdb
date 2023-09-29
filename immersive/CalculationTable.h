#pragma once
#pragma once
#include "BlackScholes.h"
#include "duckdb.hpp"

#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <sstream>
#include <string>



using duckdb::DataChunk;
using duckdb::ExpressionState;
using duckdb::FlatVector;
using duckdb::LogicalType;
using duckdb::StringVector;
using duckdb::Vector;
using duckdb::VectorType;
using duckdb::unique_ptr;

using duckdb::idx_t;
using duckdb::Value;


class CalculationTable {
public:
	CalculationTable(bool share = true);
	~CalculationTable();

	void create_table();
	void insert_row(int scenario_id, int trade_id, double notional, int cp, double K, double T, double sqrt_T,
	                double spot, double fwd, double vol, std::string greek);
	void calculate();

	void append_rows(const std::vector<std::tuple<int, int, double, int, double, double, double, double, double, double, std::string>> &rows);

public:
	const duckdb::shared_ptr<duckdb::Connection> &getConn() const;
	const std::string &getDbname() const;
	const std::string &getTableName() const;

private:
	duckdb::shared_ptr<duckdb::Connection> conn;
	std::string dbname;
	std::string table_name;
};
